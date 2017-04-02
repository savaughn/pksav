/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_ITEMS_H
#define PKSAV_GBA_ITEMS_H

#include <pksav/common/item.h>

#include <stdint.h>

#include <pksav/common/item.h>

#pragma pack(push,1)

//! Internal represention of item storage in Pokémon Ruby/Sapphire.
typedef struct {
    //! Items stored in the PC.
    pksav_item_t pc_items[50];
    //! Items stored in the Items pocket.
    pksav_item_t items[20];
    //! Items stored in the Key Items pocket.
    pksav_item_t key_items[20];
    //! Items stored in the Poké Balls pocket.
    pksav_item_t balls[16];
    //! Items stored in the TMs & HMs pocket.
    pksav_item_t tms_hms[64];
    //! Items stored in the Berries pocket.
    pksav_item_t berries[46];
} pksav_rs_item_storage_t;

//! Internal represention of item storage in Pokémon Emerald.
typedef struct {
    //! Items stored in the PC.
    pksav_item_t pc_items[50];
    //! Items stored in the Items pocket.
    pksav_item_t items[30];
    //! Items stored in the Key Items pocket.
    pksav_item_t key_items[30];
    //! Items stored in the Poké Balls pocket.
    pksav_item_t balls[16];
    //! Items stored in the TMs & HMs pocket.
    pksav_item_t tms_hms[64];
    //! Items stored in the Berries pocket.
    pksav_item_t berries[46];
} pksav_emerald_item_storage_t;

//! Internal representation of item storage in Pokémon FireRed/LeafGreen.
typedef struct {
    //! Items stored in the PC.
    pksav_item_t pc_items[50];
    //! Items stored in the Items pocket.
    pksav_item_t items[42];
    //! Items stored in the Key Items pocket.
    pksav_item_t key_items[30];
    //! Items stored in the Poké Balls pocket.
    pksav_item_t balls[13];
    //! Items stored in the TM Case.
    pksav_item_t tms_hms[58];
    //! Items stored in the Berry Pouch.
    pksav_item_t berries[43];
} pksav_frlg_item_storage_t;

/*!
 * @brief A union of all three Game Boy Advance item storage structs.
 *
 * This allows a common interface to be used for all three structs. As
 * nothing is stored in the same section after the item storage struct,
 * this union is safe, despite the different sizes of each struct.
 */
typedef union {
    //! Access to item storage in Pokémon Ruby/Sapphire.
    pksav_rs_item_storage_t rs;
    //! Access to item storage in Pokémon Emerald.
    pksav_emerald_item_storage_t emerald;
    //! Access to item storage in Pokémon FireRed/LeafGreen.
    pksav_frlg_item_storage_t frlg;
} pksav_gba_item_storage_t;

#pragma pack(pop)

#endif /* PKSAV_GBA_ITEMS_H */
