/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_ITEMS_H
#define PKSAV_GEN1_ITEMS_H

#include <stdint.h>

#pragma pack(push,1)

//! Native representation of an item slot in Generation I.
struct pksav_gen1_item
{
    //! Item index.
    uint8_t index;
    //! Item count (0-99).
    uint8_t count;
};

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
    struct pksav_gen1_item items[20];
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
    struct pksav_gen1_item items[50];
    //! End of the item list.
    uint8_t terminator;
};

#pragma pack(pop)

#endif /* PKSAV_GEN1_ITEMS_H */
