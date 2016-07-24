/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen4/save.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PKSAV_GEN4_SAVE_SIZE 0x80000

typedef enum {
    PKSAV_GEN4_SAVE_A = 0x00000,
    PKSAV_GEN4_SAVE_B = 0x40000
} pksav_gen4_save_offset_t;

typedef enum {
    PKSAV_GEN4_GENERAL_BLOCK_START = 0,
    PKSAV_GEN4_GENERAL_BLOCK_LENGTH,
    PKSAV_GEN4_GENERAL_BLOCK_FOOTER_START,
    PKSAV_GEN4_STORAGE_BLOCK_START,
    PKSAV_GEN4_STORAGE_BLOCK_LENGTH,
    PKSAV_GEN4_STORAGE_BLOCK_FOOTER_START
} pksav_gen4_block_info_field_t;

static const uint32_t pksav_gen4_block_info[][3] = {
    {0x00000,0x00000,0x00000}, // Small block start
    {0x0C100,0x0CF2C,0x0F628}, // Small block length
    {0x0CE0C,0x0CF18,0x0F618}, // Small block footer offset
    {0x0C100,0x0CF2C,0x0F700}, // Big block start
    {0x121E0,0x121E4,0x12310}, // Big block length
    {0x1E2CC,0x1F0Fc,0x21A00}  // Big block footer offset
};

typedef enum {
    PKSAV_GEN4_TRAINER_NAME = 0,
    PKSAV_GEN4_TRAINER_ID,
    PKSAV_GEN4_TRAINER_MONEY,
    PKSAV_GEN4_TRAINER_GENDER,
    PKSAV_GEN4_LANGUAGE,
    PKSAV_GEN4_BADGES,
    PKSAV_GEN4_HGSS_KANTO_BADGES,
    PKSAV_GEN4_AVATAR,
    PKSAV_GEN4_PARTY,
    PKSAV_GEN4_ITEM_BAG,
    PKSAV_GEN4_RIVAL_NAME,
    PKSAV_GEN4_TRAINER_SIGNATURE
} pksav_gen4_field_t;

static const uint16_t pksav_gen4_offsets[][3] = {
    {0x0064,0x0068,0x0064}, // Trainer Name
    {0x0074,0x0078,0x0074}, // Trainer ID
    {0x0078,0x007C,0x0078}, // Money
    {0x007C,0x0080,0x007C}, // Trainer Gender
    {0x007D,0x0081,0x007D}, // Language
    {0x007E,0x0082,0x007E}, // Badges
    {0x007E,0x0082,0x0083}, // Kanto Badges (HGSS only)
    {0x007F,0x0083,0x007F}, // Multiplayer Avatar
    {0x0098,0x00A0,0x0098}, // Party
    {0x0624,0x0630,0x0644}, // Item Bag
    {0x25A8,0x27E8,0x22D4}, // Rival Name
    {0x59A4,0x5BA8,0x4538}, // Trainer Signature
};

#define GEN4_BLOCK_INFO(field,game) pksav_gen4_block_info[field][game]
#define GEN4_OFFSET(field,game)     pksav_gen4_offsets[field][game]

static bool _pksav_file_is_gen4_save(
    const uint8_t* data,
    pksav_gen4_game_t gen4_game
) {
    static const uint16_t gen4_offsets[] = {0xC0F4,0xCF20,0xF61C};

    return (*(const uint32_t*)&data[gen4_offsets[gen4_game]] == GEN4_BLOCK_INFO(PKSAV_GEN4_GENERAL_BLOCK_LENGTH,gen4_game));
}

bool pksav_file_is_gen4_save(
    const char* filepath,
    pksav_gen4_game_t gen4_game
) {
    FILE* gen4_save = fopen(filepath, "r");
    if(!gen4_save) {
        return false;
    }

    fseek(gen4_save, SEEK_END, 0);

    if(ftell(gen4_save) < PKSAV_GEN4_SAVE_SIZE) {
        return false;
    }

    uint8_t* gen4_save_data = malloc(PKSAV_GEN4_SAVE_SIZE);
    fseek(gen4_save, SEEK_SET, 0);
    size_t num_read = fread((void*)gen4_save_data, 1, PKSAV_GEN4_SAVE_SIZE, gen4_save);
    fclose(gen4_save);

    bool ret = false;
    if(num_read == PKSAV_GEN4_SAVE_SIZE) {
        ret = _pksav_file_is_gen4_save(gen4_save_data, gen4_game);
    }

    free(gen4_save_data);
    return ret;
}
