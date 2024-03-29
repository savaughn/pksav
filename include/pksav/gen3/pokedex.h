/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN3_POKEDEX_H
#define PKSAV_GEN3_POKEDEX_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gen3/common.h>

#include <stdint.h>
#include <stdbool.h>

#define PKSAV_GEN3_RSE_NAT_POKEDEX_UNLOCKED_A_FLAG  ((uint16_t)0x01DA)
#define PKSAV_GEN3_FRLG_NAT_POKEDEX_UNLOCKED_A_FLAG ((uint8_t)0xB9)

#define PKSAV_GEN3_NAT_POKEDEX_UNLOCKED_B_FLAG ((uint8_t)(1 << 6))

#define PKSAV_GEN3_RSE_NAT_POKEDEX_UNLOCKED_C_FLAG  ((uint16_t)0x0203)
#define PKSAV_GEN3_FRLG_NAT_POKEDEX_UNLOCKED_C_FLAG ((uint16_t)0x5862)

struct pksav_gen3_pokedex
{
    uint8_t* p_seenA;
    uint8_t* p_seenB;
    uint8_t* p_seenC;

    uint8_t* p_owned;

    uint16_t* p_rse_nat_pokedex_unlockedA;
    uint8_t* p_frlg_nat_pokedex_unlockedA;

    uint8_t* p_nat_pokedex_unlockedB;

    uint16_t* p_nat_pokedex_unlockedC;
};

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_gen3_pokedex_set_has_seen(
    struct pksav_gen3_pokedex* p_gen3_pokedex,
    uint16_t pokedex_num,
    bool has_seen
);

PKSAV_API enum pksav_error pksav_gen3_pokedex_set_national_pokedex_unlocked(
    struct pksav_gen3_pokedex* p_gen3_pokedex,
    enum pksav_gen3_save_type save_type,
    bool is_unlocked
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN3_POKEDEX_H */
