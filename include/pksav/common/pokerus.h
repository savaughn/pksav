/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_POKERUS_H
#define PKSAV_COMMON_POKERUS_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>

enum pksav_pokerus_strain
{
    PKSAV_POKERUS_STRAIN_A = 0,
    PKSAV_POKERUS_STRAIN_B,
    PKSAV_POKERUS_STRAIN_C,
    PKSAV_POKERUS_STRAIN_D
};

#define PKSAV_POKERUS_STRAIN_MASK   ((uint8_t)0xF0)
#define PKSAV_POKERUS_DURATION_MASK ((uint8_t)0x0F)

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_pokerus_get_strain(
    const uint8_t* p_pokerus,
    enum pksav_pokerus_strain* p_strain_out
);

PKSAV_API enum pksav_error pksav_pokerus_set_strain(
    uint8_t* p_pokerus,
    enum pksav_pokerus_strain strain
);

PKSAV_API enum pksav_error pksav_pokerus_get_duration(
    const uint8_t* p_pokerus,
    uint8_t* p_duration_out
);

PKSAV_API enum pksav_error pksav_pokerus_set_duration(
    uint8_t* p_pokerus,
    uint8_t duration
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_POKERUS_H */
