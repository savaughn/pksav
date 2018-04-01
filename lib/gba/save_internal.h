/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_INTERNAL_H
#define PKSAV_GBA_SAVE_INTERNAL_H

#include <pksav/gba/save.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

#pragma pack(push,1)

// If it weren't for the padding, this could be user-facing...
struct pksav_gba_trainer_info_internal
{
    uint8_t name[7];
    //! Padding. No relevant data is stored here.
    uint8_t padding1;
    //! Trainer gender (0: male, 1: female).
    uint8_t gender;
    //! Padding. No relevant data is stored here.
    uint8_t padding2;
    //! Trainer ID.
    union pksav_trainer_id trainer_id;
};

#define PKSAV_GBA_NUM_SAVE_SECTIONS 14

/*!
 * This union allows the data to be parsed in multiple ways, which is useful for
 * unshuffling and decryption.
 */

#define PKSAV_GBA_POKEMON_BLOCK_SIZE_BYTES 12

union pksav_gba_pokemon_blocks_internal
{
    uint8_t blocks8[PKSAV_GBA_POKEMON_BLOCK_SIZE_BYTES];
    uint16_t blocks16[PKSAV_GBA_POKEMON_BLOCK_SIZE_BYTES/2];
    uint32_t blocks32[PKSAV_GBA_POKEMON_BLOCK_SIZE_BYTES/4];
    uint8_t blocks[4][PKSAV_GBA_POKEMON_BLOCK_SIZE_BYTES/4];
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

#define PKSAV_GBA_SAVE_SECTION_SIZE_BYTES 3968

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

// How many bytes in each section are read for the checksum
static const size_t pksav_gba_section_sizes[14] =
{
    3884,3968,3968,3968,3848,3968,3968,
    3968,3968,3968,3968,3968,3968,2000
};

#pragma pack(pop)

#endif /* PKSAV_GBA_SAVE_INTERNAL_H */
