/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_STRUCTS_H
#define PKSAV_GBA_SAVE_STRUCTS_H

#include <pksav/common/trainer_id.h>

#include <stdint.h>

#pragma pack(push,1)

/*!
 * @brief The footer to all sections in a Game Boy Advance save.
 *
 * This contains some of the mean used to verify a save file, as
 * well as information about the save file itself.
 *
 * This struct should never need to directly be accessed by the
 * user, as the functions defined in <pksav/gba/save.h> take care
 * of all internal storage details.
 */
typedef struct {
    /*!
     * @brief Which save section this section corresponds to.
     *
     * In a save file, the 14 sections are shuffled, and PKSav uses this
     * value to unshuffle them.
     */
    uint8_t section_id;
    /*!
     * @brief Padding to keep the size of the struct even.
     *
     * No relevant data is stored here.
     */
    uint8_t padding;
    //! The checksum for the given section.
    uint16_t checksum;
    //! A magic number used to confirm that the given file is a valid save.
    uint32_t validation;
    /*!
     * @brief The save number corresponding to this save slot.
     *
     * Each save file contains to save slots. Every time the game is saved, it
     * alternates which slot is used and increments the save index in the new
     * slot. PKSav emulates this behavior.
     */
    uint32_t save_index;
} pksav_gba_section_footer_t;

/*!
 * @brief Representation of a Game Boy Advance save section.
 *
 * Each section is 3968 bytes, and each save slot is made of 14 of these sections.
 */
typedef struct {
    //! A convenience union allowing the data to be accessed 1 byte or 4 bytes at a time.
    union {
        //! Access the data one byte at a time.
        uint8_t data8[3968];
        //! Access the data in 2-byte chunks.
        uint16_t data16[1984];
        //! Access the data in 4-byte chunks.
        uint32_t data32[992];
    };
    /*!
     * @brief Padding to make the size of the struct a power of 2.
     *
     * No relevant data is stored here.
     */
    uint8_t padding[116];
    //! The section's footer.
    pksav_gba_section_footer_t footer;
} pksav_gba_save_section_t;

//! The representation of time in a Game Boy Advance game.
typedef struct {
    //! Hours (0-99).
    uint16_t hours;
    //! Minutes (0-59).
    uint8_t minutes;
    //! Seconds (0-59).
    uint8_t seconds;
    //! Frames per second (0-59).
    uint8_t frames;
} pksav_gba_time_t;

/*!
 * @brief The representation of trainer info in Game Boy Advance games.
 */
typedef struct {
    /*!
     * @brief The player's name.
     *
     * This name should be accessed with ::pksav_text_from_gba and modified
     * with ::pksav_text_to_gba.
     *
     * In either case, the num_chars parameter should be 7.
     */
    uint8_t name[7];
    //! Padding. No relevant data is stored here.
    uint8_t padding1;
    //! Trainer gender (0: male, 1: female).
    uint8_t gender;
    //! Padding. No relevant data is stored here.
    uint8_t padding2;
    //! Trainer ID.
    pksav_trainer_id_t trainer_id;
    //! How long this save in the given slot has been played.
    pksav_gba_time_t time_played;
} pksav_gba_trainer_info_t;

/*!
 * @brief The internal storage of a save slot in a Game Boy Advance save file.
 *
 * Each save slot has 14 sections, which when unshuffled, provide common offsets
 * for different structs.
 *
 * Each save file has two slots, and the game alternates which slot it saves to.
 *
 * PKSav stores this as a union, allowing the data to be accessed in various ways.
 */
typedef union {
    //! Access individual bytes in the save slot.
    uint8_t data[sizeof(pksav_gba_save_section_t)*14];
    //! Access sections as an array.
    pksav_gba_save_section_t sections_arr[14];
    struct {
        //! Section 0 (trainer info).
        union {
            //! Access the data as a normal section.
            pksav_gba_save_section_t section0;
            //! Access the data as a trainer info struct.
            pksav_gba_trainer_info_t trainer_info;
        };
        //! Section 1 (team/items).
        pksav_gba_save_section_t section1;
        //! Section 2.
        pksav_gba_save_section_t section2;
        //! Section 3.
        pksav_gba_save_section_t section3;
        //! Section 4 (rival info).
        pksav_gba_save_section_t section4;
        //! Section 5.
        pksav_gba_save_section_t section5;
        //! Section 6.
        pksav_gba_save_section_t section6;
        //! Section 7.
        pksav_gba_save_section_t section7;
        //! Section 8.
        pksav_gba_save_section_t section8;
        //! Section 9.
        pksav_gba_save_section_t section9;
        //! Section 10.
        pksav_gba_save_section_t section10;
        //! Section 11.
        pksav_gba_save_section_t section11;
        //! Section 12.
        pksav_gba_save_section_t section12;
        //! Section 13.
        pksav_gba_save_section_t section13;
    };
} pksav_gba_save_slot_t;

#pragma pack(pop)

#endif /* PKSAV_GBA_SAVE_STRUCTS_H */
