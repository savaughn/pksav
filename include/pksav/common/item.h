/*!
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_ITEM_H
#define PKSAV_COMMON_ITEM_H

#include <stdint.h>

#pragma pack(push,1)

//! Native format for storing an item in Generation III-IV games.
struct pksav_item
{
    /*!
     * @brief Which item is stored in this slot.
     *
     * This value is stored in little-endian, so the function ::pksav_littleendian16
     * should be used to access and modify it.
     */
    uint16_t index;
    /*!
     * @brief How many of the given item are stored in this slot.
     *
     * This value is stored in little-endian, so the function ::pksav_littleendian16
     * should be used to access and modify it.
     */
    uint16_t count;
};

#pragma pack(pop)

#endif /* PKSAV_COMMON_ITEM_H */
