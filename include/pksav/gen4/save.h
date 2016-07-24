/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN4_SAVE_H
#define PKSAV_GEN4_SAVE_H

#include <pksav/config.h>
#include <pksav/gen4/items.h>
#include <pksav/gen4/pokemon.h>
#include <pksav/gen4/save_structs.h>
#include <pksav/gen4/items.h>

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    PKSAV_GEN4_DP = 0,
    PKSAV_GEN4_PLATINUM,
    PKSAV_GEN4_HGSS
} pksav_gen4_game_t;

PKSAV_API bool pksav_file_is_gen4_save(
    const char* filepath,
    pksav_gen4_game_t gen4_game
);

#endif /* PKSAV_GEN4_SAVE_H */
