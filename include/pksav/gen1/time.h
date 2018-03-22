/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_TIME_H
#define PKSAV_GEN1_TIME_H

#include <stdint.h>

#pragma pack(push,1)

//! Native representation of how much time has passed in a Generation I game.
struct pksav_gen1_time
{
    /*!
     * @brief The number of hours (stored in little-endian).
     *
     * This value should be accessed and set with ::pksav_littleendian16.
     */
    uint16_t hours;
    //! The number of minutes (0-59).
    uint8_t minutes;
    //! The number of second (0-59).
    uint8_t seconds;
};

#pragma pack(pop)

#endif /* PKSAV_GEN1_TIME_H */
