/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN4_POKEMON_H
#define PKSAV_GEN4_POKEMON_H

#include <pksav/common/nds_pokemon.h>

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    uint32_t count;
    pksav_nds_party_pokemon_t party[6];
} pksav_gen4_pokemon_party_t;

typedef struct {
    uint8_t padding[4];
    pksav_nds_pc_pokemon_t entries[30];
} pksav_dppt_pokemon_box_t;

typedef struct {
    pksav_nds_pc_pokemon_t entries[30];
    uint8_t unknown1[2];
    uint16_t checksum;
    uint8_t unknown2[12];
} pksav_hgss_pokemon_box_t;

typedef struct {
    uint16_t name[11];
    uint8_t unknown[18];
} pksav_gen4_box_name_t;

typedef struct {
    uint8_t unknown[4];
    pksav_dppt_pokemon_box_t boxes[18];
    pksav_gen4_box_name_t box_names[18];
} pksav_dppt_pokemon_pc_t;

typedef struct {
    pksav_hgss_pokemon_box_t boxes[18];
    uint8_t unknown[24];
    pksav_gen4_box_name_t box_names[18];
} pksav_hgss_pokemon_pc_t;

typedef union {
    pksav_dppt_pokemon_pc_t dppt;
    pksav_hgss_pokemon_pc_t hgss;
} pksav_gen4_pokemon_pc_t;

#pragma pack(pop)

#endif /* PKSAV_GEN4_POKEMON_H */
