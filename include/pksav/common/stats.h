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

enum pksav_gb_IV
{
    PKSAV_GB_IV_ATTACK,
    PKSAV_GB_IV_DEFENSE,
    PKSAV_GB_IV_SPEED,
    PKSAV_GB_IV_SPECIAL,
    PKSAV_GB_IV_HP
};

#define PKSAV_NUM_GB_IVS (5)
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
#define PKSAV_MAX_IV (31)

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_get_gb_IVs(
    const uint16_t* raw_IV_ptr,
    uint8_t* IVs_out,
    size_t IV_buffer_size
);

PKSAV_API enum pksav_error pksav_set_gb_IV(
    enum pksav_gb_IV stat,
    uint8_t IV_value,
    uint16_t* raw_IV_ptr
);

PKSAV_API enum pksav_error pksav_get_IVs(
    const uint32_t* raw_IV_ptr,
    uint8_t* IVs_out,
    size_t IV_buffer_size
);

PKSAV_API enum pksav_error pksav_set_IV(
    enum pksav_IV stat,
    uint8_t IV_value,
    uint32_t* raw_IV_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_STATS_H */
