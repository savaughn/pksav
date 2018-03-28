/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_H
#define PKSAV_GBA_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gba/items.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/time.h>
#include <pksav/gba/trainer_info.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PKSAV_GBA_NUM_POKEMON_BOXES 14

enum pksav_gba_save_type
{
    PKSAV_GBA_SAVE_TYPE_NONE,
    PKSAV_GBA_SAVE_TYPE_RS,
    PKSAV_GBA_SAVE_TYPE_EMERALD,
    PKSAV_GBA_SAVE_TYPE_FRLG
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_SAVE_H */
