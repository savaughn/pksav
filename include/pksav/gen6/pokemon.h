/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_POKEMON_H
#define PKSAV_GEN6_POKEMON_H

#include <pksav/common/contest_stats.h>
#include <pksav/common/datetime.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

#define PKSAV_GEN6_NUM_PC_BOXES 31

#pragma pack(push,1)

typedef struct
{
    uint32_t encryption_key;
    uint16_t sanity_placeholder;
    uint16_t checksum;
} pksav_gen6_pokemon_info_t;

typedef struct
{
    uint8_t region;
    uint8_t country;
} pksav_gen6_geolocation_t;

typedef struct
{
    uint16_t species;
    uint16_t held_item;
    pksav_trainer_id_t ot_id;
    uint32_t exp;
    uint8_t ability;
    uint8_t ability_num;
    uint16_t training_bag_hits_remaining;
    uint32_t personality;
    uint8_t nature;
    uint8_t form_encounterinfo;
    uint8_t ev_hp;
    uint8_t ev_atk;
    uint8_t ev_def;
    uint8_t ev_spd;
    uint8_t ev_spatk;
    uint8_t ev_spdef;
    pksav_contest_stats_t contest_stats;
    uint8_t markings;
    uint8_t pokerus;
    uint32_t super_training_medals;
    uint8_t ribbons[6];
    uint16_t unused_0x36;
    uint8_t contest_memory_ribbon_count;
    uint8_t battle_memory_ribbon_count;
    uint8_t unused_0x3B[5];
} pksav_gen6_pokemon_blockA_t;

typedef struct
{
    uint16_t nickname[12];
    uint16_t null_terminator;
    uint16_t moves[4];
    uint8_t move_pps[4];
    uint16_t relearn_moves[4];
    uint8_t secret_super_training_available;
    uint8_t unused_0x73;
    uint32_t iv_isegg_isnicknamed;
} pksav_gen6_pokemon_blockB_t;

typedef struct
{
    uint16_t latest_not_ot_name[12];
    uint16_t null_terminator;
    uint8_t not_ot_gender;
    uint8_t current_handler;
    pksav_gen6_geolocation_t geolocations[5];
    uint16_t unused_0x9E;
    uint16_t unused_0xA0;
    uint8_t not_ot_friendship;
    uint8_t not_ot_affection;
    uint8_t not_ot_memory_intensity;
    uint8_t not_ot_memory_line;
    uint8_t not_ot_memory_feeling;
    uint8_t unused_0xA7;
    uint16_t not_ot_memory_textvar;
    uint16_t unused_0xAA;
    uint16_t unused_0xAC;
    uint8_t fullness;
    uint8_t enjoyment;
} pksav_gen6_pokemon_blockC_t;

typedef struct
{
    uint16_t otname[12];
    uint16_t null_terminator;
    uint8_t ot_friendship;
    uint8_t ot_affection;
    uint8_t ot_memory_intensity;
    uint8_t ot_memory_line;
    uint16_t ot_memory_textvar;
    uint8_t ot_memory_feeling;
    pksav_date_t eggmet_date;
    pksav_date_t met_date;
    uint8_t unused_0xD7;
    uint16_t eggmet_location;
    uint16_t met_location;
    uint8_t ball;
    uint8_t metlevel_otgender;
    uint8_t gen4_encounter_info;
    uint8_t ot_game;
    uint8_t country;
    uint8_t region;
    uint8_t region_3ds;
    uint8_t ot_language;
    uint32_t unused_0xE4;
} pksav_gen6_pokemon_blockD_t;

typedef union
{
    uint8_t blocks8[224];
    uint16_t blocks16[112];
    uint32_t blocks32[56];
    uint8_t blocks[4][56];
    struct
    {
        pksav_gen6_pokemon_blockA_t blockA;
        pksav_gen6_pokemon_blockB_t blockB;
        pksav_gen6_pokemon_blockC_t blockC;
        pksav_gen6_pokemon_blockD_t blockD;
    };
} pksav_gen6_pokemon_blocks_t;

typedef struct
{
    pksav_gen6_pokemon_info_t info;
    pksav_gen6_pokemon_blocks_t blocks;
} pksav_gen6_pc_pokemon_t;

typedef struct
{
    uint8_t status;
    uint8_t unknown_0xE9;
    uint16_t unused_0xEA;
    uint8_t level;
    uint8_t unused_0xED;
    uint16_t unused_0xEE;
    uint16_t current_hp;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spatk;
    uint16_t spdef;
    uint16_t unused_0xFE;
    uint32_t unused_0x100;
} pksav_gen6_pokemon_party_data_t;

typedef struct
{
    pksav_gen6_pc_pokemon_t pc;
    pksav_gen6_pokemon_party_data_t party_data;
} pksav_gen6_party_pokemon_t;

typedef struct
{
    pksav_gen6_party_pokemon_t party[6];
} pksav_gen6_pokemon_party_t;

typedef struct
{
    pksav_gen6_pc_pokemon_t entries[30];
} pksav_gen6_pokemon_box_t;

typedef struct
{
    pksav_gen6_pokemon_box_t boxes[PKSAV_GEN6_NUM_PC_BOXES];
} pksav_gen6_pokemon_pc_t;

// TODO: verify
typedef struct
{
    uint16_t name[11];
    uint8_t unknown[12];
} pksav_gen6_pokemon_box_name_t;

typedef struct
{
    pksav_gen6_pokemon_box_name_t names[PKSAV_GEN6_NUM_PC_BOXES];
    uint8_t backgrounds[PKSAV_GEN6_NUM_PC_BOXES];
    uint8_t flags[3][PKSAV_GEN6_NUM_PC_BOXES];
} pksav_gen6_pokemon_box_info_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_POKEMON_H */
