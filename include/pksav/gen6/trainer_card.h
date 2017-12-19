/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_TRAINER_CARD_H
#define PKSAV_GEN6_TRAINER_CARD_H

#include <pksav/common/trainer_id.h>

#include <stdint.h>

#define PKSAV_GEN6_NUM_PC_BOXES 31

#pragma pack(push,1)

typedef struct
{
    uint16_t saying[12];
    uint16_t null_terminator;
} pksav_gen6_saying_t;

typedef struct
{
    pksav_trainer_id_t id;
    uint8_t game;
    uint8_t gender;
    uint8_t multiplayer_sprite;
    uint8_t gamesync_id[16];

    uint8_t subregion;
    uint8_t country;
    uint8_t console_region;
    uint8_t language;

    uint8_t unknown1[26];

    uint16_t name[12];
    uint16_t name_null_terminator;

    uint16_t nickname[12];
    uint16_t nickname_null_terminator;

    pksav_gen6_saying_t sayings[5];

    uint8_t unknown2[36];
} pksav_gen6_trainer_card_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_TRAINER_CARD_H */
