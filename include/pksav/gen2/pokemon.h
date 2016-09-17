/*!
 * @file    pksav/gen2/pokemon.h
 * @ingroup PKSav
 * @brief   Native formats and convenience functions for Pokémon in Generation II games.
 *
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_POKEMON_H
#define PKSAV_GEN2_POKEMON_H

#define GEN2_TIME_OF_DAY_MASK    uint16_t(0xC000)
#define GEN2_TIME_OF_DAY_OFFSET  14

#define GEN2_LEVEL_CAUGHT_MASK   uint16_t(0x3F00)
#define GEN2_LEVEL_CAUGHT_OFFSET 8

#define GEN2_OT_GENDER_MASK      uint16_t(0x80)

#define GEN2_LOCATION_MASK       uint16_t(0x7F)

#pragma pack(push,1)

typedef struct {
    uint8_t species;
    uint8_t held_item;
    uint8_t moves[4];
    uint16_t ot_id;
    uint8_t exp[3];
    uint16_t ev_hp;
    uint16_t ev_atk;
    uint16_t ev_def;
    uint16_t ev_spd;
    uint16_t ev_spcl;
    uint16_t iv_data;
    uint8_t move_pps[4];
    uint8_t friendship;
    uint8_t pokerus;
    uint16_t caught_data;
    uint8_t level;
} pksav_gen2_pc_pokemon_t;

typedef struct {
    uint8_t status;
    uint8_t unused;
    uint16_t current_hp;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spatk;
    uint16_t spdef;
} pksav_gen2_pokemon_party_data_t;

typedef struct {
    pksav_gen2_pc_pokemon_t pc;
    pksav_gen2_pokemon_party_data_t party_data;
} pksav_gen2_party_pokemon_t;

typedef struct {
    uint8_t count;
    uint8_t species[7];
    pksav_gen2_party_pokemon_t party[6];
    uint8_t otnames[6][11];
    uint8_t nicknames[6][11];
} pksav_gen2_pokemon_party_t;

typedef struct {
    uint8_t count;
    uint8_t species[21];
    pksav_gen2_pc_pokemon_t entries[20];
    uint8_t otnames[20][11];
    uint8_t nicknames[20][11];
    uint8_t padding[2];
} pksav_gen2_pokemon_box_t;

typedef struct {
    pksav_gen2_pokemon_box_t boxes[14];
} pksav_gen2_pokemon_pc_t;

typedef struct {
    uint8_t names[14][9];
} pksav_gen2_pokemon_box_names_t;

#pragma pack(pop)

#endif /* PKSAV_GEN2_POKEMON_H */
