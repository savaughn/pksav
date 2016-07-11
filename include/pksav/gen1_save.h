/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_SAVE_H
#define PKSAV_GEN1_SAVE_H

#include <pksav/config.h>
#include <pksav/datetime.h>
#include <pksav/error.h>
#include <pksav/items.h>
#include <pksav/pokemon.h>

#include <pksav/math/base256.h>
#include <pksav/math/bcd.h>
#include <pksav/math/endian.h>

#include <stdbool.h>
#include <stdint.h>

#define PKSAV_GEN1_SAVE_SIZE 0x8000

typedef enum {
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
} pksav_gen1_save_offset_t;

typedef struct {
    pksav_gen1_pokemon_party_t* pokemon_party;
    pksav_gen1_pokemon_box_t** pokemon_boxes;

    pksav_gen1_item_bag_t* item_bag;
    pksav_gen1_item_pc_t* item_pc;

    pksav_gen1_time_t* time_played;
    uint8_t* money;
    uint16_t* trainer_id;
    uint8_t* trainer_name;
    uint8_t* rival_name;

    bool yellow;
    uint8_t* raw;
} pksav_gen1_save_t;

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API bool pksav_file_is_gen1_save(
    const char* filepath
);

PKSAV_API pksav_error_t pksav_gen1_save_load(
    const char* filepath,
    pksav_gen1_save_t* gen1_save
);

PKSAV_API pksav_error_t pksav_gen1_save_save(
    const char* filepath,
    pksav_gen1_save_t* gen1_save
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN1_SAVE_H */
