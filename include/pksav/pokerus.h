/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_POKERUS_H
#define PKSAV_POKERUS_H

#include <pksav/config.h>

#include <stdint.h>

typedef enum {
    PKSAV_POKERUS_STRAIN_A = 0,
    PKSAV_POKERUS_STRAIN_B,
    PKSAV_POKERUS_STRAIN_C,
    PKSAV_POKERUS_STRAIN_D
} pksav_pokerus_strain_t;

#define PKSAV_POKERUS_STRAIN_MASK   ((uint8_t)0xF0)
#define PKSAV_POKERUS_DURATION_MASK ((uint8_t)0x0F)

#ifdef __cplusplus
extern "C" {
#endif

static PKSAV_INLINE pksav_pokerus_strain_t pksav_pokerus_get_strain(
    uint8_t* pokerus
) {
    return (pksav_pokerus_strain_t)((((*pokerus) & PKSAV_POKERUS_STRAIN_MASK) >> 4) % 4);
}

static PKSAV_INLINE uint8_t get_pokerus_duration(
    uint8_t* pokerus
) {
    return (*pokerus) & PKSAV_POKERUS_DURATION_MASK;
}

static PKSAV_INLINE void pksav_pokerus_set_strain(
    uint8_t* pokerus,
    pksav_pokerus_strain_t strain
) {
    *pokerus = (strain << 4) | ((strain % 4) + 1);
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_POKERUS_H */
