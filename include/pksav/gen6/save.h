/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_SAVE_H
#define PKSAV_GEN6_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gen6/coords.h>
#include <pksav/gen6/hall_of_fame.h>
#include <pksav/gen6/items.h>
#include <pksav/gen6/pokemon.h>
#include <pksav/gen6/ribbons.h>
#include <pksav/gen6/trainer_card.h>

#include <stdbool.h>
#include <stdlib.h>

#define PKSAV_GEN6_SAVE_SIZE 0x100000 // (1 MB)

typedef enum
{
    PKSAV_GEN6_XY,
    PKSAV_GEN6_ORAS
} pksav_gen6_game_t;

typedef struct
{
    pksav_gen6_trainer_card_t* trainer_card;

    pksav_gen6_pokemon_party_t* pokemon_party;
    pksav_gen6_pokemon_pc_t* pokemon_pc;
    pksav_gen6_bag_t* item_bag;

    uint32_t* money;

    // Do not edit these
#ifndef __DOXYGEN__
    pksav_gen6_game_t gen6_game;
    uint8_t* raw;
#endif
} pksav_gen6_save_t;

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API pksav_error_t pksav_buffer_is_gen6_save(
    const uint8_t* buffer,
    size_t buffer_len,
    pksav_gen6_game_t gen6_game,
    bool* result_out
);

PKSAV_API pksav_error_t pksav_file_is_gen6_save(
    const char* filepath,
    pksav_gen6_game_t gen6_game,
    bool* result_out
);

PKSAV_API pksav_error_t pksav_gen6_save_load(
    const char* filepath,
    pksav_gen6_save_t* gen6_save_out
);

PKSAV_API pksav_error_t pksav_gen6_save_save(
    const char* filepath,
    pksav_gen6_save_t* gen6_save
);

PKSAV_API pksav_error_t pksav_gen6_save_free(
    pksav_gen6_save_t* gen6_save
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN6_SAVE_H */
