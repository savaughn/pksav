/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN4_SAVE_STRUCTS_H
#define PKSAV_GEN4_SAVE_STRUCTS_H

#include <pksav/config.h>

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    uint32_t storage_id;
    uint32_t general_id;
    uint32_t block_size;
    uint32_t unknown0;
    uint16_t unknown1;
    uint16_t checksum;
} pksav_gen4_dppt_footer_t;

typedef struct {
    uint32_t save_index;
    uint32_t block_size;
    uint32_t unknown0;
    uint16_t unknown1;
    uint16_t checksum;
} pksav_gen4_hgss_footer_t;

typedef union {
    pksav_gen4_dppt_footer_t dppt;
    pksav_gen4_hgss_footer_t hgss;
} pksav_gen4_footer_t;

#pragma pack(pop)

#endif /* PKSAV_GEN4_SAVE_STRUCTS_H */
