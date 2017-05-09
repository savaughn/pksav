/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN4_SAVE_H
#define PKSAV_GEN4_SAVE_H

#include <pksav/config.h>

#include <pksav/common/coordinates.h>
#include <pksav/common/datetime.h>
#include <pksav/common/trainer_card_signature.h>
#include <pksav/common/trainer_id.h>

#include <pksav/gen4/items.h>
#include <pksav/gen4/pokemon.h>
#include <pksav/gen4/save_structs.h>
#include <pksav/gen4/items.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    PKSAV_GEN4_DP = 0,
    PKSAV_GEN4_PLATINUM,
    PKSAV_GEN4_HGSS
} pksav_gen4_game_t;

typedef enum {
    PKSAV_GEN4_COAL_BADGE   = 0x01,
    PKSAV_GEN4_FOREST_BADGE = 0x02,
    PKSAV_GEN4_COBBLE_BADGE = 0x04,
    PKSAV_GEN4_FEN_BADGE    = 0x08,
    PKSAV_GEN4_RELIC_BADGE  = 0x10,
    PKSAV_GEN4_MINE_BADGE   = 0x20,
    PKSAV_GEN4_ICICLE_BADGE = 0x40,
    PKSAV_GEN4_BEACON_BADGE = 0x80
} pksav_gen4_sinnoh_badge_t;

typedef enum {
    PKSAV_GEN4_ZEPHYR_BADGE  = 0x01,
    PKSAV_GEN4_HIVE_BADGE    = 0x02,
    PKSAV_GEN4_PLAIN_BADGE   = 0x04,
    PKSAV_GEN4_FOG_BADGE     = 0x08,
    PKSAV_GEN4_STORM_BADGE   = 0x10,
    PKSAV_GEN4_MINERAL_BADGE = 0x20,
    PKSAV_GEN4_GLACIER_BADGE = 0x40,
    PKSAV_GEN4_RISING_BADGE  = 0x80
} pksav_gen4_johto_badge_t;

typedef enum {
    PKSAV_GEN4_BOULDER_BADGE = 0x01,
    PKSAV_GEN4_CASCADE_BADGE = 0x02,
    PKSAV_GEN4_THUNDER_BADGE = 0x04,
    PKSAV_GEN4_RAINBOW_BADGE = 0x08,
    PKSAV_GEN4_SOUL_BADGE    = 0x10,
    PKSAV_GEN4_MARSH_BADGE   = 0x20,
    PKSAV_GEN4_VOLCANO_BADGE = 0x40,
    PKSAV_GEN4_EARTH_BADGE   = 0x80
} pksav_gen4_kanto_badge_t;

/*
 * Savefile structure to be used by user
 */
typedef struct {
    pksav_gen4_pokemon_party_t* pokemon_party;
    pksav_gen4_pokemon_pc_t* pokemon_pc;

    pksav_gen4_item_bag_t* item_bag;

    uint16_t* trainer_name;
    pksav_trainer_id_t* trainer_id;
    uint8_t* trainer_gender;
    uint32_t* money;
    uint16_t* rival_name;

    uint32_t* adventure_started_time;
    uint32_t* pokemon_league_champ_time;
    pksav_date_t* total_playtime;

    pksav_coordinates_t* player_coordinates;
    pksav_coordinates_t* hgss_follower_coordinates;

    pksav_trainer_card_signature_t* trainer_card_signature;

    uint8_t* sinnoh_johto_badges;
    uint8_t* hgss_kanto_badges;

    // Do not edit these
    pksav_gen4_game_t gen4_game;
    #ifndef __DOXYGEN__
    bool small_save;
    uint8_t* general_block;
    pksav_gen4_footer_t* general_footer;
    uint8_t* storage_block;
    pksav_gen4_footer_t* storage_footer;
    uint8_t* raw;
    #endif
} pksav_gen4_save_t;

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API pksav_error_t pksav_buffer_is_gen4_save(
    const uint8_t* buffer,
    size_t buffer_len,
    pksav_gen4_game_t gen4_game,
    bool* result_out
);

PKSAV_API pksav_error_t pksav_file_is_gen4_save(
    const char* filepath,
    pksav_gen4_game_t gen4_game,
    bool* result_out
);

PKSAV_API pksav_error_t pksav_gen4_save_load(
    const char* filepath,
    pksav_gen4_save_t* gen4_save
);

PKSAV_API pksav_error_t pksav_gen4_save_save(
    const char* filepath,
    pksav_gen4_save_t* gen4_save
);

PKSAV_API pksav_error_t pksav_gen4_save_free(
    pksav_gen4_save_t* gen4_save
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN4_SAVE_H */
