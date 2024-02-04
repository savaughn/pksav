/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef pksav_gen3_MAP_H
#define pksav_gen3_MAP_H

#include <stdint.h>

#pragma pack(push,1)

struct pksav_gen3_coords
{
    int16_t x;
    int16_t y;
};

struct pksav_gen3_warp_data
{
    int8_t map_group;
    int8_t map_num;
    int8_t warp_id;

    struct pksav_gen3_coords coords;
};

struct pksav_gen3_player_location_info
{
    struct pksav_gen3_coords current_location_coords;
    struct pksav_gen3_warp_data current_location;

    struct pksav_gen3_warp_data warp1;
    struct pksav_gen3_warp_data warp2;
    struct pksav_gen3_warp_data last_heal_location;
    struct pksav_gen3_warp_data warp3;
};

#pragma pack(pop)

#endif /* pksav_gen3_MAP_H */
