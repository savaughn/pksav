/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GBA_CRYPT_H
#define PKSAV_GBA_CRYPT_H

#include <pksav/gba/items.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

#include <stdbool.h>
#include <stdint.h>

void pksav_gba_crypt_pokemon(
    pksav_gba_pc_pokemon_t* gen3_pokemon,
    bool encrypt
);

void pksav_gba_save_crypt_items(
    pksav_gba_item_storage_t* gba_item_storage,
    uint32_t security_key,
    pksav_gba_game_t gba_game
);

#endif /* PKSAV_GBA_CRYPT_H */
