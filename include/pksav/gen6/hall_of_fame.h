/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_HALL_OF_FAME_H
#define PKSAV_GEN6_HALL_OF_FAME_H

#include <pksav/common/trainer_id.h>

#include <stdint.h>

#define PKSAV_GEN6_HALL_OF_FAME_FLAGS_GENDER_MASK   0x60
#define PKSAV_GEN6_HALL_OF_FAME_FLAGS_GENDER_OFFSET 5

#define PKSAV_GEN6_HALL_OF_FAME_FLAGS_LEVEL_MASK   0x3F80
#define PKSAV_GEN6_HALL_OF_FAME_FLAGS_LEVEL_OFFSET 7

#define PKSAV_GEN6_HALL_OF_FAME_FLAGS_SHINY_MASK   0x4000

#pragma pack(push,1)

typedef struct {
    uint16_t species;
    uint16_t held_item;
    uint16_t moves[4];
    pksav_trainer_id_t trainer_id;
    uint32_t flags;
    uint16_t nickname[11];
    uint16_t otname[11];
} pksav_gen6_hall_of_fame_pokemon_t;

#define PKSAV_GEN6_HALL_OF_FAME_ENTRY_NUM_MASK 0xFF

#define PKSAV_GEN6_HALL_OF_FAME_ENTRY_YEAR_MASK   0x3FC000
#define PKSAV_GEN6_HALL_OF_FAME_ENTRY_YEAR_OFFSET 14

#define PKSAV_GEN6_HALL_OF_FAME_ENTRY_MONTH_MASK   0x3C00000
#define PKSAV_GEN6_HALL_OF_FAME_ENTRY_MONTH_OFFSET 22

#define PKSAV_GEN6_HALL_OF_FAME_ENTRY_DAY_MASK   0x3C000000
#define PKSAV_GEN6_HALL_OF_FAME_ENTRY_DAY_OFFSET 26

typedef struct {
    uint32_t entry_info;
    pksav_gen6_hall_of_fame_pokemon_t party[6];
} pksav_gen6_hall_of_fame_entry_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_HALL_OF_FAME_H */
