/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_BATTLE_TOWER_H
#define PKSAV_GBA_BATTLE_TOWER_H

#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>
#include <pksav/gba/time.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

// TODO: see which can be consolidated and what needs to be in a union
// before making public

#define PKSAV_GBA_BATTLE_TOWER_PARTY_SIZE (3)

#pragma pack(push,1)

struct pksav_gba_rs_battle_tower_pokemon
{
    uint16_t species;
    uint16_t held_item;
    uint16_t moves[PKSAV_GBA_POKEMON_NUM_MOVES];
    uint8_t level;
    uint8_t pp_up;
    uint8_t ev_hp;
    uint8_t ev_atk;
    uint8_t ev_def;
    uint8_t ev_spd;
    uint8_t ev_spatk;
    uint8_t ev_spdef;
    union pksav_trainer_id id;
    uint32_t iv_ability;
    uint32_t personality;
    uint8_t nickname[PKSAV_GBA_POKEMON_NICKNAME_LENGTH + 1];
    uint8_t friendship;
};

struct pksav_gba_rs_battle_tower_trainer
{
    uint8_t trainer_class;
    uint8_t name[PKSAV_GBA_TRAINER_NAME_LENGTH + 1];
    uint8_t flags;
    uint8_t filler[2];
    uint16_t greeting[6];
};

struct pksav_gba_rs_battle_tower_record
{
    uint8_t battle_tower_level_type; // 0 = level 50, 1 = level 100 (TODO: enum)
    uint8_t trainer_class;
    uint16_t win_streak;
    uint8_t name[PKSAV_GBA_TRAINER_NAME_LENGTH + 1];
    union pksav_trainer_id id;
    uint16_t greeting[6];
    struct pksav_gba_rs_battle_tower_pokemon party[PKSAV_GBA_BATTLE_TOWER_PARTY_SIZE];
    uint32_t checksum;
};

struct pksav_gba_rs_battle_tower_ereader_trainer
{
    uint8_t unknown;
    uint8_t trainer_class;
    uint16_t win_streak;
    uint8_t name[PKSAV_GBA_TRAINER_NAME_LENGTH + 1];
    union pksav_trainer_id id;
    uint16_t greeting[6];
    uint16_t farewell_player_lost[6];
    uint16_t farewell_player_won[6];
    struct pksav_gba_rs_battle_tower_pokemon party[PKSAV_GBA_BATTLE_TOWER_PARTY_SIZE];
    uint32_t checksum;
};

struct pksav_gba_rs_battle_tower_data
{
    struct pksav_gba_rs_battle_tower_record player_record;
    struct pksav_gba_rs_battle_tower_record records[5];
    uint16_t first_party_pokemon_species;
    uint16_t defeated_by_pokemon_species;
    uint8_t defeated_by_trainer_name[PKSAV_GBA_TRAINER_NAME_LENGTH + 1];
    uint8_t first_party_pokemon_nickname[PKSAV_GBA_POKEMON_NICKNAME_LENGTH + 1];
    struct pksav_gba_rs_battle_tower_ereader_trainer ereader_trainer;
    uint8_t battle_tower_level_type; // 0 = level 50, 1 = level 100
    uint8_t battle_outcome;
    uint8_t unknown1[2];
    uint16_t current_challenge_battle_num[2];
    uint16_t current_streak_challenges_num[2];
    uint16_t record_win_streaks[2];
    uint8_t battle_tower_trainer_id;
    uint8_t selected_party_pokemon[PKSAV_GBA_BATTLE_TOWER_PARTY_SIZE];
    uint16_t prize_item;
    uint8_t unknown2[6];
    uint16_t total_battle_tower_wins;
    uint16_t best_battle_tower_win_streak;
    uint16_t current_win_streaks[2];
    uint8_t last_streak_level_type;
    uint8_t unknown[0x317];
};

#pragma pack(pop)

#endif /* PKSAV_GBA_BATTLE_TOWER_H */
