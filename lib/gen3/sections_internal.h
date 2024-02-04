/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef pksav_gen3_BLOCKS_INTERNAL_H
#define pksav_gen3_BLOCKS_INTERNAL_H

#include "battle_tower.h"

#include <pksav/gen3/daycare.h>
#include <pksav/gen3/items.h>
#include <pksav/gen3/map.h>
#include <pksav/gen3/pokemon.h>
#include <pksav/gen3/save.h>
#include <pksav/gen3/time.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

// Structs will be stored here in order to properly populate the struct and
// will be moved public as needed.

#pragma pack(push,1)

#define pksav_gen3_NUM_POKEDEX_SLOTS 412
#define pksav_gen3_POKEDEX_BUFFER_SIZE_BYTES ((pksav_gen3_NUM_POKEDEX_SLOTS / 8) + 1)

struct pksav_gen3_player_info_internal
{
    uint8_t name[pksav_gen3_TRAINER_NAME_LENGTH];
    uint8_t terminator;
    uint8_t gender;
    uint8_t special_save_warp;
    union pksav_trainer_id id;
    struct pksav_gen3_time time_played;
};

struct pksav_gen3_pokedex_internal
{
    uint8_t pokedex_order;
    uint8_t unknown1;
    uint8_t national_pokedex_unlocked;
    uint8_t unknown2;
    uint8_t first_unown_seen_personality;
    uint8_t first_spinda_seen_personality;
    uint8_t unknown3;

    uint8_t seen[pksav_gen3_POKEDEX_BUFFER_SIZE_BYTES];
    uint8_t owned[pksav_gen3_POKEDEX_BUFFER_SIZE_BYTES];
};

struct pksav_gen3_rs_secret_base_record
{
    uint8_t secret_base_id;
    uint8_t trainer_gender_misc;
    uint8_t trainer_name[pksav_gen3_TRAINER_NAME_LENGTH]; // TODO: does this have a terminator?
    uint16_t field_e;
    uint8_t fields[2];
    uint8_t decorations[16];
    uint8_t decoration_positions[6];
    uint32_t party_personalities[pksav_gen3_PARTY_NUM_POKEMON];
    uint16_t party_moves[pksav_gen3_PARTY_NUM_POKEMON * pksav_gen3_POKEMON_NUM_MOVES];
    uint16_t party_species[pksav_gen3_PARTY_NUM_POKEMON];
    uint16_t party_held_items[pksav_gen3_PARTY_NUM_POKEMON];
    uint8_t party_levels[pksav_gen3_PARTY_NUM_POKEMON];
    uint8_t party_EVs[pksav_gen3_PARTY_NUM_POKEMON];
};

struct pksav_gen3_rs_berry_tree
{
    uint8_t berry_id;
    uint8_t stage; // TODO: bitfield enum
    uint16_t minutes_until_next_stage;
    uint8_t berry_yield;
    uint8_t stats; // TODO: bitfield enum
};

struct pksav_gen3_rs_player_room_decorations
{
    uint8_t decorations[12];
    uint8_t positions[12];
    uint8_t chairs[10];
    uint8_t plants[10];
    uint8_t ornaments[10];
    uint8_t maps[10];
    uint8_t posters[10];
    uint8_t dolls[10];
    uint8_t cushions[10];
};

#pragma pack(pop)

#endif /* pksav_gen3_BLOCKS_INTERNAL_H */
