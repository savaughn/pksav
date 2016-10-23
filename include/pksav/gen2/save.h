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

typedef enum {
    PKSAV_GEN2_MALE = 0,
    PKSAV_GEN2_FEMALE
} pksav_gen2_gender_t;

typedef enum {
    PKSAV_GEN2_GS = 0,
    PKSAV_GEN2_CRYSTAL
} pksav_gen2_game_t;

/*
 * Savefile structure to be used by user
 */
typedef struct {
    // Party/box pointers
    pksav_gen2_pokemon_party_t* pokemon_party;
    uint8_t* current_pokemon_box_num;
    pksav_gen2_pokemon_box_t* current_pokemon_box;
    pksav_gen2_pokemon_pc_t* pokemon_pc;
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

PKSAV_API bool pksav_buffer_is_gen2_save(
    const uint8_t* buffer,
    size_t buffer_len,
    bool crystal
);

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
    free(gen2_save->raw);
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_SAVE_H */
