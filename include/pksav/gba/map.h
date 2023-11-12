/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_MAP_H
#define PKSAV_GBA_MAP_H

#include <stdint.h>

#pragma pack(push,1)

struct pksav_gba_coords
{
    int16_t x;
    int16_t y;
};

struct pksav_gba_warp_data
{
    int8_t map_group;
    int8_t map_num;
    int8_t warp_id;

    struct pksav_gba_coords coords;
};

struct pksav_gba_player_location_info
{
    struct pksav_gba_coords current_location_coords;
    struct pksav_gba_warp_data current_location;

    struct pksav_gba_warp_data warp1;
    struct pksav_gba_warp_data warp2;
    struct pksav_gba_warp_data last_heal_location;
    struct pksav_gba_warp_data warp3;
};

#pragma pack(pop)

#endif /* PKSAV_GBA_MAP_H */
