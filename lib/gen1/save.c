/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen1/save.h>

#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define PKSAV_GEN1_SAVE_SIZE 0x8000

// Offsets in a Generation I save
enum pksav_gen1_save_offset {
    PKSAV_GEN1_PLAYER_NAME             = 0x2598,
    PKSAV_GEN1_POKEDEX_OWNED           = 0x25A3,
    PKSAV_GEN1_POKEDEX_SEEN            = 0x25B6,
    PKSAV_GEN1_ITEM_BAG                = 0x25C9,
    PKSAV_GEN1_MONEY                   = 0x25F3,
    PKSAV_GEN1_RIVAL_NAME              = 0x25F6,
    PKSAV_GEN1_OPTIONS                 = 0x2601,
    PKSAV_GEN1_BADGES                  = 0x2602,
    PKSAV_GEN1_PLAYER_ID               = 0x2605,
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

static uint8_t _pksav_get_gen1_save_checksum(
    const uint8_t* data
) {
    uint8_t checksum = 255;
    for(uint16_t i = 0x2598; i < PKSAV_GEN1_CHECKSUM; ++i) {
        checksum -= data[i];
    }

    return checksum;
}

pksav_error_t pksav_buffer_is_gen1_save(
    const uint8_t* data,
    size_t buffer_len,
    bool* result_out
) {
    if(!data || !result_out) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    if(buffer_len < PKSAV_GEN1_SAVE_SIZE) {
         *result_out = false;
         return PKSAV_ERROR_NONE;
    }

    uint8_t checksum = data[PKSAV_GEN1_CHECKSUM];
    uint8_t actual_checksum = _pksav_get_gen1_save_checksum(data);
    *result_out = (checksum == actual_checksum);

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_file_is_gen1_save(
    const char* filepath,
    bool* result_out
) {
    if(!filepath || !result_out) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    FILE* gen1_save = fopen(filepath, "rb");
    if(!gen1_save) {
        *result_out = false;
        return PKSAV_ERROR_NONE;
    }

    fseek(gen1_save, 0, SEEK_END);

    if(ftell(gen1_save) < PKSAV_GEN1_SAVE_SIZE) {
        fclose(gen1_save);
        *result_out = false;
        return PKSAV_ERROR_NONE;
    }

    uint8_t* gen1_save_data = calloc(PKSAV_GEN1_SAVE_SIZE, 1);
    fseek(gen1_save, 0, SEEK_SET);
    size_t num_read = fread((void*)gen1_save_data, 1, PKSAV_GEN1_SAVE_SIZE, gen1_save);
    fclose(gen1_save);

    bool ret = false;
    if(num_read == PKSAV_GEN1_SAVE_SIZE) {
        pksav_error_t status = pksav_buffer_is_gen1_save(
                                   gen1_save_data,
                                   PKSAV_GEN1_SAVE_SIZE,
                                   &ret
                               );
        if(status) {
            free(gen1_save_data);
            return status;
        }
    }

    free(gen1_save_data);
    *result_out = ret;
    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_save_load(
    const char* filepath,
    struct pksav_gen1_save* gen1_save
) {
    if(!filepath || !gen1_save) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Read the file and make sure it's valid
    /*FILE* gen1_save_file = fopen(filepath, "rb");
    if(!gen1_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gen1_save_file, 0, SEEK_END);

    if(ftell(gen1_save_file) < PKSAV_GEN1_SAVE_SIZE) {
        fclose(gen1_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen1_save->_raw = calloc(PKSAV_GEN1_SAVE_SIZE, 1);
    fseek(gen1_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gen1_save->_raw, 1, PKSAV_GEN1_SAVE_SIZE, gen1_save_file);
    fclose(gen1_save_file);
    if(num_read != PKSAV_GEN1_SAVE_SIZE) {
        return PKSAV_ERROR_FILE_IO;
    }

    bool buffer_is_valid = false;
    pksav_buffer_is_gen1_save(
        gen1_save->_raw,
        PKSAV_GEN1_SAVE_SIZE,
        &buffer_is_valid
    );

    if(!buffer_is_valid) {
        free(gen1_save->_raw);
        return PKSAV_ERROR_INVALID_SAVE;
    }*/

    /*
     * Check if this save is for the Yellow version. The only way to check this is to check the
     * Pikachu Friendship field, which isn't used in Red/Blue. This is usually fine but will fail
     * if the trainer's Pikachu despises the trainer enough to have a friendship value of 0, which
     * is unlikely but technically possible.
     */
    /*if(gen1_save->_raw[PKSAV_GEN1_PIKACHU_FRIENDSHIP] > 0)
    {
        gen1_save->gen1_game = PKSAV_GEN1_YELLOW;
    }
    else
    {
        gen1_save->gen1_game = PKSAV_GEN1_RED_BLUE;
    }

    // Set pointers
    gen1_save->pokemon_party_ptr = (struct pksav_gen1_pokemon_party*)&gen1_save->_raw[PKSAV_GEN1_POKEMON_PARTY];

    gen1_save->current_pokemon_box_num_ptr = &gen1_save->_raw[PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM];
    gen1_save->current_pokemon_box_ptr = (struct pksav_gen1_pokemon_box*)&gen1_save->_raw[PKSAV_GEN1_CURRENT_POKEMON_BOX];

    for(uint8_t i = 0; i < 6; ++i) {
        uint16_t offset = PKSAV_GEN1_POKEMON_PC_FIRST_HALF + (sizeof(struct pksav_gen1_pokemon_box)*i);
        gen1_save->pokemon_box_ptrs[i] = (struct pksav_gen1_pokemon_box*)&gen1_save->_raw[offset];
    }
    for(uint8_t i = 6; i < 12; ++i) {
        uint16_t offset = PKSAV_GEN1_POKEMON_PC_SECOND_HALF + (sizeof(struct pksav_gen1_pokemon_box)*(i-6));
        gen1_save->pokemon_box_ptrs[i] = (struct pksav_gen1_pokemon_box*)&gen1_save->_raw[offset];
    }

    gen1_save->pokedex_seen_ptr = &gen1_save->_raw[PKSAV_GEN1_POKEDEX_SEEN];
    gen1_save->pokedex_owned_ptr = &gen1_save->_raw[PKSAV_GEN1_POKEDEX_OWNED];
    gen1_save->item_bag_ptr = (struct pksav_gen1_item_bag*)&gen1_save->_raw[PKSAV_GEN1_ITEM_BAG];
    gen1_save->item_pc_ptr = (struct pksav_gen1_item_pc*)&gen1_save->_raw[PKSAV_GEN1_ITEM_PC];
    gen1_save->time_played_ptr = (struct pksav_gen1_time*)&gen1_save->_raw[PKSAV_GEN1_TIME_PLAYED];
    gen1_save->money_ptr = &gen1_save->_raw[PKSAV_GEN1_MONEY];
    gen1_save->casino_coins_ptr = &gen1_save->_raw[PKSAV_GEN1_CASINO_COINS];
    gen1_save->trainer_id_ptr = (uint16_t*)&gen1_save->_raw[PKSAV_GEN1_PLAYER_ID];
    gen1_save->trainer_name_ptr = &gen1_save->_raw[PKSAV_GEN1_PLAYER_NAME];
    gen1_save->rival_name_ptr = &gen1_save->_raw[PKSAV_GEN1_RIVAL_NAME];
    gen1_save->badges_ptr = &gen1_save->_raw[PKSAV_GEN1_BADGES];
    gen1_save->pikachu_friendship_ptr = &gen1_save->_raw[PKSAV_GEN1_PIKACHU_FRIENDSHIP];*/

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_save_save(
    const char* filepath,
    struct pksav_gen1_save* gen1_save
) {
    if(!filepath || !gen1_save) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Make sure we can write to this file
    /*FILE* gen1_save_file = fopen(filepath, "wb");
    if(!gen1_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    // Set checksum
    gen1_save->_raw[PKSAV_GEN1_CHECKSUM] = _pksav_get_gen1_save_checksum(gen1_save->_raw);

    // Write to file
    fwrite((void*)gen1_save->_raw, 1, PKSAV_GEN1_SAVE_SIZE, gen1_save_file);
    fclose(gen1_save_file);*/

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_save_free(
    struct pksav_gen1_save* gen1_save
) {
    if(!gen1_save) {
    //if(!gen1_save || !gen1_save->_raw) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    //free(gen1_save->_raw);

    return PKSAV_ERROR_NONE;
}
