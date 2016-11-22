/*!
 * @file    pksav/common/item.h
 * @ingroup PKSav
 * @brief   Native format for storing an item in Generation III-VI games.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_ITEM_H
#define PKSAV_COMMON_ITEM_H

#include <stdint.h>

#pragma pack(push,1)

//! Native format for storing an item in Generation III-IV games.
typedef struct {
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
} pksav_item_t;

#pragma pack(pop)

#endif /* PKSAV_COMMON_ITEM_H */
