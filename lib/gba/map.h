/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_MAP_H
#define PKSAV_GBA_MAP_H

#include <stdint.h>

// TODO: see which can be consolidated and what needs to be in a union
// before making public

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

struct pksav_gba_rs_map_object
{
    uint8_t unknown1[4];

    uint8_t sprite_id;
    uint8_t graphics_id;
    uint8_t animation_pattern;
    uint8_t trainer_id;
    uint8_t local_id;
    uint8_t map_num;
    uint8_t map_group;
    uint8_t elevation; // Top 4 bits only
    struct pksav_gba_coords coords1;
    struct pksav_gba_coords coords2;
    struct pksav_gba_coords coords3;

    uint8_t unknown2[4];

    uint8_t trainer_range_berry_tree_id;

    uint8_t unknown3[5];
};

struct pksav_gba_rs_map_object_template
{
    uint8_t local_id;
    uint8_t graphics_id;
    uint8_t unknown1;

    int16_t x;
    int16_t y;
    uint8_t elevation;

    uint8_t movement_type;

    uint8_t unknown2[4];

    uint32_t p_script;

    uint16_t flag_id;

    uint8_t unknown3[2];
};

#pragma pack(pop)

#endif /* PKSAV_GBA_MAP_H */
