/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "gen2/save_internal.h"
#include "util/fs.h"

#include <pksav/gen2/common.h>
#include <pksav/gen2/save.h>

#include <pksav/math/endian.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

void pksav_gen2_get_save_checksums(
    enum pksav_gen2_save_type save_type,
    const uint8_t* p_buffer,
    uint16_t* p_checksum1_out,
    uint16_t* p_checksum2_out
)
{
    assert(p_buffer != NULL);
    assert(p_checksum1_out != NULL);
    assert(p_checksum2_out != NULL);

    *p_checksum1_out = 0;
    *p_checksum2_out = 0;

    switch(save_type)
    {
        case PKSAV_GEN2_SAVE_TYPE_GS:
            for(size_t buffer_index = 0x2009; buffer_index <= 0x2D68; ++buffer_index)
            {
                *p_checksum1_out += p_buffer[buffer_index];
            }

            for(size_t buffer_index = 0x0C6B; buffer_index <= 0x17EC; ++buffer_index)
            {
                *p_checksum2_out += p_buffer[buffer_index];
            }
            for(size_t buffer_index = 0x3D96; buffer_index <= 0x3F3F; ++buffer_index)
            {
                *p_checksum2_out += p_buffer[buffer_index];
            }
            for(size_t buffer_index = 0x7E39; buffer_index <= 0x7E6C; ++buffer_index)
            {
                *p_checksum2_out += p_buffer[buffer_index];
            }
            break;

        case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
            for(size_t buffer_index = 0x2009; buffer_index <= 0x2B82; ++buffer_index)
            {
                *p_checksum1_out += p_buffer[buffer_index];
            }

            for(size_t buffer_index = 0x1209; buffer_index <= 0x1D82; ++buffer_index)
            {
                *p_checksum2_out += p_buffer[buffer_index];
            }
            break;

        default:
            assert(false);
    }

    *p_checksum1_out = pksav_littleendian16(*p_checksum1_out);
    *p_checksum2_out = pksav_littleendian16(*p_checksum2_out);
}

enum pksav_error pksav_gen2_get_buffer_save_type(
    const uint8_t* p_buffer,
    size_t buffer_len,
    enum pksav_gen2_save_type* p_save_type_out
)
{
    if(!p_buffer || !p_save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    *p_save_type_out = PKSAV_GEN2_SAVE_TYPE_NONE;
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
                p_buffer,
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

            const uint16_t* p_checksum1 = (const uint16_t*)&p_buffer[checksum1_index];
            const uint16_t* p_checksum2 = (const uint16_t*)&p_buffer[checksum2_index];

            uint16_t checksum1 = pksav_littleendian16(*p_checksum1);
            uint16_t checksum2 = pksav_littleendian16(*p_checksum2);

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
                *p_save_type_out = save_type;
            }
            else
            {
                error = PKSAV_ERROR_INVALID_SAVE;
            }
        }
    }

    return error;
}

enum pksav_error pksav_gen2_get_file_save_type(
    const char* p_filepath,
    enum pksav_gen2_save_type* p_save_type_out
)
{
    if(!p_filepath || !p_save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* p_file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                p_filepath,
                &p_file_buffer,
                &buffer_len
            );
    if(!error)
    {
        assert(p_file_buffer != NULL);

        enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
        error = pksav_gen2_get_buffer_save_type(
                    p_file_buffer,
                    buffer_len,
                    &save_type
                );
        free(p_file_buffer);

        // Only return a result upon success.
        if(!error)
        {
            *p_save_type_out = save_type;
        }
    }

    return error;
}

static void _pksav_gen2_set_save_pointers(
    struct pksav_gen2_save* p_gen2_save,
    uint8_t* p_buffer
)
{
    assert(p_gen2_save != NULL);
    assert(p_buffer != NULL);

    const size_t* p_offsets = NULL;

    // Internal
    p_gen2_save->p_internal = calloc(sizeof(struct pksav_gen2_save_internal), 1);
    struct pksav_gen2_save_internal* p_internal = p_gen2_save->p_internal;
    p_internal->p_raw_save = p_buffer;
    switch(p_gen2_save->save_type)
    {
        case PKSAV_GEN2_SAVE_TYPE_GS:
            p_offsets = GS_OFFSETS;
            break;

        case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
            p_offsets = CRYSTAL_OFFSETS;
            break;

        default:
            assert(false);
    }

    assert(p_offsets != NULL);

    p_internal->p_checksum1 = (uint16_t*)&p_buffer[
                                  p_offsets[PKSAV_GEN2_CHECKSUM1]
                              ];

    p_internal->p_checksum2 = (uint16_t*)&p_buffer[
                                  p_offsets[PKSAV_GEN2_CHECKSUM2]
                              ];
    // Mailbox
    p_gen2_save->p_mailbox = (struct pksav_gen2_mailbox *)(&p_buffer[p_offsets[PKSAV_GEN2_MAILBOX_DATA]]);

    // Options
    struct pksav_gen2_options* p_options = &p_gen2_save->options;

    p_options->p_misc_options = &p_buffer[p_offsets[PKSAV_GEN2_OPTIONS]];
    p_options->p_textbox_frame_index = &p_buffer[p_offsets[PKSAV_GEN2_TEXTBOX_FRAME_INDEX]];
    p_options->p_gbprinter_brightness = &p_buffer[p_offsets[PKSAV_GEN2_GBPRINTER_BRIGHTNESS]];
    p_options->p_menu_account = &p_buffer[p_offsets[PKSAV_GEN2_MENU_ACCOUNT]];

    // Save time
    struct pksav_gen2_save_time* p_save_time = &p_gen2_save->save_time;

    p_save_time->p_time_played = (struct pksav_gen2_time*)(
                                     &p_buffer[p_offsets[PKSAV_GEN2_TIME_PLAYED]]
                                 );
    p_save_time->p_daylight_savings = &p_buffer[p_offsets[PKSAV_GEN2_DAYLIGHT_SAVINGS]];

    // Item storage
    struct pksav_gen2_item_storage* p_item_storage = &p_gen2_save->item_storage;

    p_item_storage->p_item_bag = (struct pksav_gen2_item_bag*)(
                                     &p_buffer[p_offsets[PKSAV_GEN2_ITEM_BAG]]
                                 );
    p_item_storage->p_item_pc = (struct pksav_gen2_item_pc*)(
                                    &p_buffer[p_offsets[PKSAV_GEN2_ITEM_PC]]
                                );
    p_item_storage->p_registered_item_index = &p_buffer[p_offsets[PKSAV_GEN2_REGISTERED_ITEM]];
    p_item_storage->p_registered_item_info = &p_buffer[p_offsets[PKSAV_GEN2_REGISTERED_ITEM_INFO]];

    // Pokémon storage
    struct pksav_gen2_pokemon_storage* p_pokemon_storage = &p_gen2_save->pokemon_storage;

    p_pokemon_storage->p_party = (struct pksav_gen2_pokemon_party*)(
                                     &p_buffer[p_offsets[PKSAV_GEN2_POKEMON_PARTY]]
                                 );

    for(size_t box_index = 0; box_index < 7; ++box_index)
    {
        size_t offset = p_offsets[PKSAV_GEN2_POKEMON_PC_FIRST_HALF] +
                        (sizeof(struct pksav_gen2_pokemon_box) * box_index);

        p_pokemon_storage->pp_boxes[box_index] = (struct pksav_gen2_pokemon_box*)(
                                                     &p_buffer[offset]
                                                 );
    }
    for(size_t box_index = 7; box_index < 14; ++box_index)
    {
        size_t offset = p_offsets[PKSAV_GEN2_POKEMON_PC_SECOND_HALF] +
                        (sizeof(struct pksav_gen2_pokemon_box) * (box_index - 7));

        p_pokemon_storage->pp_boxes[box_index] = (struct pksav_gen2_pokemon_box*)(
                                                     &p_buffer[offset]
                                                 );
    }

    p_pokemon_storage->p_box_names = (struct pksav_gen2_pokemon_box_names*)(
                                         &p_buffer[p_offsets[PKSAV_GEN2_PC_BOX_NAMES]]
                                     );
    p_pokemon_storage->p_current_box_num = &p_buffer[
                                               p_offsets[PKSAV_GEN2_CURRENT_BOX_NUM]
                                           ];
    p_pokemon_storage->p_current_box = (struct pksav_gen2_pokemon_box*)(
                                           &p_buffer[p_offsets[PKSAV_GEN2_CURRENT_BOX]]
                                       );

    // Party mail
    p_pokemon_storage->p_party_mail = (struct pksav_gen2_party_mail*)(&p_buffer[p_offsets[PKSAV_GEN2_PARTY_MAIL_DATA]]);

    // Pokédex lists
    struct pksav_gen2_pokedex_lists* p_pokedex_lists = &p_gen2_save->pokedex_lists;

    p_pokedex_lists->p_seen  = &p_buffer[p_offsets[PKSAV_GEN2_POKEDEX_SEEN]];
    p_pokedex_lists->p_owned = &p_buffer[p_offsets[PKSAV_GEN2_POKEDEX_OWNED]];

    // Trainer info
    struct pksav_gen2_trainer_info* p_trainer_info = &p_gen2_save->trainer_info;

    p_trainer_info->p_id   = (uint16_t*)&p_buffer[p_offsets[PKSAV_GEN2_PLAYER_ID]];
    p_trainer_info->p_name = &p_buffer[p_offsets[PKSAV_GEN2_PLAYER_NAME]];
    if(p_gen2_save->save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
    {
        p_trainer_info->p_gender = &p_buffer[p_offsets[PKSAV_GEN2_PLAYER_GENDER]];
    }
    else
    {
        p_trainer_info->p_gender = NULL;
    }
    p_trainer_info->p_palette      = &p_buffer[p_offsets[PKSAV_GEN2_PLAYER_PALETTE]];
    p_trainer_info->p_money        = &p_buffer[p_offsets[PKSAV_GEN2_MONEY]];
    p_trainer_info->p_johto_badges = &p_buffer[p_offsets[PKSAV_GEN2_JOHTO_BADGES]];
    p_trainer_info->p_kanto_badges = &p_buffer[p_offsets[PKSAV_GEN2_KANTO_BADGES]];

    // Misc
    struct pksav_gen2_misc_fields* p_misc_fields = &p_gen2_save->misc_fields;

    p_misc_fields->p_rival_name = &p_buffer[p_offsets[PKSAV_GEN2_RIVAL_NAME]];
    p_misc_fields->p_money_with_mom = &p_buffer[p_offsets[PKSAV_GEN2_MONEY_WITH_MOM]];
    p_misc_fields->p_mom_money_policy = &p_buffer[p_offsets[PKSAV_GEN2_MOM_MONEY_POLICY]];
    p_misc_fields->p_casino_coins = &p_buffer[p_offsets[PKSAV_GEN2_CASINO_COINS]];
}

static enum pksav_error _pksav_gen2_load_save_from_buffer(
    uint8_t* p_buffer,
    size_t buffer_len,
    bool is_buffer_ours,
    struct pksav_gen2_save* p_gen2_save_out
)
{
    assert(p_gen2_save_out != NULL);
    assert(p_buffer != NULL);

    enum pksav_error error = PKSAV_ERROR_NONE;

    enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
    error = pksav_gen2_get_buffer_save_type(
                p_buffer,
                buffer_len,
                &save_type
            );
    if(!error && (save_type != PKSAV_GEN2_SAVE_TYPE_NONE))
    {
        if(save_type != PKSAV_GEN2_SAVE_TYPE_NONE)
        {
            p_gen2_save_out->save_type = save_type;
            _pksav_gen2_set_save_pointers(
                p_gen2_save_out,
                p_buffer
            );

            // Internal
            struct pksav_gen2_save_internal* p_internal = p_gen2_save_out->p_internal;
            p_internal->is_buffer_ours = is_buffer_ours;
        }
        else
        {
            error = PKSAV_ERROR_INVALID_SAVE;
        }
    }

    return error;
}

enum pksav_error pksav_gen2_load_save_from_buffer(
    uint8_t* p_buffer,
    size_t buffer_len,
    struct pksav_gen2_save* p_gen2_save_out
)
{
    if(!p_buffer || !p_gen2_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    return _pksav_gen2_load_save_from_buffer(
               p_buffer,
               buffer_len,
               false, // is_buffer_ours
               p_gen2_save_out
           );
}

enum pksav_error pksav_gen2_load_save_from_file(
    const char* p_filepath,
    struct pksav_gen2_save* p_gen2_save_out
)
{
    if(!p_filepath || !p_gen2_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* p_file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                p_filepath,
                &p_file_buffer,
                &buffer_len
            );

    if(!error)
    {
        error = _pksav_gen2_load_save_from_buffer(
                    p_file_buffer,
                    buffer_len,
                    true, // is_buffer_ours
                    p_gen2_save_out
                );
        if(error)
        {
            // We made this buffer, so it's on us to free it if there's
            // an error.
            free(p_file_buffer);
        }
    }

    return error;
}

enum pksav_error pksav_gen2_save_save(
    const char* p_filepath,
    struct pksav_gen2_save* p_gen2_save
)
{
    if(!p_filepath || !p_gen2_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    struct pksav_gen2_save_internal* p_internal = p_gen2_save->p_internal;
    pksav_gen2_get_save_checksums(
        p_gen2_save->save_type,
        p_internal->p_raw_save,
        p_internal->p_checksum1,
        p_internal->p_checksum2
    );

    error = pksav_fs_write_buffer_to_file(
                p_filepath,
                p_internal->p_raw_save,
                PKSAV_GEN2_SAVE_SIZE
            );

    return error;
}

enum pksav_error pksav_gen2_free_save(
    struct pksav_gen2_save* p_gen2_save
)
{
    if(!p_gen2_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    struct pksav_gen2_save_internal* p_internal = p_gen2_save->p_internal;
    if(p_internal->is_buffer_ours)
    {
        free(p_internal->p_raw_save);
    }
    free(p_internal);

    // Everything else is a pointer or an enum with a default value of 0,
    // so this one memset should be fine.
    memset(p_gen2_save, 0, sizeof(*p_gen2_save));

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen2_pokemon_storage_set_current_box(
    struct pksav_gen2_pokemon_storage* p_gen2_pokemon_storage,
    uint8_t new_current_box_num
)
{
    if(!p_gen2_pokemon_storage)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(new_current_box_num >= PKSAV_GEN2_NUM_POKEMON_BOXES)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    uint8_t* p_current_box_num = p_gen2_pokemon_storage->p_current_box_num;
    struct pksav_gen2_pokemon_box* p_current_box = p_gen2_pokemon_storage->p_current_box;
    struct pksav_gen2_pokemon_box** pp_boxes = p_gen2_pokemon_storage->pp_boxes;

    *pp_boxes[*p_current_box_num] = *p_current_box;
    *p_current_box_num = new_current_box_num;
    *p_current_box = *pp_boxes[new_current_box_num];

    return PKSAV_ERROR_NONE;
}
