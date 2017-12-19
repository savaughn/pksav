/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_BATTLE_MAISON_DATA_H
#define PKSAV_GEN6_BATTLE_MAISON_DATA_H

#include <stdint.h>

#pragma pack(push,1)

typedef struct
{
    uint16_t current_singles_streak;
    uint16_t current_super_singles_streak;
    uint16_t best_singles_streak;
    uint16_t best_super_singles_streak;

    uint16_t current_doubles_streak;
    uint16_t current_super_doubles_streak;
    uint16_t best_doubles_streak;
    uint16_t best_super_doubles_streak;

    uint16_t current_triples_streak;
    uint16_t current_super_triples_streak;
    uint16_t best_triples_streak;
    uint16_t best_super_triples_streak;

    uint16_t current_rotation_streak;
    uint16_t current_super_rotation_streak;
    uint16_t best_rotation_streak;
    uint16_t best_super_rotation_streak;

    uint16_t current_multi_streak;
    uint16_t current_super_multi_streak;
    uint16_t best_multi_streak;
    uint16_t best_super_multi_streak;
} pksav_gen6_battle_maison_data_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_BATTLE_MAISON_DATA_H */
