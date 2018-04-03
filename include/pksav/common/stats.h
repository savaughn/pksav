/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_STATS_H
#define PKSAV_COMMON_STATS_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>

enum pksav_battle_stat
{
    PKSAV_STAT_NONE = 0,
    PKSAV_STAT_HP,
    PKSAV_STAT_ATTACK,
    PKSAV_STAT_DEFENSE,
    PKSAV_STAT_SPATK,
    PKSAV_STAT_SPDEF,
    PKSAV_STAT_SPEED,
    PKSAV_STAT_SPECIAL
};

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_get_gb_IV(
    const uint16_t* raw,
    enum pksav_battle_stat stat,
    uint8_t* IV_out
);

PKSAV_API enum pksav_error pksav_set_gb_IV(
    uint16_t* raw,
    enum pksav_battle_stat stat,
    uint8_t new_IV
);

PKSAV_API enum pksav_error pksav_get_IV(
    const uint32_t* raw,
    enum pksav_battle_stat stat,
    uint8_t* IV_out
);

PKSAV_API enum pksav_error pksav_set_IV(
    uint32_t* raw,
    enum pksav_battle_stat stat,
    uint8_t new_IV
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_STATS_H */
