/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_POKEMON_H
#define PKSAV_GBA_POKEMON_H

#include <pksav/config.h>

#include <pksav/common/contest_stats.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

#define PKSAV_GBA_EGG_MASK uint32_t(1 << 30)
#define PKSAV_GBA_ABILITY_MASK uint32_t(1 << 31)
#define PKSAV_GBA_LEVEL_MET_MASK uint16_t(0x7F)
#define PKSAV_GBA_ORIGIN_GAME_MASK uint16_t(0x780)
#define PKSAV_GBA_ORIGIN_GAME_OFFSET 7
#define PKSAV_GBA_BALL_MASK uint16_t(0x7800)
#define PKSAV_GBA_BALL_OFFSET 11
#define PKSAV_GBA_OTGENDER_MASK uint16_t(1 << 15)

#define PKSAV_GBA_COOL_MASK uint32_t(7)
#define PKSAV_GBA_BEAUTY_OFFSET 3
#define PKSAV_GBA_BEAUTY_MASK uint32_t(7 << PKSAV_GBA_BEAUTY_OFFSET)
#define PKSAV_GBA_CUTE_OFFSET 6
#define PKSAV_GBA_CUTE_MASK uint32_t(7 << PKSAV_GBA_CUTE_OFFSET)
#define PKSAV_GBA_SMART_OFFSET 9
#define PKSAV_GBA_SMART_MASK uint32_t(7 << PKSAV_GBA_SMART_OFFSET)
#define PKSAV_GBA_TOUGH_OFFSET 12
#define PKSAV_GBA_TOUGH_MASK uint32_t(7 << PKSAV_GBA_TOUGH_OFFSET)
#define PKSAV_GBA_OBEDIENCE_MASK uint32_t(1 << 31)

#pragma pack(push,1)

typedef struct {
    uint16_t species;
    uint16_t held_item;
    uint32_t exp;
    uint8_t pp_up;
    uint8_t friendship;
    uint16_t unknown_0xA;
} pksav_gba_pokemon_growth_t;

typedef struct {
    uint16_t moves[4];
    uint8_t move_pps[4];
} pksav_gba_pokemon_attacks_t;

typedef struct {
    uint8_t ev_hp;
    uint8_t ev_atk;
    uint8_t ev_def;
    uint8_t ev_spd;
    uint8_t ev_spatk;
    uint8_t ev_spdef;
    pksav_contest_stats_t contest_stats;
} pksav_gba_pokemon_effort_t;

typedef struct {
    uint8_t pokerus;
    uint8_t met_location;
    uint16_t origin_info;
    uint32_t iv_egg_ability;
    uint32_t ribbons_obedience;
} pksav_gba_pokemon_misc_t;

typedef union {
    uint8_t blocks8[48];
    uint8_t blocks16[24];
    uint32_t blocks32[12];
    uint8_t blocks[4][12];
    struct {
        pksav_gba_pokemon_growth_t growth;
        pksav_gba_pokemon_attacks_t attacks;
        pksav_gba_pokemon_effort_t effort;
        pksav_gba_pokemon_misc_t misc;
    };
} pksav_gba_pokemon_blocks_t;

typedef struct {
    uint32_t personality;
    pksav_trainer_id_t ot_id;
    uint8_t nickname[10];
    uint16_t language;
    uint8_t otname[7];
    uint8_t markings;
    uint16_t checksum;
    uint16_t unknown_0x1E;
    pksav_gba_pokemon_blocks_t blocks;
} pksav_gba_pc_pokemon_t;

typedef struct {
    uint32_t condition;
    uint8_t level;
    uint8_t pokerus_time;
    uint16_t current_hp;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spatk;
    uint16_t spdef;
} pksav_gba_pokemon_party_data_t;

typedef struct {
    pksav_gba_pc_pokemon_t pc;
    pksav_gba_pokemon_party_data_t party_data;
} pksav_gba_party_pokemon_t;

typedef struct {
    uint32_t count;
    pksav_gba_party_pokemon_t party[6];
} pksav_gba_pokemon_party_t;

typedef struct {
    pksav_gba_pc_pokemon_t entries[30];
} pksav_gba_pokemon_box_t;

typedef struct {
    uint32_t current_box;
    pksav_gba_pokemon_box_t boxes[14];
    uint8_t box_names[14][9];
    uint8_t wallpapers[14];
} pksav_gba_pokemon_pc_t;

#pragma pack(pop)

#endif /* PKSAV_GBA_POKEMON_H */
