/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_NDS_POKEMON_H
#define PKSAV_COMMON_NDS_POKEMON_H

#include <pksav/config.h>

#include <pksav/common/contest_stats.h>
#include <pksav/common/datetime.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

#define PKSAV_NDS_ISEGG_MASK            ((uint32_t)(1 << 30))
#define PKSAV_NDS_ISNICKNAMED_MASK      ((uint32_t)(1 << 31))
#define PKSAV_NDS_FATEFULENCOUNTER_MASK ((uint8_t)(1 << 0))
#define PKSAV_NDS_FEMALE_MASK           ((uint8_t)(1 << 1))
#define PKSAV_NDS_GENDERLESS_MASK       ((uint8_t)(1 << 2))
#define PKSAV_NDS_LEVELMET_MASK         ((uint8_t)0x7F)
#define PKSAV_NDS_OTGENDER_MASK         ((uint8_t)0x80)
#define PKSAV_NDS_OTGENDER_OFFSET       7

#pragma pack(push,1)

typedef struct {
    uint16_t species;
    uint16_t held_item;
    pksav_trainer_id_t ot_id;
    uint32_t exp;
    uint8_t friendship;
    uint8_t ability;
    uint8_t markings;
    uint8_t country;
    uint8_t ev_hp;
    uint8_t ev_atk;
    uint8_t ev_def;
    uint8_t ev_spd;
    uint8_t ev_spatk;
    uint8_t ev_spdef;
    pksav_contest_stats_t contest_stats;
    uint16_t sinnoh_ribbons1;
    union {
        uint16_t sinnoh_ribbons2; // Generation IV
        uint16_t unova_ribbons;   // Generation V
    };
} pksav_nds_pokemon_blockA_t;

typedef struct {
    uint16_t moves[4];
    uint8_t move_pps[4];
    uint8_t move_pp_ups[4];
    uint32_t iv_isegg_isnicknamed;
    uint32_t hoenn_ribbons;
    uint8_t form_encounterinfo; // Fateful encounter, is female, is genderless
    union {
        uint8_t shiny_leaf; // Generation IV
        uint8_t nature;     // Generation V
    };
    uint8_t gen5_info; // Has Dream World ability, N's Pokemon flag
    uint8_t unknown_0x43;
    uint16_t eggmet_plat;
    uint16_t met_plat;
} pksav_nds_pokemon_blockB_t;

typedef struct {
    uint16_t nickname[11];
    uint8_t unknown_0x5E;
    uint8_t hometown;
    uint32_t sinnoh_ribbons3;
    uint8_t unknown_0x64;
    uint8_t unknown_0x65;
    uint8_t unknown_0x66;
    uint8_t unknown_0x67;
} pksav_nds_pokemon_blockC_t;

typedef struct {
    uint16_t otname[8];
    pksav_date_t eggmet_date;
    pksav_date_t met_date;
    uint16_t eggmet_dp;
    uint16_t met_dp;
    uint8_t pokerus;
    uint8_t ball;
    uint8_t metlevel_otgender;
    uint8_t encounter_info;
    uint8_t ball_hgss;
    uint8_t unknown_0x87;
} pksav_nds_pokemon_blockD_t;

typedef union {
    uint8_t blocks8[128];
    uint16_t blocks16[64];
    uint32_t blocks32[32];
    uint8_t blocks[4][32];
    struct {
        pksav_nds_pokemon_blockA_t blockA;
        pksav_nds_pokemon_blockB_t blockB;
        pksav_nds_pokemon_blockC_t blockC;
        pksav_nds_pokemon_blockD_t blockD;
    };
} pksav_nds_pokemon_blocks_t;

#define PKSAV_NDS_PARTY_DATA_DECRYPTED_MASK uint32_t(1 << 31)
#define PKSAV_NDS_PC_DATA_DECRYPTED_MASK    uint32_t(1 << 30)
#define PKSAV_NDS_IS_EGG_MASK               uint32_t(1 << 29)

typedef struct {
    uint32_t personality;
    uint8_t isdecrypted_isegg; // Is party data decrypted, is box data decrypted, is egg
    uint8_t unknown_0x05;
    uint16_t checksum;
    pksav_nds_pokemon_blocks_t blocks;
} pksav_nds_pc_pokemon_t;

typedef struct {
    uint8_t status;
    uint8_t unknown_x89;
    uint8_t unknown_x8A;
    uint8_t unknown_x8B;
    uint8_t level;
    uint8_t capsule;
    uint16_t current_hp;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spatk;
    uint16_t spdef;
    uint8_t unknown_0x9C[80];
} pksav_nds_pokemon_party_data_t;

typedef struct {
    pksav_nds_pc_pokemon_t pc;
    pksav_nds_pokemon_party_data_t party_data;
} pksav_nds_party_pokemon_t;

#pragma pack(pop)

#endif /* PKSAV_COMMON_NDS_POKEMON_H */
