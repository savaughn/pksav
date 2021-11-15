/*!
 * @file    pksav/gen1/map_data.h
 * @ingroup PKSav
 * @brief   Native format for storing information on the player's current map.
 *
 * Copyright (c) 2021 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_MAP_DATA_H
#define PKSAV_GEN1_MAP_DATA_H

#include <stdint.h>

#define PKSAV_GEN1_SPRITE_SET_NUM_IDS (11)
#define PKSAV_GEN1_MAX_NUM_WARPS (32)
#define PKSAV_GEN1_MAX_NUM_SIGNS (16)

#pragma pack(push,1)

typedef enum {
    PKSAV_GEN1_DIRECTION_EAST = 1,
    PKSAV_GEN1_DIRECTION_WEST = 2,
    PKSAV_GEN1_DIRECTION_SOUTH = 4,
    PKSAV_GEN1_DIRECTION_NORTH = 8
} pksav_gen1_direction_t;

typedef struct {
    uint8_t y;
    uint8_t x;
} pksav_gen1_map_alignment_t;

typedef struct {
    uint8_t y;
    uint8_t x;
} pksav_gen1_map_coords_t;

typedef struct {
    uint8_t height;
    uint8_t width;
} pksav_gen1_map_dimensions_t;

typedef struct {
    uint8_t tileset;
    pksav_gen1_map_dimensions_t dimensions;
    uint16_t data_ptr;
    uint16_t text_ptr;
    uint16_t script_ptr;
} pksav_gen1_map_header_t;

typedef struct {
    uint8_t sprite_ids[PKSAV_GEN1_SPRITE_SET_NUM_IDS];
    uint8_t id;
} pksav_gen1_sprite_set_t;

typedef struct {
    uint8_t connected_map;
    uint16_t strip_src;
    uint16_t strip_dst;
    uint8_t strip_length;
    uint8_t map_width;
    pksav_gen1_map_alignment_t connected_map_alignment;
    uint16_t connected_map_ptr;
} pksav_gen1_map_connection_t;

typedef struct {
    uint8_t connections;
    pksav_gen1_map_connection_t north;
    pksav_gen1_map_connection_t south;
    pksav_gen1_map_connection_t west;
    pksav_gen1_map_connection_t east;
} pksav_gen1_map_connection_data_t;

typedef struct {
    pksav_gen1_map_coords_t coords;
    uint8_t warp_id;
    uint8_t map_id;
} pksav_gen1_warp_entry_t;

typedef struct {
    uint8_t num_warps;
    pksav_gen1_warp_entry_t warps[PKSAV_GEN1_MAX_NUM_WARPS];
    uint8_t destination_warp_id;
} pksav_gen1_warp_data_t;

typedef struct {
    uint8_t num_signs;
    pksav_gen1_map_coords_t sign_coords[PKSAV_GEN1_MAX_NUM_SIGNS];
    uint8_t sign_ids[PKSAV_GEN1_MAX_NUM_SIGNS];
} pksav_gen1_sign_data_t;

typedef struct {
    uint8_t music_sound_id;
    uint8_t music_rom_bank;

    uint8_t palette_offset;

    uint8_t map_id;
    uint16_t block_map_ptr;
    pksav_gen1_map_coords_t coords;
    pksav_gen1_map_coords_t block_coords;
    uint8_t last_map_id;
    uint8_t unused0;

    pksav_gen1_map_header_t map_header;
    pksav_gen1_map_connection_data_t connection_data;

    pksav_gen1_sprite_set_t sprite_set;
    uint16_t object_data_temp_ptr;
    uint8_t unused1;
    uint8_t background_tile;

    pksav_gen1_warp_data_t warp_data;

    uint8_t unused2[128];

    pksav_gen1_sign_data_t sign_data;

    uint8_t num_sprites;

    pksav_gen1_map_dimensions_t offset_since_last_special_warp;

    // TODO: wram.asm, up through wGrassTile?
} pksav_gen1_current_map_data_t;

#pragma pack(pop)

#endif /* PKSAV_GEN1_MAP_DATA_H */
