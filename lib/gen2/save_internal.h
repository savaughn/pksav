/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GEN2_SAVE_INTERNAL_H
#define PKSAV_GEN2_SAVE_INTERNAL_H

#include <pksav/gen2/save.h>

#include <stdint.h>

#define PKSAV_GS_CHECKSUM1 (0x2D69)
#define PKSAV_GS_CHECKSUM2 (0x7E6D)

#define PKSAV_CRYSTAL_CHECKSUM1 (0x2D02)
#define PKSAV_CRYSTAL_CHECKSUM2 (0x1F0D)

struct pksav_gen2_save_internal
{
    uint8_t* p_raw_save;

    uint16_t* p_checksum1;
    uint16_t* p_checksum2;

    bool is_buffer_ours;
};

enum pksav_gen2_field
{
    PKSAV_GEN2_DAYCARE_DATA = 0,
    PKSAV_GEN2_OPTIONS,
    PKSAV_GEN2_TEXTBOX_FRAME_INDEX,
    PKSAV_GEN2_GBPRINTER_BRIGHTNESS,
    PKSAV_GEN2_MENU_ACCOUNT,
    PKSAV_GEN2_PLAYER_ID,
    PKSAV_GEN2_PLAYER_NAME,
    PKSAV_GEN2_RIVAL_NAME,
    PKSAV_GEN2_DAYLIGHT_SAVINGS,
    PKSAV_GEN2_TIME_PLAYED,
    PKSAV_GEN2_PLAYER_PALETTE,
    PKSAV_GEN2_MONEY,
    PKSAV_GEN2_MONEY_WITH_MOM,
    PKSAV_GEN2_MOM_MONEY_POLICY,
    PKSAV_GEN2_CASINO_COINS,
    PKSAV_GEN2_JOHTO_BADGES,
    PKSAV_GEN2_KANTO_BADGES,
    PKSAV_GEN2_ITEM_BAG,
    PKSAV_GEN2_ITEM_PC,
    PKSAV_GEN2_CURRENT_BOX_NUM,
    PKSAV_GEN2_PC_BOX_NAMES,
    PKSAV_GEN2_POKEMON_PARTY,
    PKSAV_GEN2_POKEDEX_OWNED,
    PKSAV_GEN2_POKEDEX_SEEN,
    PKSAV_GEN2_CURRENT_BOX,
    PKSAV_GEN2_PLAYER_GENDER,
    PKSAV_GEN2_POKEMON_PC_FIRST_HALF,
    PKSAV_GEN2_POKEMON_PC_SECOND_HALF,
    PKSAV_GEN2_CHECKSUM1,
    PKSAV_GEN2_CHECKSUM2
};

static const size_t GS_OFFSETS[] =
{
    0x1306, // Daycare data
    0x2000, // Options
    0x2002, // Text box frame index
    0x2004, // Game Boy Printer brightness
    0x2005, // Menu account
    0x2009, // Player ID
    0x200B, // Player name
    0x2021, // Rival name
    0x2037, // Daylight savings
    0x2054, // Time played
    0x206B, // Player palette
    0x23DB, // Money
    0x23DE, // Mom's money
    0x23E1, // Mom's money policy
    0x23E2, // Casino coins
    0x23E4, // Johto badges
    0x23E5, // Kanto badges
    0x23E6, // Item bag
    0x247E, // Item PC
    0x2724, // Current Pokemon box number
    0x2727, // PC box names
    0x288A, // Pokemon party
    0x2A4C, // Pokedex owned
    0x2A6C, // Pokedex seen
    0x2D6C, // Current Pokemon box list
    0x3E3D, // Player gender (Crystal only)
    0x4000, // Pokemon PC (first half)
    0x6000, // Pokemon PC (second half)
    0x2D69, // Checksum 1
    0x7E6D  // Checksum 2
};

static const size_t CRYSTAL_OFFSETS[] =
{
    0x1C82, // Daycare data
    0x2000, // Options
    0x2002, // Text box frame index
    0x2004, // Game Boy Printer brightness
    0x2005, // Menu account
    0x2009, // Player ID
    0x200B, // Player name
    0x2021, // Rival name
    0x2037, // Daylight savings
    0x2054, // Time played
    0x206A, // Player palette
    0x23DC, // Money
    0x23DF, // Mom's money
    0x23E2, // Mom's money policy
    0x23E3, // Casino coins
    0x23E5, // Johto badges
    0x23E6, // Kanto badges
    0x23E7, // Item bag
    0x247F, // Item PC
    0x2700, // Current Pokemon box number
    0x2703, // PC box names
    0x2865, // Pokemon party
    0x2A27, // Pokedex owned
    0x2A47, // Pokedex seen
    0x2D10, // Current Pokemon box list
    0x3E3D, // Player gender (Crystal only)
    0x4000, // Pokemon PC (first half)
    0x6000, // Pokemon PC (second half)
    0x2D0D, // Checksum 1
    0x1F0D  // Checksum 2
};

#ifdef __cplusplus
extern "C" {
#endif

void pksav_gen2_get_save_checksums(
    enum pksav_gen2_save_type save_type,
    const uint8_t* p_buffer,
    uint16_t* p_checksum1_out,
    uint16_t* p_checksum2_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_SAVE_INTERNAL_H */
