/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "gen1/save_internal.h"
#include "util/fs.h"

#include <pksav/gen1/save.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum pksav_error pksav_gen1_get_buffer_save_type(
    const uint8_t* p_buffer,
    size_t buffer_len,
    enum pksav_gen1_save_type* p_save_type_out
)
{
    if(!p_buffer || !p_save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    *p_save_type_out = PKSAV_GEN1_SAVE_TYPE_NONE;

    if(buffer_len >= PKSAV_GEN1_SAVE_SIZE)
    {
        uint8_t buffer_checksum = p_buffer[PKSAV_GEN1_CHECKSUM];
        uint8_t calculated_checksum = pksav_gen1_get_save_checksum(p_buffer);
        if(buffer_checksum == calculated_checksum)
        {
            /*
             * Check if this save is for the Yellow version. The only way to
             * check this is to check the Pikachu Friendship field, which
             * isn't used in Red/Blue. This is usually fine but will fail if
             * the trainer's Pikachu despises the trainer enough to have a
             * friendship value of 0, which is unlikely but technically
             * possible.
             */
            if(p_buffer[PKSAV_GEN1_PIKACHU_FRIENDSHIP] > 0)
            {
                *p_save_type_out = PKSAV_GEN1_SAVE_TYPE_YELLOW;
            }
            else
            {
                *p_save_type_out = PKSAV_GEN1_SAVE_TYPE_RED_BLUE;
            }
        }
    }

    return error;
}

enum pksav_error pksav_gen1_get_file_save_type(
    const char* p_filepath,
    enum pksav_gen1_save_type* p_save_type_out
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

        enum pksav_gen1_save_type save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
        error = pksav_gen1_get_buffer_save_type(
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

static void _pksav_gen1_set_save_pointers(
    struct pksav_gen1_save* p_gen1_save,
    uint8_t* p_file_buffer
)
{
    assert(p_gen1_save != NULL);
    assert(p_file_buffer != NULL);

    // Item storage
    struct pksav_gen1_item_storage* p_item_storage = &p_gen1_save->item_storage;

    p_item_storage->p_item_bag = (struct pksav_gen1_item_bag*)(
                                         &p_file_buffer[PKSAV_GEN1_ITEM_BAG]
                                     );
    p_item_storage->p_item_pc = (struct pksav_gen1_item_pc*)(
                                        &p_file_buffer[PKSAV_GEN1_ITEM_PC]
                                    );

    // Pokémon storage
    struct pksav_gen1_pokemon_storage* p_pokemon_storage = &p_gen1_save->pokemon_storage;

    p_pokemon_storage->p_party = (struct pksav_gen1_pokemon_party*)(
                                         &p_file_buffer[PKSAV_GEN1_POKEMON_PARTY]
                                     );

    for(size_t box_index = 0; box_index < 6; ++box_index)
    {
        size_t offset = PKSAV_GEN1_POKEMON_PC_FIRST_HALF +
                        (sizeof(struct pksav_gen1_pokemon_box) * box_index);

        p_pokemon_storage->pp_boxes[box_index] = (struct pksav_gen1_pokemon_box*)(
                                                               &p_file_buffer[offset]
                                                           );
    }
    for(size_t box_index = 6; box_index < 12; ++box_index)
    {
        size_t offset = PKSAV_GEN1_POKEMON_PC_SECOND_HALF +
                        (sizeof(struct pksav_gen1_pokemon_box) * (box_index - 6));

        p_pokemon_storage->pp_boxes[box_index] = (struct pksav_gen1_pokemon_box*)(
                                                               &p_file_buffer[offset]
                                                           );
    }

    p_pokemon_storage->p_current_box_num = &p_file_buffer[PKSAV_GEN1_CURRENT_BOX_NUM];
    p_pokemon_storage->p_current_box = (struct pksav_gen1_pokemon_box*)(
                                                       &p_file_buffer[PKSAV_GEN1_CURRENT_BOX]
                                                   );

    // Pokédex lists
    struct pksav_gen1_pokedex_lists* p_pokedex_lists = &p_gen1_save->pokedex_lists;

    p_pokedex_lists->p_seen  = &p_file_buffer[PKSAV_GEN1_POKEDEX_SEEN];
    p_pokedex_lists->p_owned = &p_file_buffer[PKSAV_GEN1_POKEDEX_OWNED];

    // Trainer info
    struct pksav_gen1_trainer_info* p_trainer_info = &p_gen1_save->trainer_info;

    p_trainer_info->p_id     = (uint16_t*)&p_file_buffer[PKSAV_GEN1_PLAYER_ID];
    p_trainer_info->p_name   = &p_file_buffer[PKSAV_GEN1_PLAYER_NAME];
    p_trainer_info->p_money  = &p_file_buffer[PKSAV_GEN1_MONEY];
    p_trainer_info->p_badges = &p_file_buffer[PKSAV_GEN1_BADGES];

    // Misc
    struct pksav_gen1_misc_fields* p_misc_fields = &p_gen1_save->misc_fields;

    p_misc_fields->p_rival_name   = &p_file_buffer[PKSAV_GEN1_RIVAL_NAME];
    p_misc_fields->p_casino_coins = &p_file_buffer[PKSAV_GEN1_CASINO_COINS];

    if(p_gen1_save->save_type == PKSAV_GEN1_SAVE_TYPE_YELLOW)
    {
        p_misc_fields->p_pikachu_friendship = &p_file_buffer[PKSAV_GEN1_PIKACHU_FRIENDSHIP];
    }
    else
    {
        p_misc_fields->p_pikachu_friendship = NULL;
    }

    p_gen1_save->p_time_played = (struct pksav_gen1_time*)(
                                         &p_file_buffer[PKSAV_GEN1_TIME_PLAYED]
                                     );
    p_gen1_save->p_options = &p_file_buffer[PKSAV_GEN1_OPTIONS];

    // Internal
    p_gen1_save->p_internal = calloc(sizeof(struct pksav_gen1_save_internal), 1);
    struct pksav_gen1_save_internal* p_internal = p_gen1_save->p_internal;
    p_internal->p_raw_save = p_file_buffer;
    p_internal->p_checksum = &p_file_buffer[PKSAV_GEN1_CHECKSUM];
}

static enum pksav_error _pksav_gen1_load_save_from_buffer(
    uint8_t* buffer,
    size_t buffer_len,
    bool is_buffer_ours,
    struct pksav_gen1_save* gen1_save_out
)
{
    assert(gen1_save_out != NULL);
    assert(buffer != NULL);

    enum pksav_error error = PKSAV_ERROR_NONE;

    enum pksav_gen1_save_type save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
    error = pksav_gen1_get_buffer_save_type(
                buffer,
                buffer_len,
                &save_type
            );
    if(!error && (save_type != PKSAV_GEN1_SAVE_TYPE_NONE))
    {
        if(save_type != PKSAV_GEN1_SAVE_TYPE_NONE)
        {
            gen1_save_out->save_type = save_type;
            _pksav_gen1_set_save_pointers(
                gen1_save_out,
                buffer
            );

            // Internal
            struct pksav_gen1_save_internal* p_internal = gen1_save_out->p_internal;
            p_internal->is_buffer_ours = is_buffer_ours;
        }
        else
        {
            error = PKSAV_ERROR_INVALID_SAVE;
        }
    }

    return error;
}

enum pksav_error pksav_gen1_load_save_from_buffer(
    uint8_t* buffer,
    size_t buffer_len,
    struct pksav_gen1_save* gen1_save_out
)
{
    if(!buffer || !gen1_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    return _pksav_gen1_load_save_from_buffer(
               buffer,
               buffer_len,
               false, // is_buffer_ours
               gen1_save_out
           );
}

enum pksav_error pksav_gen1_load_save_from_file(
    const char* filepath,
    struct pksav_gen1_save* gen1_save_out
)
{
    if(!filepath || !gen1_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                filepath,
                &buffer,
                &buffer_len
            );

    if(!error)
    {
        error = _pksav_gen1_load_save_from_buffer(
                    buffer,
                    buffer_len,
                    true, // is_buffer_ours
                    gen1_save_out
                );
        if(error)
        {
            // We made this buffer, so it's on us to free it if there's
            // an error.
            free(buffer);
        }
    }

    return error;
}

enum pksav_error pksav_gen1_save_save(
    const char* filepath,
    struct pksav_gen1_save* p_gen1_save
)
{
    if(!filepath || !p_gen1_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    struct pksav_gen1_save_internal* p_internal = p_gen1_save->p_internal;

    *p_internal->p_checksum = pksav_gen1_get_save_checksum(
                                       p_internal->p_raw_save
                                  );

    error = pksav_fs_write_buffer_to_file(
                filepath,
                p_internal->p_raw_save,
                PKSAV_GEN1_SAVE_SIZE
            );

    return error;
}

enum pksav_error pksav_gen1_free_save(
    struct pksav_gen1_save* p_gen1_save
)
{
    if(!p_gen1_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    struct pksav_gen1_save_internal* p_internal = p_gen1_save->p_internal;
    if(p_internal->is_buffer_ours)
    {
        free(p_internal->p_raw_save);
    }
    free(p_internal);

    // Everything else is a pointer or an enum with a default value of 0,
    // so this one memset should be fine.
    memset(p_gen1_save, 0, sizeof(*p_gen1_save));

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen1_pokemon_storage_set_current_box(
    struct pksav_gen1_pokemon_storage* p_gen1_pokemon_storage,
    uint8_t new_current_box_num
)
{
    if(!p_gen1_pokemon_storage)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(new_current_box_num >= PKSAV_GEN1_NUM_POKEMON_BOXES)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    uint8_t* p_current_box_num = p_gen1_pokemon_storage->p_current_box_num;
    struct pksav_gen1_pokemon_box* p_current_box = p_gen1_pokemon_storage->p_current_box;
    struct pksav_gen1_pokemon_box** pp_boxes = p_gen1_pokemon_storage->pp_boxes;

    uint8_t current_box_num = *p_current_box_num
                            & PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK;

    *pp_boxes[current_box_num] = *p_current_box;

    *p_current_box_num &= ~PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK;
    *p_current_box_num |= new_current_box_num;

    *p_current_box = *pp_boxes[new_current_box_num];

    return PKSAV_ERROR_NONE;
}
