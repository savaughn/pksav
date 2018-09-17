/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_TIME_H
#define PKSAV_GBA_TIME_H

#include <stdint.h>

#pragma pack(push,1)

//! The representation of time in a Game Boy Advance game.
struct pksav_gba_time
{
    //! Hours (0-99).
    uint16_t hours;
    //! Minutes (0-59).
    uint8_t minutes;
    //! Seconds (0-59).
    uint8_t seconds;
    //! Frames per second (0-59).
    uint8_t frames;
};

struct pksav_gba_time_offset
{
    int16_t days;
    int8_t hours;
    int8_t minutes;
    int8_t seconds;
};

#pragma pack(pop)

#endif /* PKSAV_GBA_TIME_H */
