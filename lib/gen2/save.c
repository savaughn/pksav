/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util/fs.h"

#include <pksav/gen2/save.h>

#include <pksav/math/endian.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define PKSAV_GS_CHECKSUM1 0x2D69
#define PKSAV_GS_CHECKSUM2 0x7E6D

#define PKSAV_CRYSTAL_CHECKSUM1 0x2D02
#define PKSAV_CRYSTAL_CHECKSUM2 0x1F0D

struct pksav_gen2_save_internal
{
    uint8_t* raw_save_ptr;

    uint16_t* checksum1_ptr;
    uint16_t* checksum2_ptr;
};

enum pksav_gen2_field
{
    PKSAV_GEN2_PLAYER_ID = 0,
    PKSAV_GEN2_PLAYER_NAME,
    PKSAV_GEN2_RIVAL_NAME,
    PKSAV_GEN2_DAYLIGHT_SAVINGS,
    PKSAV_GEN2_TIME_PLAYED,
    PKSAV_GEN2_PLAYER_PALETTE,
    PKSAV_GEN2_MONEY,
    PKSAV_GEN2_JOHTO_BADGES,
    PKSAV_GEN2_KANTO_BADGES,
    PKSAV_GEN2_ITEM_BAG,
    PKSAV_GEN2_ITEM_PC,
    PKSAV_GEN2_CURRENT_BOX_NUM,
    PKSAV_GEN2_PC_BOX_NAMES,
    PKSAV_GEN2_POKEMON_PARTY,
    PKSAV_GEN2_POKEDEX_OWNED,
    PKSAV_GEN2_POKEDEX_SEEN,
    PKSAV_GEN2_CURRENT_BOX,
    PKSAV_GEN2_PLAYER_GENDER,
    PKSAV_GEN2_POKEMON_PC_FIRST_HALF,
    PKSAV_GEN2_POKEMON_PC_SECOND_HALF,
    PKSAV_GEN2_CHECKSUM1,
    PKSAV_GEN2_CHECKSUM2
};

static const size_t GS_OFFSETS[] =
{
    0x2009, // Player ID
    0x200B, // Player name
    0x2021, // Rival name
    0x2037, // Daylight savings
    0x2053, // Time played
    0x206B, // Player palette
    0x23DB, // Money
    0x23E4, // Johto badges
    0x23E5, // Kanto badges
    0x23E6, // Item bag
    0x247E, // Item PC
    0x2724, // Current Pokemon box number
    0x2727, // PC box names
    0x288A, // Pokemon party
    0x2A4C, // Pokedex owned
    0x2A6C, // Pokedex seen
    0x2D6C, // Current Pokemon box list
    0x3E3D, // Player gender (Crystal only)
    0x4000, // Pokemon PC (first half)
    0x6000, // Pokemon PC (second half)
    0x2D69, // Checksum 1
    0x7E6D  // Checksum 2
};

static const size_t CRYSTAL_OFFSETS[] =
{
    0x2009, // Player ID
    0x200B, // Player name
    0x2021, // Rival name
    0x2037, // Daylight savings
    0x2054, // Time played
    0x206A, // Player palette
    0x23DB, // Money
    0x23E5, // Johto badges
    0x23E6, // Kanto badges
    0x23E7, // Item bag
    0x247F, // Item PC
    0x2700, // Current Pokemon box number
    0x2703, // PC box names
    0x2865, // Pokemon party
    0x2A27, // Pokedex owned
    0x2A47, // Pokedex seen
    0x2D10, // Current Pokemon box list
    0x3E3D, // Player gender (Crystal only)
    0x4000, // Pokemon PC (first half)
    0x6000, // Pokemon PC (second half)
    0x2D0D, // Checksum 1
    0x1F0D  // Checksum 2
};

static void _pksav_gen2_get_save_checksums(
    enum pksav_gen2_save_type save_type,
    const uint8_t* buffer,
    uint16_t* checksum1_out,
    uint16_t* checksum2_out
)
{
    assert(buffer != NULL);
    assert(checksum1_out != NULL);
    assert(checksum2_out != NULL);

    *checksum1_out = 0;
    *checksum2_out = 0;

    switch(save_type)
    {
        case PKSAV_GEN2_SAVE_TYPE_GS:
            for(size_t buffer_index = 0x2009; buffer_index <= 0x2D68; ++buffer_index)
            {
                *checksum1_out += buffer[buffer_index];
            }

            for(size_t buffer_index = 0x0C6B; buffer_index <= 0x17EC; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            for(size_t buffer_index = 0x3D96; buffer_index <= 0x3F3F; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            for(size_t buffer_index = 0x7E39; buffer_index <= 0x7E6C; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            break;

        case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
            for(size_t buffer_index = 0x2009; buffer_index <= 0x2B82; ++buffer_index)
            {
                *checksum1_out += buffer[buffer_index];
            }

            for(size_t buffer_index = 0x1209; buffer_index <= 0x1D82; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            break;

        default:
            assert(false);
    }

    *checksum1_out = pksav_littleendian16(*checksum1_out);
    *checksum2_out = pksav_littleendian16(*checksum2_out);
}

enum pksav_error pksav_gen2_get_buffer_save_type(
    const uint8_t* buffer,
    size_t buffer_len,
    enum pksav_gen2_save_type* save_type_out
)
{
    if(!buffer || !save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    *save_type_out = PKSAV_GEN2_SAVE_TYPE_NONE;
    if(buffer_len >= PKSAV_GEN2_SAVE_SIZE)
    {
        bool is_type_found = false;
        for(enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_GS;
            (save_type <= PKSAV_GEN2_SAVE_TYPE_CRYSTAL) && !is_type_found;
            ++save_type)
        {
            uint16_t buffer_checksum1 = 0;
            uint16_t buffer_checksum2 = 0;
            _pksav_gen2_get_save_checksums(
                save_type,
                buffer,
                &buffer_checksum1,
                &buffer_checksum2
            );

            size_t checksum1_index = 0;
            size_t checksum2_index = 0;
            switch(save_type)
            {
                case PKSAV_GEN2_SAVE_TYPE_GS:
                    checksum1_index = PKSAV_GS_CHECKSUM1;
                    checksum2_index = PKSAV_GS_CHECKSUM2;
                    break;

                case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
                    checksum1_index = PKSAV_CRYSTAL_CHECKSUM1;
                    checksum2_index = PKSAV_CRYSTAL_CHECKSUM2;
                    break;

                default:
                    assert(false);
            }

            const uint16_t* checksum1_ptr = (const uint16_t*)&buffer[checksum1_index];
            const uint16_t* checksum2_ptr = (const uint16_t*)&buffer[checksum2_index];

            uint16_t checksum1 = pksav_littleendian16(*checksum1_ptr);
            uint16_t checksum2 = pksav_littleendian16(*checksum2_ptr);

            switch(save_type)
            {
                case PKSAV_GEN2_SAVE_TYPE_GS:
                    is_type_found = (checksum1 == buffer_checksum1) &&
                                    (checksum2 == buffer_checksum2);
                    break;

                /*
                 * From what I've seen, valid Crystal saves don't always have both
                 * checksums set correctly.
                 */
                case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
                    is_type_found = (checksum1 == buffer_checksum1) ||
                                    (checksum2 == buffer_checksum2);
                    break;

                default:
                    assert(false);
            }

            if(is_type_found)
            {
                *save_type_out = save_type;
            }
        }
    }

    return error;
}

enum pksav_error pksav_gen2_get_file_save_type(
    const char* filepath,
    enum pksav_gen2_save_type* save_type_out
)
{
    if(!filepath || !save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                filepath,
                &file_buffer,
                &buffer_len
            );
    if(!error)
    {
        assert(file_buffer != NULL);

        enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
        error = pksav_gen2_get_buffer_save_type(
                    file_buffer,
                    buffer_len,
                    &save_type
                );
        free(file_buffer);

        // Only return a result upon success.
        if(!error)
        {
            *save_type_out = save_type;
        }
    }

    return error;
}

static void _pksav_gen2_set_save_pointers(
    struct pksav_gen2_save* gen2_save_ptr,
    uint8_t* file_buffer
)
{
    assert(gen2_save_ptr != NULL);
    assert(file_buffer != NULL);

    const size_t* offsets = NULL;

    // Internal
    gen2_save_ptr->internal_ptr = calloc(sizeof(struct pksav_gen2_save_internal), 1);
    struct pksav_gen2_save_internal* internal_ptr = gen2_save_ptr->internal_ptr;
    internal_ptr->raw_save_ptr = file_buffer;
    switch(gen2_save_ptr->save_type)
    {
        case PKSAV_GEN2_SAVE_TYPE_GS:
            offsets = GS_OFFSETS;
            break;

        case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
            offsets = CRYSTAL_OFFSETS;
            break;

        default:
            assert(false);
    }

    assert(offsets != NULL);

    internal_ptr->checksum1_ptr = (uint16_t*)&file_buffer[
                                                 offsets[PKSAV_GEN2_CHECKSUM1]
                                             ];

    internal_ptr->checksum2_ptr = (uint16_t*)&file_buffer[
                                                 offsets[PKSAV_GEN2_CHECKSUM2]
                                             ];

    // Save time
    struct pksav_gen2_save_time* save_time_ptr = &gen2_save_ptr->save_time;

    save_time_ptr->time_played_ptr = (struct pksav_gen2_time*)(
                                         &file_buffer[offsets[PKSAV_GEN2_TIME_PLAYED]]
                                     );
    save_time_ptr->daylight_savings_ptr = &file_buffer[offsets[PKSAV_GEN2_DAYLIGHT_SAVINGS]];

    // Item storage
    struct pksav_gen2_item_storage* item_storage_ptr = &gen2_save_ptr->item_storage;

    item_storage_ptr->item_bag_ptr = (struct pksav_gen2_item_bag*)(
                                         &file_buffer[offsets[PKSAV_GEN2_ITEM_BAG]]
                                     );
    item_storage_ptr->item_pc_ptr = (struct pksav_gen2_item_pc*)(
                                        &file_buffer[offsets[PKSAV_GEN2_ITEM_PC]]
                                    );

    // Pokémon storage
    struct pksav_gen2_pokemon_storage* pokemon_storage_ptr = &gen2_save_ptr->pokemon_storage;

    pokemon_storage_ptr->party_ptr = (struct pksav_gen2_pokemon_party*)(
                                         &file_buffer[offsets[PKSAV_GEN2_POKEMON_PARTY]]
                                     );

    for(size_t box_index = 0; box_index < 7; ++box_index)
    {
        size_t offset = offsets[PKSAV_GEN2_POKEMON_PC_FIRST_HALF] +
                        (sizeof(struct pksav_gen2_pokemon_box) * box_index);

        pokemon_storage_ptr->box_ptrs[box_index] = (struct pksav_gen2_pokemon_box*)(
                                                       &file_buffer[offset]
                                                   );
    }
    for(size_t box_index = 7; box_index < 14; ++box_index)
    {
        size_t offset = offsets[PKSAV_GEN2_POKEMON_PC_SECOND_HALF] +
                        (sizeof(struct pksav_gen2_pokemon_box) * (box_index - 7));

        pokemon_storage_ptr->box_ptrs[box_index] = (struct pksav_gen2_pokemon_box*)(
                                                       &file_buffer[offset]
                                                   );
    }

    pokemon_storage_ptr->box_names_ptr = (struct pksav_gen2_pokemon_box_names*)(
                                             &file_buffer[offsets[PKSAV_GEN2_PC_BOX_NAMES]]
                                         );
    pokemon_storage_ptr->current_box_num_ptr = &file_buffer[
                                                   offsets[PKSAV_GEN2_CURRENT_BOX_NUM]
                                               ];
    pokemon_storage_ptr->current_box_ptr = (struct pksav_gen2_pokemon_box*)(
                                               &file_buffer[offsets[PKSAV_GEN2_CURRENT_BOX]]
                                           );

    // Pokédex lists
    struct pksav_gen2_pokedex_lists* pokedex_lists_ptr = &gen2_save_ptr->pokedex_lists;

    pokedex_lists_ptr->seen_ptr  = &file_buffer[PKSAV_GEN2_POKEDEX_SEEN];
    pokedex_lists_ptr->owned_ptr = &file_buffer[PKSAV_GEN2_POKEDEX_OWNED];

    // Trainer info
    struct pksav_gen2_trainer_info* trainer_info_ptr = &gen2_save_ptr->trainer_info;

    trainer_info_ptr->id_ptr           = (uint16_t*)&file_buffer[offsets[PKSAV_GEN2_PLAYER_ID]];
    trainer_info_ptr->name_ptr         = &file_buffer[offsets[PKSAV_GEN2_PLAYER_NAME]];
    if(gen2_save_ptr->save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
    {
        trainer_info_ptr->gender_ptr = &file_buffer[offsets[PKSAV_GEN2_PLAYER_GENDER]];
    }
    else
    {
        trainer_info_ptr->gender_ptr = NULL;
    }
    trainer_info_ptr->palette_ptr      = &file_buffer[offsets[PKSAV_GEN2_PLAYER_PALETTE]];
    trainer_info_ptr->money_ptr        = &file_buffer[offsets[PKSAV_GEN2_MONEY]];
    trainer_info_ptr->johto_badges_ptr = &file_buffer[offsets[PKSAV_GEN2_JOHTO_BADGES]];
    trainer_info_ptr->kanto_badges_ptr = &file_buffer[offsets[PKSAV_GEN2_KANTO_BADGES]];

    // Misc
    struct pksav_gen2_misc_fields* misc_fields_ptr = &gen2_save_ptr->misc_fields;

    misc_fields_ptr->rival_name_ptr = &file_buffer[offsets[PKSAV_GEN2_RIVAL_NAME]];
}

enum pksav_error pksav_gen2_load_save(
    const char* filepath,
    struct pksav_gen2_save* gen2_save_out
)
{
    if(!filepath || !gen2_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                filepath,
                &file_buffer,
                &buffer_len
            );

    if(!error)
    {
        assert(file_buffer != NULL);

        enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
        error = pksav_gen2_get_buffer_save_type(
                    file_buffer,
                    buffer_len,
                    &save_type
                );
        if(!error && (save_type != PKSAV_GEN2_SAVE_TYPE_NONE))
        {
            gen2_save_out->save_type = save_type;
            _pksav_gen2_set_save_pointers(
                gen2_save_out,
                file_buffer
            );
        }
        else
        {
            free(file_buffer);
        }
    }

    return error;
}

enum pksav_error pksav_gen2_save_save(
    const char* filepath,
    struct pksav_gen2_save* gen2_save_ptr
)
{
    if(!filepath || !gen2_save_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    struct pksav_gen2_save_internal* internal_ptr = gen2_save_ptr->internal_ptr;
    _pksav_gen2_get_save_checksums(
        gen2_save_ptr->save_type,
        internal_ptr->raw_save_ptr,
        internal_ptr->checksum1_ptr,
        internal_ptr->checksum2_ptr
    );

    error = pksav_fs_write_buffer_to_file(
                filepath,
                internal_ptr->raw_save_ptr,
                PKSAV_GEN2_SAVE_SIZE
            );

    return error;
}

enum pksav_error pksav_gen2_free_save(
    struct pksav_gen2_save* gen2_save_ptr
)
{
    if(!gen2_save_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    struct pksav_gen2_save_internal* internal_ptr = gen2_save_ptr->internal_ptr;
    free(internal_ptr->raw_save_ptr);
    free(internal_ptr);

    // Everything else is a pointer or an enum with a default value of 0,
    // so this one memset should be fine.
    memset(
        gen2_save_ptr,
        0,
        sizeof(*gen2_save_ptr)
    );

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen2_pokemon_storage_set_current_box(
    struct pksav_gen2_pokemon_storage* gen2_pokemon_storage_ptr,
    uint8_t new_current_box_num
)
{
    if(!gen2_pokemon_storage_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(new_current_box_num >= PKSAV_GEN2_NUM_POKEMON_BOXES)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    uint8_t* current_box_num_ptr = gen2_pokemon_storage_ptr->current_box_num_ptr;
    struct pksav_gen2_pokemon_box* current_box_ptr = gen2_pokemon_storage_ptr->current_box_ptr;
    struct pksav_gen2_pokemon_box** box_ptrs = gen2_pokemon_storage_ptr->box_ptrs;

    *box_ptrs[*current_box_num_ptr] = *current_box_ptr;
    *current_box_num_ptr = new_current_box_num;
    *current_box_ptr = *box_ptrs[new_current_box_num];

    return PKSAV_ERROR_NONE;
}
