/*!
 * @file    pksav/common/coordinates.h
 * @ingroup PKSav
 * @brief   Native storage for (x,y,z) coordinates.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_COORDINATES_H
#define PKSAV_COMMON_COORDINATES_H

#pragma pack(push,1)

//! (z,y,z) coordinates.
typedef struct {
    //! X coordinate.
    uint16_t x;
    //! Y coordinate.
    uint16_t y;
    //! Z coordinate.
    uint16_t z;
} pksav_coordinates_t;

#pragma pack(pop)

#endif /* PKSAV_COMMON_COORDINATES_H */
