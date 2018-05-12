/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_INTERNAL_H
#define PKSAV_GBA_SAVE_INTERNAL_H

#include "blocks_internal.h"

#include <pksav/gba/save.h>
#include <pksav/gba/time.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>
#include <stdlib.h>

#pragma pack(push,1)

#define PKSAV_GBA_NUM_SAVE_SECTIONS (14)

/*!
 * This union allows the data to be parsed in multiple ways, which is useful for
 * unshuffling and decryption.
 */

#define PKSAV_GBA_POKEMON_ALL_BLOCKS_SIZE_BYTES (48)

union pksav_gba_pokemon_blocks_internal
{
    uint8_t blocks8[PKSAV_GBA_POKEMON_ALL_BLOCKS_SIZE_BYTES];
    uint16_t blocks16[PKSAV_GBA_POKEMON_ALL_BLOCKS_SIZE_BYTES/2];
    uint32_t blocks32[PKSAV_GBA_POKEMON_ALL_BLOCKS_SIZE_BYTES/4];
    uint8_t blocks[4][PKSAV_GBA_POKEMON_ALL_BLOCKS_SIZE_BYTES/4];
    struct pksav_gba_pokemon_blocks by_name;
};

/*
 * The footer to all sections in a Game Boy Advance save.
 *
 * This contains some of the mean used to verify a save file, as
 * well as information about the save file itself.
 *
 * This struct should never need to directly be accessed by the
 * user, as the functions defined in <pksav/gba/save.h> take care
 * of all internal storage details.
 */
struct pksav_gba_section_footer
{
    /*
     * Which save section this section corresponds to.
     *
     * In a save file, the 14 sections are shuffled, and PKSav uses this
     * value to unshuffle them.
     */
    uint8_t section_id;
    /*
     * Padding to keep the size of the struct even.
     *
     * No relevant data is stored here.
     */
    uint8_t padding;
    // The checksum for the given section.
    uint16_t checksum;
    // A magic number used to confirm that the given file is a valid save.
    uint32_t validation;
    /*
     * The save number corresponding to this save slot.
     *
     * Each save file contains to save slots. Every time the game is saved, it
     * alternates which slot is used and increments the save index in the new
     * slot. PKSav emulates this behavior.
     */
    uint32_t save_index;
};

/*
 * Representation of a Game Boy Advance save section.
 *
 * Each section is 3968 bytes, and each save slot is made of 14 of these sections.
 */

#define PKSAV_GBA_SAVE_SECTION_SIZE_BYTES (3968)

struct pksav_gba_save_section
{
    // A convenience union allowing the data to be accessed 1 byte or 4 bytes at a time.
    union
    {
        // Access the data one byte at a time.
        uint8_t data8[PKSAV_GBA_SAVE_SECTION_SIZE_BYTES];
        // Access the data in 2-byte chunks.
        uint16_t data16[PKSAV_GBA_SAVE_SECTION_SIZE_BYTES/2];
        // Access the data in 4-byte chunks.
        uint32_t data32[PKSAV_GBA_SAVE_SECTION_SIZE_BYTES/4];
    };
    /*
     * Padding to make the size of the struct a power of 2.
     *
     * No relevant data is stored here.
     */
    uint8_t padding[116];
    // The section's footer.
    struct pksav_gba_section_footer footer;
};

/*
 * The internal storage of a save slot in a Game Boy Advance save file.
 *
 * Each save slot has 14 sections, which when unshuffled, provide common offsets
 * for different structs.
 *
 * Each save file has two slots, and the game alternates which slot it saves to.
 *
 * PKSav stores this as a union, allowing the data to be accessed in various ways.
 */
union pksav_gba_save_slot
{
    // Access individual bytes in the save slot.
    uint8_t data[sizeof(struct pksav_gba_save_section)*PKSAV_GBA_NUM_SAVE_SECTIONS];
    // Access sections as an array.
    struct pksav_gba_save_section sections_arr[PKSAV_GBA_NUM_SAVE_SECTIONS];
    struct
    {
        // Section 0 (trainer info).
        union
        {
            // Access the data as a normal section.
            struct pksav_gba_save_section section0;
            // Access the data as a trainer info struct.
            struct pksav_gba_trainer_info_internal trainer_info;
        };
        // Section 1 (team/items).
        struct pksav_gba_save_section section1;
        // Section 2.
        struct pksav_gba_save_section section2;
        // Section 3.
        struct pksav_gba_save_section section3;
        // Section 4 (rival info).
        struct pksav_gba_save_section section4;
        // Section 5.
        struct pksav_gba_save_section section5;
        // Section 6.
        struct pksav_gba_save_section section6;
        // Section 7.
        struct pksav_gba_save_section section7;
        // Section 8.
        struct pksav_gba_save_section section8;
        // Section 9.
        struct pksav_gba_save_section section9;
        // Section 10.
        struct pksav_gba_save_section section10;
        // Section 11.
        struct pksav_gba_save_section section11;
        // Section 12.
        struct pksav_gba_save_section section12;
        // Section 13.
        struct pksav_gba_save_section section13;
    };
};

#pragma pack(pop)

#define PKSAV_GBA_SAVE_SLOT_SIZE (0xE000)

struct pksav_gba_save_internal
{
    uint8_t* p_raw_save;

    // There are multiple storage formats, so we can't just use our
    // "minimum size" #define.
    size_t save_len;

    bool is_save_from_first_slot;
    union pksav_gba_save_slot unshuffled_save_slot;
    uint8_t shuffled_section_nums[PKSAV_GBA_NUM_SAVE_SECTIONS];

    struct pksav_gba_pokemon_pc consolidated_pokemon_pc;
    uint32_t* p_security_key;

    bool is_buffer_ours;
};

// Each footer has a field that must equal this value to be considered valid.
#define PKSAV_GBA_VALIDATION_MAGIC (0x08012025)

// How many bytes in each section are read for the checksum
static const size_t pksav_gba_section_sizes[14] =
{
    3884,3968,3968,3968,3848,3968,3968,
    3968,3968,3968,3968,3968,3968,2000
};

/*
 * Offsets
 */

enum pksav_gba_section0_field
{
    PKSAV_GBA_OPTIONS_BUTTON_MODE = 0,
    PKSAV_GBA_OPTIONS_TEXT,
    PKSAV_GBA_OPTIONS_SOUND_BATTLE,
    PKSAV_GBA_NAT_POKEDEX_UNLOCKED_A,
    PKSAV_GBA_POKEDEX_OWNED,
    PKSAV_GBA_POKEDEX_SEEN_A,
    PKSAV_GBA_GAME_CODE,
    PKSAV_GBA_SECURITY_KEY1,
    PKSAV_GBA_SECURITY_KEY2
};

static const size_t PKSAV_GBA_SAVE_SECTION0_OFFSETS[][9] =
{
    {0x0013,0x0014,0x0015,0x0019,0x0028,0x005C,0x00AC,0x00AC,0x00AC},
    {0x0013,0x0014,0x0015,0x0019,0x0028,0x005C,0x00AC,0x00AC,0x01F4},
    {0x0013,0x0014,0x0015,0x001B,0x0028,0x005C,0x00AC,0x0AF8,0x0F20}
};

enum pksav_gba_section1_field
{
    PKSAV_GBA_POKEMON_PARTY = 0,
    PKSAV_GBA_MONEY,
    PKSAV_GBA_CASINO_COINS,
    PKSAV_GBA_ITEM_PC,
    PKSAV_GBA_ITEM_BAG,
    PKSAV_GBA_POKEDEX_SEEN_B
};

static const size_t PKSAV_GBA_SAVE_SECTION1_OFFSETS[][6] =
{
    {0x0234,0x0490,0x0494,0x0498,0x0560,0x0938},
    {0x0234,0x0490,0x0494,0x0498,0x0560,0x0988},
    {0x0034,0x0290,0x0294,0x0298,0x0310,0x0788}
};

enum pksav_gba_section2_field
{
    PKSAV_GBA_NAT_POKEDEX_UNLOCKED_B = 0,
    PKSAV_GBA_NAT_POKEDEX_UNLOCKED_C
};

static const size_t PKSAV_GBA_SAVE_SECTION2_OFFSETS[][2] =
{
    {0x03A6,0x044C},
    {0x0402,0x04A8},
    {0x0068,0x011C}
};

enum pksav_gba_section4_field
{
    PKSAV_GBA_POKEDEX_SEEN_C = 0,
    PKSAV_GBA_FRLG_RIVAL_NAME
};

static const size_t PKSAV_GBA_SAVE_SECTION4_OFFSETS[][2] =
{
    {0x0C0C,0x0000},
    {0x0CA4,0x0000},
    {0x0B98,0x0BCC}
};

#ifdef __cplusplus
extern "C" {
#endif

union pksav_gba_save_slot* _pksav_gba_get_active_save_slot_ptr(
    uint8_t* buffer,
    size_t buffer_len
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_SAVE_INTERNAL_H */
