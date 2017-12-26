/*!
 * @file    pksav/gen1/save.h
 * @ingroup PKSav
 * @brief   Struct for storing Hall of Fame in Generation I games.
 *
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_HALL_OF_FAME_H
#define PKSAV_GEN1_HALL_OF_FAME_H

#include <stdint.h>

#pragma pack(push,1)

#define PKSAV_GEN1_NUM_HALL_OF_FAME_TEAMS 50

// TODO: verify structs, this comes from parsing the disassembly

typedef struct
{
    uint8_t species;
    uint8_t team_index;
    uint8_t level;
} pksav_gen1_hall_of_fame_pokemon_t;

typedef struct
{
    uint8_t index;
    pksav_gen1_hall_of_fame_pokemon_t pokemon[6];
    uint8_t nicknames[6][11];
} pksav_gen1_hall_of_fame_team_t;

typedef struct
{
    pksav_gen1_hall_of_fame_team_t teams[PKSAV_GEN1_NUM_HALL_OF_FAME_TEAMS];
} pksav_gen1_hall_of_fame_t;

#pragma pack(pop)

#endif /* PKSAV_GEN1_HALL_OF_FAME_H */
