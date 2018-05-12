/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GEN1_SAVE_INTERNAL_H
#define PKSAV_GEN1_SAVE_INTERNAL_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct pksav_gen1_save_internal
{
    uint8_t* p_raw_save;
    uint8_t* p_checksum;

    bool is_buffer_ours;
};

// Offsets in a Generation I save
enum pksav_gen1_save_offset
{
    PKSAV_GEN1_PLAYER_NAME            = 0x2598,
    PKSAV_GEN1_POKEDEX_OWNED          = 0x25A3,
    PKSAV_GEN1_POKEDEX_SEEN           = 0x25B6,
    PKSAV_GEN1_ITEM_BAG               = 0x25C9,
    PKSAV_GEN1_MONEY                  = 0x25F3,
    PKSAV_GEN1_RIVAL_NAME             = 0x25F6,
    PKSAV_GEN1_OPTIONS                = 0x2601,
    PKSAV_GEN1_BADGES                 = 0x2602,
    PKSAV_GEN1_PLAYER_ID              = 0x2605,
    PKSAV_GEN1_PIKACHU_FRIENDSHIP     = 0x271C,
    PKSAV_GEN1_ITEM_PC                = 0x27E6,
    PKSAV_GEN1_CURRENT_BOX_NUM        = 0x284C,
    PKSAV_GEN1_CASINO_COINS           = 0x2850,
    PKSAV_GEN1_TIME_PLAYED            = 0x2CED,
    PKSAV_GEN1_DAYCARE_DATA           = 0x2CF4,
    PKSAV_GEN1_POKEMON_PARTY          = 0x2F2C,
    PKSAV_GEN1_CURRENT_BOX            = 0x30C0,
    PKSAV_GEN1_CHECKSUM               = 0x3523,
    PKSAV_GEN1_POKEMON_PC_FIRST_HALF  = 0x4000,
    PKSAV_GEN1_POKEMON_PC_SECOND_HALF = 0x6000
};

#ifdef __cplusplus
extern "C" {
#endif

static inline uint8_t pksav_gen1_get_save_checksum(
    const uint8_t* p_buffer
)
{
    assert(p_buffer != NULL);

    uint8_t checksum = 255;
    for(size_t buffer_index = PKSAV_GEN1_PLAYER_NAME;
        buffer_index < PKSAV_GEN1_CHECKSUM;
        ++buffer_index)
    {
        checksum -= p_buffer[buffer_index];
    }

    return checksum;
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN1_SAVE_INTERNAL_H */
