/*!
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_ITEMS_H
#define PKSAV_GEN2_ITEMS_H

#include <stdint.h>

#pragma pack(push,1)

#define PKSAV_GEN2_TM_COUNT 50
#define PKSAV_GEN2_HM_COUNT 7
#define PKSAV_GEN2_ITEM_POCKET_SIZE 20
#define PKSAV_GEN2_KEY_ITEM_POCKET_SIZE 26
#define PKSAV_GEN2_BALL_POCKET_SIZE 12
#define PKSAV_GEN2_ITEM_PC_SIZE 50

//! Native representation of an item slot in Generation II.
struct pksav_gen2_item
{
    //! Item count (0-99).
    uint8_t count;
    //! Item index.
    uint8_t index;
};

/*!
 * @brief Native representation of the TM/HM pocket in Generation II.
 *
 * This pocket does not store item indices, as there is a natural ordering to
 * the set of items that go in this pocket. Instead, an index exists for each
 * TM and HM, and the value of the index corresponds to the number of that item.
 */
struct pksav_gen2_tmhm_pocket
{
    /*!
     * @brief Number of each TM.
     *
     * Each index corresponds to the TM of that same number (offset by one), so
     * for example, tm_count[35] contains the number of TM36 held in the bag.
     */
    uint8_t tm_count[PKSAV_GEN2_TM_COUNT];
    /*!
     * @brief Number of each HM.
     *
     * Each index corresponds to the HM of that same number (offset by one), so
     * for example, hm_count[3] contains the number of HM04 held in the bag.
     *
     * For HMs, this value should only be 0-1.
     */
    uint8_t hm_count[PKSAV_GEN2_HM_COUNT];
};

/*!
 * @brief Native representation of the trainer's item pocket in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
struct pksav_gen2_item_pocket
{
    //! How many unique items are in the bag (0-20).
    uint8_t count;
    //! Item slots.
    struct pksav_gen2_item items[PKSAV_GEN2_ITEM_POCKET_SIZE];
    //! End of the item list.
    uint8_t terminator;
};

/*!
 * @brief Native representation of the trainer's key item pocket in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 *
 * @todo: verify
 */
struct pksav_gen2_key_item_pocket
{
    //! How many unique items are in the bag (0-26).
    uint8_t count;
    //! Item slots.
    uint8_t item_indices[PKSAV_GEN2_KEY_ITEM_POCKET_SIZE];
    //! End of the item list.
    uint8_t terminator;
};

/*!
 * @brief Native representation of the trainer's key ball pocket in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
struct pksav_gen2_ball_pocket
{
    //! How many unique items are in the bag (0-12).
    uint8_t count;
    //! Item slots.
    struct pksav_gen2_item items[PKSAV_GEN2_BALL_POCKET_SIZE];
    //! End of the item list.
    uint8_t terminator;
};

/*!
 * @brief Native representation of the trainer's item bag in Generation II.
 *
 * This struct is simply all four pockets, stored contiguously in memory.
 */
struct pksav_gen2_item_bag
{
    //! TM/HM pocket.
    struct pksav_gen2_tmhm_pocket tmhm_pocket;
    //! Item pocket.
    struct pksav_gen2_item_pocket item_pocket;
    //! Key item pocket.
    struct pksav_gen2_key_item_pocket key_item_pocket;
    //! Ball pocket.
    struct pksav_gen2_ball_pocket ball_pocket;
};

/*!
 * @brief Native representation of the trainer's item PC. in Generation II.
 *
 * All items in this list should be contiguous, and the amount should match
 * the value in the count field. If the count value is too small, the game will
 * ignore any later items, and if it is too large, whatever happens to be in
 * memory will be parsed as items, leading to undefined behavior when used.
 */
struct pksav_gen2_item_pc
{
    //! How many unique items are in the PC (0-50).
    uint8_t count;
    //! Item slots.
    struct pksav_gen2_item items[PKSAV_GEN2_ITEM_PC_SIZE];
    //! End of the item list.
    uint8_t terminator;
};

#pragma pack(pop)

#endif /* PKSAV_GEN2_ITEMS_H */
