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

#define PKSAV_GEN6_ITEM_POCKET_SIZE     310
#define PKSAV_GEN6_KEY_ITEM_POCKET_SIZE 83
#define PKSAV_GEN6_TM_HM_POCKET_SIZE    109
#define PKSAV_GEN6_MEDICINE_POCKET_SIZE 48
#define PKSAV_GEN6_BERRIES_POCKET_SIZE  64

typedef struct {
    pksav_item_t items[PKSAV_GEN6_ITEM_POCKET_SIZE];
    pksav_item_t key_items[PKSAV_GEN6_KEY_ITEM_POCKET_SIZE];
    pksav_item_t tms_hms[PKSAV_GEN6_TM_HM_POCKET_SIZE];
    pksav_item_t medicine[PKSAV_GEN6_MEDICINE_POCKET_SIZE];
    pksav_item_t berries[PKSAV_GEN6_BERRIES_POCKET_SIZE];
    uint8_t unknown[0x66];
    uint16_t checksum;
} pksav_gen6_bag_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_ITEMS_H */
