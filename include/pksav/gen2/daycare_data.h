/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_DAYCARE_DATA_H
#define PKSAV_GEN2_DAYCARE_DATA_H

#include <pksav/gen2/pokemon.h>

#include <stdint.h>

#pragma pack(push,1)

// TODO: find offsets

struct pksav_gen2_daycare_pokemon_data
{
    uint8_t nickname[PKSAV_GEN2_POKEMON_NICKNAME_LENGTH];
    uint8_t otname[PKSAV_GEN2_POKEMON_OTNAME_STORAGE_LENGTH];
    struct pksav_gen2_pc_pokemon pokemon;
};

struct pksav_gen2_daycare_data
{
    uint8_t daycare_man_fields;
    struct pksav_gen2_daycare_pokemon_data stored_pokemon1_data;

    uint8_t daycare_lady_fields;
    uint8_t steps_to_egg;
    uint8_t is_breed_mother_or_non_ditto;
    struct pksav_gen2_daycare_pokemon_data stored_pokemon2_data;

    struct pksav_gen2_daycare_pokemon_data egg_pokemon_data;
};

#pragma pack(pop)

#endif /* PKSAV_GEN2_DAYCARE_DATA_H */
