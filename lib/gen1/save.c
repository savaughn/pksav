/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util/fs.h"

#include <pksav/gen1/save.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Offsets in a Generation I save
enum pksav_gen1_save_offset
{
    PKSAV_GEN1_TRAINER_NAME            = 0x2598,
    PKSAV_GEN1_POKEDEX_OWNED           = 0x25A3,
    PKSAV_GEN1_POKEDEX_SEEN            = 0x25B6,
    PKSAV_GEN1_ITEM_BAG                = 0x25C9,
    PKSAV_GEN1_MONEY                   = 0x25F3,
    PKSAV_GEN1_RIVAL_NAME              = 0x25F6,
    PKSAV_GEN1_OPTIONS                 = 0x2601,
    PKSAV_GEN1_BADGES                  = 0x2602,
    PKSAV_GEN1_TRAINER_ID              = 0x2605,
    PKSAV_GEN1_PIKACHU_FRIENDSHIP      = 0x271C,
    PKSAV_GEN1_ITEM_PC                 = 0x27E6,
    PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM = 0x284C,
    PKSAV_GEN1_CASINO_COINS            = 0x2850,
    PKSAV_GEN1_TIME_PLAYED             = 0x2CED,
    PKSAV_GEN1_POKEMON_PARTY           = 0x2F2C,
    PKSAV_GEN1_CURRENT_POKEMON_BOX     = 0x30C0,
    PKSAV_GEN1_CHECKSUM                = 0x3523,
    PKSAV_GEN1_POKEMON_PC_FIRST_HALF   = 0x4000,
    PKSAV_GEN1_POKEMON_PC_SECOND_HALF  = 0x6000
};

static uint8_t _pksav_gen1_get_save_checksum(
    const uint8_t* buffer
)
{
    assert(buffer != NULL);

    uint8_t checksum = 255;
    for(size_t buffer_index = 0x2598;
        buffer_index < PKSAV_GEN1_CHECKSUM;
        ++buffer_index)
    {
        checksum -= buffer[buffer_index];
    }

    return checksum;
}

enum pksav_error pksav_gen1_is_buffer_valid_save(
    const uint8_t* buffer,
    size_t buffer_len,
    bool* result_out
)
{
    if(!buffer || !buffer_len || !result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    if(buffer_len < PKSAV_GEN1_SAVE_SIZE)
    {
        *result_out = false;
    }
    else
    {
        uint8_t buffer_checksum = buffer[PKSAV_GEN1_CHECKSUM];
        uint8_t calculated_checksum = _pksav_gen1_get_save_checksum(buffer);
        *result_out = (buffer_checksum == calculated_checksum);
    }

    return error;
}

enum pksav_error pksav_gen1_is_file_valid_save(
    const char* filepath,
    bool* result_out
)
{
    if(!filepath || !result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;
    bool is_valid_save = false;

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

        error = pksav_gen1_is_buffer_valid_save(
                    file_buffer,
                    buffer_len,
                    &is_valid_save
                );
        free(file_buffer);

        // Only return a result upon success.
        if(!error)
        {
            *result_out = is_valid_save;
        }
    }

    return error;
}

static void _pksav_gen1_set_save_pointers(
    struct pksav_gen1_save* gen1_save_ptr
)
{
    assert(gen1_save_ptr != NULL);
    assert(gen1_save_ptr->_internal.raw_save_ptr = NULL);

    uint8_t* raw_save_ptr = gen1_save_ptr->_internal.raw_save_ptr;

    // Item storage
    struct pksav_gen1_item_storage* item_storage_ptr = &gen1_save_ptr->item_storage;

    item_storage_ptr->item_bag_ptr = (struct pksav_gen1_item_bag*)(
                                         &raw_save_ptr[PKSAV_GEN1_ITEM_BAG]
                                     );
    item_storage_ptr->item_pc_ptr = (struct pksav_gen1_item_pc*)(
                                        &raw_save_ptr[PKSAV_GEN1_ITEM_PC]
                                    );

    // Pokémon PC
    struct pksav_gen1_pokemon_pc* pokemon_pc_ptr = &gen1_save_ptr->pokemon_pc;

    for(size_t box_index = 0; box_index < 6; ++box_index)
    {
        size_t offset = PKSAV_GEN1_POKEMON_PC_FIRST_HALF +
                        (sizeof(struct pksav_gen1_pokemon_box) * box_index);

        pokemon_pc_ptr->pokemon_box_ptrs[box_index] = (struct pksav_gen1_pokemon_box*)(
                                                          &raw_save_ptr[offset]
                                                      );
    }
    for(size_t box_index = 6; box_index < 12; ++box_index)
    {
        size_t offset = PKSAV_GEN1_POKEMON_PC_SECOND_HALF +
                        (sizeof(struct pksav_gen1_pokemon_box) * (box_index - 6));

        pokemon_pc_ptr->pokemon_box_ptrs[box_index] = (struct pksav_gen1_pokemon_box*)(
                                                          &raw_save_ptr[offset]
                                                      );
    }

    pokemon_pc_ptr->current_pokemon_box_num_ptr = &raw_save_ptr[PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM];

    // Pokédex lists
    struct pksav_gen1_pokedex_lists* pokedex_lists_ptr = &gen1_save_ptr->pokedex_lists;

    pokedex_lists_ptr->seen_ptr  = &raw_save_ptr[PKSAV_GEN1_POKEDEX_SEEN];
    pokedex_lists_ptr->owned_ptr = &raw_save_ptr[PKSAV_GEN1_POKEDEX_OWNED];

    // Trainer info
    struct pksav_gen1_trainer_info* trainer_info_ptr = &gen1_save_ptr->trainer_info;

    trainer_info_ptr->trainer_id_ptr   = (uint16_t*)&raw_save_ptr[PKSAV_GEN1_TRAINER_ID];
    trainer_info_ptr->trainer_name_ptr = &raw_save_ptr[PKSAV_GEN1_TRAINER_NAME];
    trainer_info_ptr->money_ptr        = &raw_save_ptr[PKSAV_GEN1_MONEY];
    trainer_info_ptr->badges_ptr       = &raw_save_ptr[PKSAV_GEN1_BADGES];

    // Misc
    struct pksav_gen1_misc_fields* misc_fields_ptr = &gen1_save_ptr->misc_fields;

    misc_fields_ptr->rival_name_ptr   = &raw_save_ptr[PKSAV_GEN1_RIVAL_NAME];
    misc_fields_ptr->casino_coins_ptr = &raw_save_ptr[PKSAV_GEN1_CASINO_COINS];

    if(gen1_save_ptr->save_type == PKSAV_GEN1_SAVE_TYPE_YELLOW)
    {
        misc_fields_ptr->pikachu_friendship_ptr = &raw_save_ptr[PKSAV_GEN1_PIKACHU_FRIENDSHIP];
    }
    else
    {
        misc_fields_ptr->pikachu_friendship_ptr = NULL;
    }

    gen1_save_ptr->time_played_ptr = (struct pksav_gen1_time*)(
                                         &raw_save_ptr[PKSAV_GEN1_TIME_PLAYED]
                                     );
    gen1_save_ptr->options_ptr = &raw_save_ptr[PKSAV_GEN1_OPTIONS];

    // Internal
    struct pksav_gen1_save_internal* internal_ptr = &gen1_save_ptr->_internal;

    internal_ptr->checksum_ptr = &raw_save_ptr[PKSAV_GEN1_CHECKSUM];
}

enum pksav_error pksav_gen1_load_save(
    const char* filepath,
    struct pksav_gen1_save* gen1_save_out
)
{
    if(!filepath || !gen1_save_out)
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

        bool is_valid_save = false;
        error = pksav_gen1_is_buffer_valid_save(
                    file_buffer,
                    buffer_len,
                    &is_valid_save
                );
        if(!error)
        {
            gen1_save_out->_internal.raw_save_ptr = file_buffer;

            /*
             * Check if this save is for the Yellow version. The only way to
             * check this is to check the Pikachu Friendship field, which
             * isn't used in Red/Blue. This is usually fine but will fail if
             * the trainer's Pikachu despises the trainer enough to have a
             * friendship value of 0, which is unlikely but technically
             * possible.
             */
            if(file_buffer[PKSAV_GEN1_PIKACHU_FRIENDSHIP] > 0)
            {
                gen1_save_out->save_type = PKSAV_GEN1_SAVE_TYPE_YELLOW;
            }
            else
            {
                gen1_save_out->save_type = PKSAV_GEN1_SAVE_TYPE_RED_BLUE;
            }

            _pksav_gen1_set_save_pointers(gen1_save_out);
        }
    }

    return error;
}

enum pksav_error pksav_gen1_save_save(
    const char* filepath,
    struct pksav_gen1_save* gen1_save_ptr
)
{
    if(!filepath || !gen1_save_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    struct pksav_gen1_save_internal* internal_ptr = &gen1_save_ptr->_internal;

    *internal_ptr->checksum_ptr = _pksav_gen1_get_save_checksum(
                                       internal_ptr->raw_save_ptr
                                  );

    error = pksav_fs_write_buffer_to_file(
                filepath,
                internal_ptr->raw_save_ptr,
                PKSAV_GEN1_SAVE_SIZE
            );

    return error;
}

enum pksav_error pksav_gen1_free_save(
    struct pksav_gen1_save* gen1_save_ptr
)
{
    if(!gen1_save_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    free(gen1_save_ptr->_internal.raw_save_ptr);

    // Everything else is a pointer or an enum with a default value of 0,
    // so this one memset should be fine.
    memset(
        gen1_save_ptr,
        0,
        sizeof(*gen1_save_ptr)
    );

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen1_pokemon_pc_set_current_box(
    struct pksav_gen1_pokemon_pc* gen1_pokemon_pc_ptr,
    uint8_t new_current_box_num
)
{
    if(!gen1_pokemon_pc_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(new_current_box_num >= PKSAV_GEN1_NUM_POKEMON_BOXES)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    uint8_t* current_pokemon_box_num_ptr = gen1_pokemon_pc_ptr->current_pokemon_box_num_ptr;
    struct pksav_gen1_pokemon_box* current_pokemon_box_ptr = gen1_pokemon_pc_ptr->current_pokemon_box_ptr;
    struct pksav_gen1_pokemon_box** pokemon_box_ptrs = gen1_pokemon_pc_ptr->pokemon_box_ptrs;

    *pokemon_box_ptrs[*current_pokemon_box_num_ptr] = *current_pokemon_box_ptr;
    *current_pokemon_box_num_ptr = (new_current_box_num+1);
    *current_pokemon_box_ptr = *pokemon_box_ptrs[*current_pokemon_box_num_ptr];

    return PKSAV_ERROR_NONE;
}
