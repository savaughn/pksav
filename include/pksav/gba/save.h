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
#include <pksav/gba/pokedex.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save_type.h>
#include <pksav/gba/time.h>
#include <pksav/gba/trainer_info.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PKSAV_GBA_NUM_POKEMON_BOXES 14

struct pksav_gba_pokemon_storage
{
    struct pksav_gba_pokemon_party* party_ptr;

    struct pksav_gba_pokemon_pc* pc_ptr;
};

struct pksav_gba_item_storage
{
    union pksav_gba_item_bag* item_bag_ptr;

    struct pksav_gba_item_pc* item_pc_ptr;
};

struct pksav_gba_misc_fields
{
    uint8_t* rival_name_ptr;

    uint32_t* money_ptr;

    uint16_t* casino_coins_ptr;
};

struct pksav_gba_save
{
    enum pksav_gba_save_type save_type;

    struct pksav_gba_time* time_played_ptr;

    struct pksav_gba_item_storage item_storage;

    struct pksav_gba_pokemon_storage pokemon_storage;

    struct pksav_gba_pokedex pokedex;

    struct pksav_gba_trainer_info* trainer_info_ptr;

    struct pksav_gba_misc_fields misc_fields;

    void* internal_ptr;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_SAVE_H */
