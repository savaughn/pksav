/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_DAYCARE_DATA_H
#define PKSAV_GEN1_DAYCARE_DATA_H

#include <pksav/gen1/pokemon.h>

#include <stdint.h>

#pragma pack(push,1)

struct pksav_gen1_daycare_data
{
    uint8_t is_daycare_in_use;
    uint8_t stored_pokemon_nickname[PKSAV_GEN1_POKEMON_NICKNAME_LENGTH];
    uint8_t stored_pokemon_otname[PKSAV_GEN1_POKEMON_OTNAME_STORAGE_LENGTH];
    struct pksav_gen1_pc_pokemon stored_pokemon;
};

#pragma pack(pop)

#endif /* PKSAV_GEN1_DAYCARE_DATA_H */
