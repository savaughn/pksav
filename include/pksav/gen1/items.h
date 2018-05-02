/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_ITEMS_H
#define PKSAV_GEN1_ITEMS_H

#include <pksav/common/item.h>

#include <stdint.h>

#pragma pack(push,1)

#define PKSAV_GEN1_ITEM_BAG_SIZE (20)
#define PKSAV_GEN1_ITEM_PC_SIZE  (50)

/*!
 * @brief Native representation of the trainer's item bag in Generation I.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
struct pksav_gen1_item_bag
{
    //! How many unique items are in the bag (0-20).
    uint8_t count;
    //! Item slots.
    struct pksav_gb_item items[PKSAV_GEN1_ITEM_BAG_SIZE];
    //! End of the item list.
    uint8_t terminator;
};

/*!
 * @brief Native representation of the trainer's item PC. in Generation I.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
struct pksav_gen1_item_pc
{
    //! How many unique items are in the PC (0-50).
    uint8_t count;
    //! Item slots.
    struct pksav_gb_item items[PKSAV_GEN1_ITEM_PC_SIZE];
    //! End of the item list.
    uint8_t terminator;
};

#pragma pack(pop)

#endif /* PKSAV_GEN1_ITEMS_H */
