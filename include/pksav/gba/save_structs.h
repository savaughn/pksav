/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_STRUCTS_H
#define PKSAV_GBA_SAVE_STRUCTS_H

#include <pksav/common/trainer_id.h>

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    uint8_t section_id;
    uint8_t padding;
    uint16_t checksum;
    uint32_t validation;
    uint32_t save_index;
} pksav_gba_section_footer_t;

typedef struct {
    union {
        uint8_t data8[3968];
        uint32_t data32[992];
    };
    uint8_t padding[116];
    pksav_gba_section_footer_t footer;
} pksav_gba_save_section_t;

typedef struct {
    uint16_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t frames;
} pksav_gba_time_t;

typedef struct {
    uint8_t name[7];
    uint8_t padding_0x2;
    uint8_t gender;
    uint8_t padding_0x4;
    pksav_trainer_id_t trainer_id;
    pksav_gba_time_t time_played;
} pksav_gba_trainer_info_t;

typedef union {
    uint8_t data[sizeof(pksav_gba_save_section_t)*14];
    pksav_gba_save_section_t sections_arr[14];
    struct {
        union {
            pksav_gba_save_section_t section0;
            pksav_gba_trainer_info_t trainer_info;
        };
        pksav_gba_save_section_t section1; // Team/items
        pksav_gba_save_section_t section2; // Unknown
        pksav_gba_save_section_t section3; // Unknown
        pksav_gba_save_section_t section4; // Rival info
        pksav_gba_save_section_t section5;
        pksav_gba_save_section_t section6;
        pksav_gba_save_section_t section7;
        pksav_gba_save_section_t section8;
        pksav_gba_save_section_t section9;
        pksav_gba_save_section_t section10;
        pksav_gba_save_section_t section11;
        pksav_gba_save_section_t section12;
        pksav_gba_save_section_t section13;
    } sections;
} pksav_gba_save_t;

#pragma pack(pop)

#endif /* PKSAV_GBA_SAVE_STRUCTS_H */
