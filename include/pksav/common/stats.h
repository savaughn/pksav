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
#include <stdlib.h>

enum pksav_gen1_stat
{
    PKSAV_GEN1_STAT_ATTACK,
    PKSAV_GEN1_STAT_DEFENSE,
    PKSAV_GEN1_STAT_SPEED,
    PKSAV_GEN1_STAT_SPECIAL,
    PKSAV_GEN1_STAT_HP,
    PKSAV_GEN1_STAT_COUNT
};

enum pksav_gen2_stat
{
    PKSAV_GEN2_STAT_ATTACK,
    PKSAV_GEN2_STAT_DEFENSE,
    PKSAV_GEN2_STAT_SPEED,
    PKSAV_GEN2_STAT_SPATK,
    PKSAV_GEN2_STAT_SPDEF,
    PKSAV_GEN2_STAT_HP,
    PKSAV_GEN2_STAT_COUNT
};

// Effort values (EVs)

#define PKSAV_MIN_GB_EV (0)
#define PKSAV_MAX_GB_EV (UINT16_MAX)

#define PKSAV_MIN_EV (0)
#define PKSAV_MAX_EV (UINT8_MAX)

// Individual values (IVs)

enum pksav_gb_IV
{
    PKSAV_GB_IV_ATTACK,
    PKSAV_GB_IV_DEFENSE,
    PKSAV_GB_IV_SPEED,
    PKSAV_GB_IV_SPECIAL,
    PKSAV_GB_IV_HP
};

#define PKSAV_NUM_GB_IVS (5)

#define PKSAV_MIN_GB_IV (0)
#define PKSAV_MAX_GB_IV (15)

enum pksav_IV
{
    PKSAV_IV_ATTACK,
    PKSAV_IV_DEFENSE,
    PKSAV_IV_SPEED,
    PKSAV_IV_SPATK,
    PKSAV_IV_SPDEF,
    PKSAV_IV_HP
};

#define PKSAV_NUM_IVS (6)

#define PKSAV_MIN_IV (0)
#define PKSAV_MAX_IV (31)

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_get_gb_IVs(
    const uint16_t* p_raw_IV,
    uint8_t* IVs_out,
    size_t IV_buffer_size
);

PKSAV_API enum pksav_error pksav_set_gb_IV(
    enum pksav_gb_IV stat,
    uint8_t IV_value,
    uint16_t* p_raw_IV
);

PKSAV_API enum pksav_error pksav_get_IVs(
    const uint32_t* p_raw_IV,
    uint8_t* IVs_out,
    size_t IV_buffer_size
);

PKSAV_API enum pksav_error pksav_set_IV(
    enum pksav_IV stat,
    uint8_t IV_value,
    uint32_t* p_raw_IV
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_STATS_H */
