/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_SAVE_H
#define PKSAV_GEN2_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gen2/items.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/text.h>

#include <pksav/math/bcd.h>
#include <pksav/math/endian.h>

#include <stdbool.h>
#include <stdint.h>

/*
 * Structures exclusive to Generation II games
 */
#pragma pack(push,1)

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t frames;
} pksav_gen2_time_t;

#pragma pack(pop)

/*
 * Savefile size and locations of fields within file
 */

#define PKSAV_GEN2_SAVE_SIZE 0x8000

typedef enum {
    PKSAV_GEN2_MALE = 0,
    PKSAV_GEN2_FEMALE
} pksav_gen2_gender_t;

typedef enum {
    PKSAV_GEN2_GS = 0,
    PKSAV_GEN2_CRYSTAL
} pksav_gen2_game_t;

typedef enum {
    PKSAV_GEN2_PLAYER_ID = 0,
    PKSAV_GEN2_PLAYER_NAME,
    PKSAV_GEN2_RIVAL_NAME,
    PKSAV_GEN2_DAYLIGHT_SAVINGS,
    PKSAV_GEN2_TIME_PLAYED,
    PKSAV_GEN2_PLAYER_PALETTE,
    PKSAV_GEN2_MONEY,
    PKSAV_GEN2_ITEM_BAG,
    PKSAV_GEN2_ITEM_PC,
    PKSAV_GEN2_CURRENT_POKEMON_BOX_NUM,
    PKSAV_GEN2_PC_BOX_NAMES,
    PKSAV_GEN2_POKEMON_PARTY,
    PKSAV_GEN2_POKEDEX_OWNED,
    PKSAV_GEN2_POKEDEX_SEEN,
    PKSAV_GEN2_CURRENT_POKEMON_BOX,
    PKSAV_GEN2_PLAYER_GENDER,
    PKSAV_GEN2_POKEMON_PC_FIRST_HALF,
    PKSAV_GEN2_POKEMON_PC_SECOND_HALF,
    PKSAV_GEN2_CHECKSUM1,
    PKSAV_GEN2_CHECKSUM2
} pksav_gen2_field_t;

static const uint16_t pksav_gen2_offsets[21][2] = {
    {0x2009,0x2009}, // Player ID
    {0x200B,0x200B}, // Player name
    {0x2021,0x2021}, // Rival name
    {0x2037,0x2037}, // Daylight savings
    {0x2053,0x2054}, // Time played
    {0x206B,0x206A}, // Player palette
    {0x23DB,0x23DB}, // Money
    {0x23E6,0x23E7}, // Item bag
    {0x247E,0x247F}, // Item PC
    {0x2724,0x2700}, // Current Pokemon box number
    {0x2727,0x2703}, // PC box names
    {0x288A,0x2865}, // Pokemon party
    {0x2A4C,0x2A27}, // Pokedex owned
    {0x2A6C,0x2A47}, // Pokedex seen
    {0x2D6C,0x2D10}, // Current Pokemon box list
    {0x3E3D,0x3E3D}, // Player gender (Crystal only)
    {0x4000,0x4000}, // Pokemon PC (first half)
    {0x6000,0x6000}, // Pokemon PC (second half)
    {0x2D69,0x2D02}, // Checksum 1
    {0x7E6D,0x1F0D}  // Checksum 2
};

#define PKSAV_GEN2_DATA(save,offset) save->raw[pksav_gen2_offsets[offset][save->gen2_game]]

/*
 * Savefile structure to be used by user
 */
typedef struct {
    // Party/box pointers
    pksav_gen2_pokemon_party_t* pokemon_party;
    pksav_gen2_pokemon_box_t** pokemon_boxes;
    pksav_gen2_pokemon_box_names_t* pokemon_box_names;

    // Item pointers
    pksav_gen2_item_bag_t* item_bag;
    pksav_gen2_item_pc_t* item_pc;

    // Trainer info
    uint8_t* trainer_name;
    uint16_t* trainer_id;
    uint8_t* trainer_gender;
    uint8_t* money;
    uint8_t* rival_name;

    // Game info
    uint8_t* daylight_savings;
    pksav_gen2_time_t* time_played;

    // Save info
    pksav_gen2_game_t gen2_game;
    uint8_t* raw;
} pksav_gen2_save_t;

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API bool pksav_file_is_gen2_save(
    const char* filepath,
    bool crystal
);

PKSAV_API pksav_error_t pksav_gen2_save_load(
    const char* filepath,
    pksav_gen2_save_t* gen2_save
);

PKSAV_API pksav_error_t pksav_gen2_save_save(
    const char* filepath,
    pksav_gen2_save_t* gen2_save
);

static PKSAV_INLINE void pksav_gen2_save_free(
    pksav_gen2_save_t* gen2_save
) {
    free(gen2_save->pokemon_boxes);
    free(gen2_save->raw);
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_SAVE_H */
