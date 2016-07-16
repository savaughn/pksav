/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "crypt.h"

#include <pksav/config.h>

#include <pksav/gba/items.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

#include <stdbool.h>
#include <string.h>

/*
 * Offsets and sizes
 */

#define PKSAV_GBA_SAVE_SIZE  0x20000 // TODO: small Gen III saves
#define PKSAV_GBA_VALIDATION 0x08012025

typedef enum {
    PKSAV_GBA_SAVE_A = 0x0000,
    PKSAV_GBA_SAVE_B = 0xE000
} pksav_gba_save_offset_t;

typedef enum {
    PKSAV_GBA_NAT_POKEDEX_A = 0,
    PKSAV_GBA_POKEDEX_OWNED,
    PKSAV_GBA_POKEDEX_SEEN_A,
    PKSAV_GBA_GAME_CODE,
    PKSAV_GBA_SECURITY_KEY1,
    PKSAV_GBA_SECURITY_KEY2
} pksav_gba_section0_field_t;

static const uint16_t pksav_gba_section0_offsets[][4] = {
    {0,0x019,0x019,0x01B},
    {0,0x028,0x028,0x028},
    {0,0x05C,0x05C,0x05C},
    {0,0x0AC,0x0AC,0x1F4}, // RS has no security key
    {0,0x0AC,0x0AC,0x1F4}, // Emerald has no game code
    {0,0x0AC,0xAF8,0xF20}
};

typedef enum {
    PKSAV_GBA_POKEMON_PARTY = 0,
    PKSAV_GBA_MONEY,
    PKSAV_GBA_ITEM_STORAGE,
    PKSAV_GBA_POKEDEX_SEEN_B
} pksav_gba_section1_field_t;

static const uint16_t pksav_gba_section1_offsets[][4] = {
    {0,0x234,0x490,0x498},
    {0,0x234,0x490,0x498},
    {0,0x034,0x290,0x298},
    {0,0x938,0x988,0x5F8}
};

typedef enum {
    PKSAV_GBA_NAT_POKEDEX_B = 0,
    PKSAV_GBA_NAT_POKEDEX_C
} pksav_gba_section2_field_t;

static const uint16_t pksav_gba_section2_offsets[][4] = {
    {0,0x3A6,0x402,0x068},
    {0,0x44C,0x4A8,0x11C}
};

typedef enum {
    PKSAV_GBA_POKEDEX_SEEN_C = 0,
    PKSAV_GBA_FRLG_RIVAL_NAME
} pksav_gba_section4_field_t;

static const uint16_t pksav_gba_section4_offsets[][4] = {
    {0,0xC0C,0xCA4,0xB98},
    {0,0x000,0x000,0xBCC} // FR/LG only
};

// How many bytes in each section are read for the checksum
static const uint16_t pksav_gba_section_sizes[14] = {
    3884,3968,3968,3968,3848,3968,3968,
    3968,3968,3968,3968,3968,3968,2000
};

static PKSAV_INLINE void _pksav_gba_save_unshuffle_sections(
    const pksav_gba_save_sections_t* save_sections_in,
    pksav_gba_save_sections_t* save_sections_out,
    uint8_t section_nums[14]
) {
    for(uint8_t i = 0; i < 14; ++i) {
        uint8_t section_id = save_sections_in->sections_arr[i].footer.section_id;
        save_sections_out->sections_arr[section_id] = save_sections_in->sections_arr[i];

        // Cache the original positions
        section_nums[i] = section_id;
    }
}

static PKSAV_INLINE void _pksav_gba_save_shuffle_sections(
    const pksav_gba_save_sections_t* save_sections_in,
    pksav_gba_save_sections_t* save_sections_out,
    const uint8_t section_nums[14]
) {
    for(uint8_t i = 0; i < 14; ++i) {
        save_sections_out->sections_arr[i] = save_sections_in->sections_arr[section_nums[i]];
    }
}

static void _pksav_gba_save_load_pokemon_pc(
    const pksav_gba_save_sections_t* gba_save_sections,
    pksav_gba_pokemon_pc_t* pokemon_pc_out
) {
    // Copy data from sections into contiguous data structure
    uint8_t* dst_ptr = (uint8_t*)pokemon_pc_out;
    for(uint8_t i = 0; i < 13; ++i) {
        memcpy(dst_ptr, gba_save_sections->sections_arr[i].data8, pksav_gba_section_sizes[i]);
        dst_ptr += pksav_gba_section_sizes[i];
    }

    // Decrypt Pokémon
    for(uint8_t i = 0; i < 14; ++i) {
        for(uint8_t j = 0; j < 30; ++j) {
            pksav_gba_crypt_pokemon(
                &pokemon_pc_out->boxes[i].entries[j],
                false
            );
        }
    }
}

// TODO: Pokémon checksum, saving PC
