/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_ITEMS_H
#define PKSAV_GEN2_ITEMS_H

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    uint8_t count;
    uint8_t index;
} pksav_gen2_item_t;

typedef struct {
    uint8_t tm_count[50];
    uint8_t hm_count[7];
} pksav_gen2_tmhm_pocket_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[20];
    uint8_t terminator;
} pksav_gen2_item_pocket_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[26];
    uint8_t terminator;
} pksav_gen2_key_item_pocket_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[12];
    uint8_t terminator;
} pksav_gen2_ball_pocket_t;

typedef struct {
    pksav_gen2_tmhm_pocket_t tmhm_pocket;
    pksav_gen2_item_pocket_t item_pocket;
    pksav_gen2_key_item_pocket_t key_item_pocket;
    pksav_gen2_ball_pocket_t ball_pocket;
} pksav_gen2_item_bag_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[50];
    uint8_t terminator;
} pksav_gen2_item_pc_t;

#pragma pack(pop)

#endif /* PKSAV_GEN2_ITEMS_H */
