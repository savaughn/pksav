/*!
 * @file    pksav/gen1/items.h
 * @ingroup PKSav
 * @brief   Native formats for item storage in Generation I games.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_ITEMS_H
#define PKSAV_GEN1_ITEMS_H

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    uint8_t index;
    uint8_t count;
} pksav_gen1_item_t;

typedef struct {
    uint8_t count;
    pksav_gen1_item_t items[20];
    uint8_t terminator;
} pksav_gen1_item_bag_t;

typedef struct {
    uint8_t count;
    pksav_gen1_item_t items[50];
    uint8_t terminator;
} pksav_gen1_item_pc_t;

#pragma pack(pop)

#endif /* PKSAV_GEN1_ITEMS_H */
