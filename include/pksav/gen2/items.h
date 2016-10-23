/*!
 * @file    pksav/gen2/items.h
 * @ingroup PKSav
 * @brief   Native formats for item storage in Generation II games.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_ITEMS_H
#define PKSAV_GEN2_ITEMS_H

#include <stdint.h>

#pragma pack(push,1)

//! Native representation of an item slot in Generation II.
typedef struct {
    //! Item count (0-99).
    uint8_t count;
    //! Item index.
    uint8_t index;
} pksav_gen2_item_t;

/*!
 * @brief Native representation of the TM/HM pocket in Generation II.
 *
 * This pocket does not store item indices, as there is a natural ordering to
 * the set of items that go in this pocket. Instead, an index exists for each
 * TM and HM, and the value of the index corresponds to the number of that item.
 */
typedef struct {
    /*!
     * @brief Number of each TM.
     *
     * Each index corresponds to the TM of that same number (offset by one), so
     * for example, tm_count[35] contains the number of TM36 held in the bag.
     */
    uint8_t tm_count[50];
    /*!
     * @brief Number of each HM.
     *
     * Each index corresponds to the HM of that same number (offset by one), so
     * for example, hm_count[3] contains the number of HM04 held in the bag.
     *
     * For HMs, this value should only be 0-1.
     */
    uint8_t hm_count[7];
} pksav_gen2_tmhm_pocket_t;

/*!
 * @brief Native representation of the trainer's item pocket in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
typedef struct {
    //! How many unique items are in the bag (0-20).
    uint8_t count;
    //! Item slots.
    pksav_gen2_item_t items[20];
    //! End of the item list.
    uint8_t terminator;
} pksav_gen2_item_pocket_t;

/*!
 * @brief Native representation of the trainer's key item pocket in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
typedef struct {
    //! How many unique items are in the bag (0-26).
    uint8_t count;
    //! Item slots.
    pksav_gen2_item_t items[26];
    //! End of the item list.
    uint8_t terminator;
} pksav_gen2_key_item_pocket_t;

/*!
 * @brief Native representation of the trainer's key ball pocket in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
typedef struct {
    //! How many unique items are in the bag (0-12).
    uint8_t count;
    //! Item slots.
    pksav_gen2_item_t items[12];
    //! End of the item list.
    uint8_t terminator;
} pksav_gen2_ball_pocket_t;

/*!
 * @brief Native representation of the trainer's item bag in Generation II.
 *
 * This struct is simply all four pockets, stored contiguously in memory.
 */
typedef struct {
    //! TM/HM pocket.
    pksav_gen2_tmhm_pocket_t tmhm_pocket;
    //! Item pocket.
    pksav_gen2_item_pocket_t item_pocket;
    //! Key item pocket.
    pksav_gen2_key_item_pocket_t key_item_pocket;
    //! Ball pocket.
    pksav_gen2_ball_pocket_t ball_pocket;
} pksav_gen2_item_bag_t;

/*!
 * @brief Native representation of the trainer's item PC. in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
typedef struct {
    //! How many unique items are in the PC (0-50).
    uint8_t count;
    //! Item slots.
    pksav_gen2_item_t items[50];
    //! End of the item list.
    uint8_t terminator;
} pksav_gen2_item_pc_t;

#pragma pack(pop)

#endif /* PKSAV_GEN2_ITEMS_H */
