/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_H
#define PKSAV_GBA_SAVE_H

#include <pksav/gba/save_structs.h>

#include <stdint.h>

typedef enum {
    PKSAV_GBA_RS = 0,
    PKSAV_GBA_EMERALD,
    PKSAV_GBA_FRLG
} pksav_gba_game_t;

// How many bytes in each section are read for the checksum
static const uint16_t pksav_gba_section_sizes[14] = {
    3884,3968,3968,3968,3848,3968,3968,
    3968,3968,3968,3968,3968,3968,2000
};

#pragma pack(push,1)

#pragma pack(pop)

#endif /* PKSAV_GBA_SAVE_H */
