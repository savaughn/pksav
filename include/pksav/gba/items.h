/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_ITEMS_H
#define PKSAV_GBA_ITEMS_H

#include <pksav/common/item.h>

#include <stdint.h>

#pragma pack(push,1)

#define PKSAV_GBA_ITEM_PC_NUM_ITEMS (50)

struct pksav_gba_item_pc
{
    struct pksav_item items[PKSAV_GBA_ITEM_PC_NUM_ITEMS];
};

//! Internal represention of item storage in Pokémon Ruby/Sapphire.
struct pksav_gba_rs_item_bag
{
    //! Items stored in the Items pocket.
    struct pksav_item items[20];
    //! Items stored in the Key Items pocket.
    struct pksav_item key_items[20];
    //! Items stored in the Poké Balls pocket.
    struct pksav_item balls[16];
    //! Items stored in the TMs & HMs pocket.
    struct pksav_item tms_hms[64];
    //! Items stored in the Berries pocket.
    struct pksav_item berries[46];
};

//! Internal represention of item storage in Pokémon Emerald.
struct pksav_gba_emerald_item_bag
{
    //! Items stored in the Items pocket.
    struct pksav_item items[30];
    //! Items stored in the Key Items pocket.
    struct pksav_item key_items[30];
    //! Items stored in the Poké Balls pocket.
    struct pksav_item balls[16];
    //! Items stored in the TMs & HMs pocket.
    struct pksav_item tms_hms[64];
    //! Items stored in the Berries pocket.
    struct pksav_item berries[46];
};

//! Internal representation of item storage in Pokémon FireRed/LeafGreen.
struct pksav_gba_frlg_item_bag
{
    //! Items stored in the Items pocket.
    struct pksav_item items[42];
    //! Items stored in the Key Items pocket.
    struct pksav_item key_items[30];
    //! Items stored in the Poké Balls pocket.
    struct pksav_item balls[13];
    //! Items stored in the TM Case.
    struct pksav_item tms_hms[58];
    //! Items stored in the Berry Pouch.
    struct pksav_item berries[43];
};

/*!
 * @brief A union of all three Game Boy Advance item storage structs.
 *
 * This allows a common interface to be used for all three structs. As
 * nothing is stored in the same section after the item storage struct,
 * this union is safe, despite the different sizes of each struct.
 */
union pksav_gba_item_bag
{
    //! Access to item storage in Pokémon Ruby/Sapphire.
    struct pksav_gba_rs_item_bag rs;
    //! Access to item storage in Pokémon Emerald.
    struct pksav_gba_emerald_item_bag emerald;
    //! Access to item storage in Pokémon FireRed/LeafGreen.
    struct pksav_gba_frlg_item_bag frlg;
};

#pragma pack(pop)

#endif /* PKSAV_GBA_ITEMS_H */
