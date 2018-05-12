/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_BLOCKS_INTERNAL_H
#define PKSAV_GBA_BLOCKS_INTERNAL_H

#include "battle_tower.h"
#include "map.h"

#include <pksav/gba/items.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>
#include <pksav/gba/time.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

// Structs will be stored here in order to properly populate the struct and
// will be moved public as needed.

#pragma pack(push,1)

#define PKSAV_GBA_NUM_POKEDEX_SLOTS 412
#define PKSAV_GBA_POKEDEX_BUFFER_SIZE_BYTES ((PKSAV_GBA_NUM_POKEDEX_SLOTS / 8) + 1)

struct pksav_gba_trainer_info_internal
{
    uint8_t name[PKSAV_GBA_TRAINER_NAME_LENGTH];
    uint8_t terminator;
    uint8_t gender;
    uint8_t special_save_warp;
    union pksav_trainer_id id;
    struct pksav_gba_time time_played;
};

struct pksav_gba_rs_pokedex_internal
{
    uint8_t pokedex_order;
    uint8_t unknown1;
    uint8_t national_pokedex_unlocked;
    uint8_t unknown2;
    uint8_t first_unown_seen_personality;
    uint8_t first_spinda_seen_personality;
    uint8_t unknown3;

    uint8_t seen[PKSAV_GBA_POKEDEX_BUFFER_SIZE_BYTES];
    uint8_t owned[PKSAV_GBA_POKEDEX_BUFFER_SIZE_BYTES];
};

struct pksav_gba_rs_save_block0
{
    struct pksav_gba_trainer_info_internal trainer_info;

    uint8_t button_mode;
    uint8_t options[4];

    struct pksav_gba_rs_pokedex_internal pokedex;

    uint8_t unknown[8];

    struct pksav_gba_time local_time_offset;
    struct pksav_gba_time last_berry_tree_update_time;

    struct pksav_gba_rs_battle_tower_data battle_tower_data;
};

struct pksav_gba_rs_save_block1
{
    struct pksav_gba_coords position;
    struct pksav_gba_warp_data warp1;
    struct pksav_gba_warp_data warp2;
    struct pksav_gba_warp_data last_heal_location;
    struct pksav_gba_warp_data warp4;

    uint16_t saved_music;
    uint8_t weather;
    uint8_t weather_cycle_stage;
    uint8_t flash_level; // 0-4
    uint16_t map_data_id;
    uint16_t map_view[0x100];

    struct pksav_gba_pokemon_party pokemon_party;

    uint32_t money;
    uint16_t casino_coins;
    uint16_t registered_item_index;

    struct pksav_gba_rs_item_bag item_bag;

    uint8_t pokedex_seen2[PKSAV_GBA_POKEDEX_BUFFER_SIZE_BYTES];

    uint16_t berry_blender_records[3];

    uint8_t unknown1[6];

    uint16_t trainer_rematch_step_counter;
    uint8_t trainer_rematches[100];

    struct pksav_gba_rs_map_object map_objects[16];
    struct pksav_gba_rs_map_object_template map_object_templates[64];
};

#pragma pack(pop)

#endif /* PKSAV_GBA_BLOCKS_INTERNAL_H */
