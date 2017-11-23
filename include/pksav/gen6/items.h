/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_ITEMS_H
#define PKSAV_GEN6_ITEMS_H

#include <pksav/common/item.h>

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    pksav_item_t items[310];
    pksav_item_t key_items[83];
    pksav_item_t tms_hms[109];
    pksav_item_t medicine[48];
    pksav_item_t berries[64];
    uint8_t unknown[0x66];
    uint16_t checksum;
} pksav_gen6_bag_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_ITEMS_H */
