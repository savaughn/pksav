/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN4_ITEMS_H
#define PKSAV_GEN4_ITEMS_H

#include <stdint.h>

#include <pksav/common/item.h>

#pragma pack(push,1)

typedef struct {
    pksav_item_t items[165];
    pksav_item_t key_items[50];
    pksav_item_t tms_hms[100];
    pksav_item_t mail[12];
    pksav_item_t medicine[40];
    pksav_item_t berries[64];
    pksav_item_t balls[15];
    pksav_item_t battle_items[30];
} pksav_dppt_item_bag_t;

typedef struct {
    pksav_item_t items[165];
    pksav_item_t key_items[43];
    pksav_item_t tms_hms[100];
    pksav_item_t mail[12];
    pksav_item_t medicine[40];
    pksav_item_t berries[64];
    pksav_item_t balls[24];
    pksav_item_t battle_items[30];
} pksav_hgss_item_bag_t;

typedef union {
    pksav_dppt_item_bag_t dppt;
    pksav_hgss_item_bag_t hgss;
} pksav_gen4_item_bag_t;

#pragma pack(pop)

#endif /* PKSAV_GEN4_ITEMS_H */
