/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "gen2/save_internal.h"
#include "util/fs.h"

#include <pksav/gen2/save.h>

#include <pksav/math/endian.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

void pksav_gen2_get_save_checksums(
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
            pksav_gen2_get_save_checksums(
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
    uint8_t* buffer
)
{
    assert(gen2_save_ptr != NULL);
    assert(buffer != NULL);

    const size_t* offsets = NULL;

    // Internal
    gen2_save_ptr->internal_ptr = calloc(sizeof(struct pksav_gen2_save_internal), 1);
    struct pksav_gen2_save_internal* internal_ptr = gen2_save_ptr->internal_ptr;
    internal_ptr->raw_save_ptr = buffer;
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

    internal_ptr->checksum1_ptr = (uint16_t*)&buffer[
                                                 offsets[PKSAV_GEN2_CHECKSUM1]
                                             ];

    internal_ptr->checksum2_ptr = (uint16_t*)&buffer[
                                                 offsets[PKSAV_GEN2_CHECKSUM2]
                                             ];

    // Options
    struct pksav_gen2_options* options_ptr = &gen2_save_ptr->options;

    options_ptr->misc_options_ptr = &buffer[offsets[PKSAV_GEN2_OPTIONS]];
    options_ptr->textbox_frame_index_ptr = &buffer[offsets[PKSAV_GEN2_TEXTBOX_FRAME_INDEX]];
    options_ptr->gbprinter_brightness_ptr = &buffer[offsets[PKSAV_GEN2_GBPRINTER_BRIGHTNESS]];
    options_ptr->menu_account_ptr = &buffer[offsets[PKSAV_GEN2_MENU_ACCOUNT]];

    // Save time
    struct pksav_gen2_save_time* save_time_ptr = &gen2_save_ptr->save_time;

    save_time_ptr->time_played_ptr = (struct pksav_gen2_time*)(
                                         &buffer[offsets[PKSAV_GEN2_TIME_PLAYED]]
                                     );
    save_time_ptr->daylight_savings_ptr = &buffer[offsets[PKSAV_GEN2_DAYLIGHT_SAVINGS]];

    // Item storage
    struct pksav_gen2_item_storage* item_storage_ptr = &gen2_save_ptr->item_storage;

    item_storage_ptr->item_bag_ptr = (struct pksav_gen2_item_bag*)(
                                         &buffer[offsets[PKSAV_GEN2_ITEM_BAG]]
                                     );
    item_storage_ptr->item_pc_ptr = (struct pksav_gen2_item_pc*)(
                                        &buffer[offsets[PKSAV_GEN2_ITEM_PC]]
                                    );

    // Pokémon storage
    struct pksav_gen2_pokemon_storage* pokemon_storage_ptr = &gen2_save_ptr->pokemon_storage;

    pokemon_storage_ptr->party_ptr = (struct pksav_gen2_pokemon_party*)(
                                         &buffer[offsets[PKSAV_GEN2_POKEMON_PARTY]]
                                     );

    for(size_t box_index = 0; box_index < 7; ++box_index)
    {
        size_t offset = offsets[PKSAV_GEN2_POKEMON_PC_FIRST_HALF] +
                        (sizeof(struct pksav_gen2_pokemon_box) * box_index);

        pokemon_storage_ptr->box_ptrs[box_index] = (struct pksav_gen2_pokemon_box*)(
                                                       &buffer[offset]
                                                   );
    }
    for(size_t box_index = 7; box_index < 14; ++box_index)
    {
        size_t offset = offsets[PKSAV_GEN2_POKEMON_PC_SECOND_HALF] +
                        (sizeof(struct pksav_gen2_pokemon_box) * (box_index - 7));

        pokemon_storage_ptr->box_ptrs[box_index] = (struct pksav_gen2_pokemon_box*)(
                                                       &buffer[offset]
                                                   );
    }

    pokemon_storage_ptr->box_names_ptr = (struct pksav_gen2_pokemon_box_names*)(
                                             &buffer[offsets[PKSAV_GEN2_PC_BOX_NAMES]]
                                         );
    pokemon_storage_ptr->current_box_num_ptr = &buffer[
                                                   offsets[PKSAV_GEN2_CURRENT_BOX_NUM]
                                               ];
    pokemon_storage_ptr->current_box_ptr = (struct pksav_gen2_pokemon_box*)(
                                               &buffer[offsets[PKSAV_GEN2_CURRENT_BOX]]
                                           );

    // Pokédex lists
    struct pksav_gen2_pokedex_lists* pokedex_lists_ptr = &gen2_save_ptr->pokedex_lists;

    pokedex_lists_ptr->seen_ptr  = &buffer[offsets[PKSAV_GEN2_POKEDEX_SEEN]];
    pokedex_lists_ptr->owned_ptr = &buffer[offsets[PKSAV_GEN2_POKEDEX_OWNED]];

    // Trainer info
    struct pksav_gen2_trainer_info* trainer_info_ptr = &gen2_save_ptr->trainer_info;

    trainer_info_ptr->id_ptr           = (uint16_t*)&buffer[offsets[PKSAV_GEN2_PLAYER_ID]];
    trainer_info_ptr->name_ptr         = &buffer[offsets[PKSAV_GEN2_PLAYER_NAME]];
    if(gen2_save_ptr->save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
    {
        trainer_info_ptr->gender_ptr = &buffer[offsets[PKSAV_GEN2_PLAYER_GENDER]];
    }
    else
    {
        trainer_info_ptr->gender_ptr = NULL;
    }
    trainer_info_ptr->palette_ptr      = &buffer[offsets[PKSAV_GEN2_PLAYER_PALETTE]];
    trainer_info_ptr->money_ptr        = &buffer[offsets[PKSAV_GEN2_MONEY]];
    trainer_info_ptr->johto_badges_ptr = &buffer[offsets[PKSAV_GEN2_JOHTO_BADGES]];
    trainer_info_ptr->kanto_badges_ptr = &buffer[offsets[PKSAV_GEN2_KANTO_BADGES]];

    // Misc
    struct pksav_gen2_misc_fields* misc_fields_ptr = &gen2_save_ptr->misc_fields;

    misc_fields_ptr->rival_name_ptr = &buffer[offsets[PKSAV_GEN2_RIVAL_NAME]];
}

static enum pksav_error _pksav_gen2_load_save_from_buffer(
    uint8_t* buffer,
    size_t buffer_len,
    bool is_buffer_ours,
    struct pksav_gen2_save* gen2_save_out
)
{
    assert(gen2_save_out != NULL);
    assert(buffer != NULL);

    enum pksav_error error = PKSAV_ERROR_NONE;

    enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
    error = pksav_gen2_get_buffer_save_type(
                buffer,
                buffer_len,
                &save_type
            );
    if(!error && (save_type != PKSAV_GEN2_SAVE_TYPE_NONE))
    {
        if(save_type != PKSAV_GEN2_SAVE_TYPE_NONE)
        {
            gen2_save_out->save_type = save_type;
            _pksav_gen2_set_save_pointers(
                gen2_save_out,
                buffer
            );

            // Internal
            struct pksav_gen2_save_internal* internal_ptr = gen2_save_out->internal_ptr;
            internal_ptr->is_buffer_ours = is_buffer_ours;
        }
        else
        {
            error = PKSAV_ERROR_INVALID_SAVE;
        }
    }

    return error;
}

enum pksav_error pksav_gen2_load_save_from_buffer(
    uint8_t* buffer,
    size_t buffer_len,
    struct pksav_gen2_save* gen2_save_out
)
{
    if(!buffer || !gen2_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    return _pksav_gen2_load_save_from_buffer(
               buffer,
               buffer_len,
               false, // is_buffer_ours
               gen2_save_out
           );
}

enum pksav_error pksav_gen2_load_save_from_file(
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
        error = _pksav_gen2_load_save_from_buffer(
                    file_buffer,
                    buffer_len,
                    true, // is_buffer_ours
                    gen2_save_out
                );
        if(error)
        {
            // We made this buffer, so it's on us to free it if there's
            // an error.
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
    pksav_gen2_get_save_checksums(
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
    if(internal_ptr->is_buffer_ours)
    {
        free(internal_ptr->raw_save_ptr);
    }
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
