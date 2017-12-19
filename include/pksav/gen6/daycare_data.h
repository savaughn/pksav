/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_DAYCARE_DATA_H
#define PKSAV_GEN6_DAYCARE_DATA_H

#include <pksav/gen6/pokemon.h>

#include <stdint.h>

#pragma pack(push, 1)

typedef struct
{
    uint32_t is_slot_occupied;
    uint32_t steps_since_depositing;
    pksav_gen6_pc_pokemon_t pokemon;
} pksav_gen6_daycare_slot_t;

typedef struct
{
    pksav_gen6_daycare_slot_t slots[2];
    uint64_t is_egg_available;
    uint64_t rng_seed;
} pksav_gen6_daycare_data_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_DAYCARE_DATA_H */
