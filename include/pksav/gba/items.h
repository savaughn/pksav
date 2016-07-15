/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_ITEMS_H
#define PKSAV_GBA_ITEMS_H

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    uint16_t index;
    uint16_t count;
} pksav_gba_item_t;

typedef struct {
    pksav_gba_item_t pc_items[50];
    pksav_gba_item_t items[20];
    pksav_gba_item_t key_items[20];
    pksav_gba_item_t balls[16];
    pksav_gba_item_t tms_hms[64];
    pksav_gba_item_t berries[46];
} pksav_rs_item_storage_t;

typedef struct {
    pksav_gba_item_t pc_items[50];
    pksav_gba_item_t items[30];
    pksav_gba_item_t key_items[30];
    pksav_gba_item_t balls[16];
    pksav_gba_item_t tms_hms[64];
    pksav_gba_item_t berries[46];
} pksav_emerald_item_storage_t;

typedef struct {
    pksav_gba_item_t pc_items[50];
    pksav_gba_item_t items[42];
    pksav_gba_item_t key_items[30];
    pksav_gba_item_t balls[13];
    pksav_gba_item_t tms_hms[58];
    pksav_gba_item_t berries[43];
} pksav_frlg_item_storage_t;

typedef union {
    pksav_rs_item_storage_t rs;
    pksav_emerald_item_storage_t emerald;
    pksav_frlg_item_storage_t frlg;
} pksav_gba_item_storage_t;

#pragma pack(pop)

#endif /* PKSAV_GBA_ITEMS_H */
