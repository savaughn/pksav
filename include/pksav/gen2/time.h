/*
 * Copyright (c) 2015-2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_TIME_H
#define PKSAV_GEN2_TIME_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <time.h>

#pragma pack(push,1)

//! How much time has passed in a given save.
struct pksav_gen2_time
{
    //! Hours (0-23).
    uint8_t hours;
    //! Minutes (0-59).
    uint8_t minutes;
    //! Seconds (0-59).
    uint8_t seconds;
    //! Frames (0-59).
    uint8_t frames;
};

#pragma pack(pop)

//! Valid values for time of day (Crystal only).
enum pksav_gen2_time_of_day
{
    //! No value set (Gold/Silver).
    PKSAV_GEN2_NONE_SET = 0,
    //! Morning (4:00-9:59).
    PKSAV_GEN2_MORNING,
    //! Day (10:00-17:59).
    PKSAV_GEN2_DAY,
    //! Night (18:00-3:59).
    PKSAV_GEN2_NIGHT
};

//! The mask for getting/setting the time of day in the pksav_gen2_pc_pokemon_t.caught_data field.
#define PKSAV_GEN2_TIME_OF_DAY_MASK    ((uint16_t)0xC000)

//! The offset of the time of day information in the pksav_gen2_pc_pokemon_t.caught_data field.
#define PKSAV_GEN2_TIME_OF_DAY_OFFSET  (14)

#define PKSAV_GEN2_TIME_OF_DAY(field) \
    (((field) & PKSAV_GEN2_TIME_OF_DAY_MASK) >> PKSAV_GEN2_TIME_OF_DAY_OFFSET)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Set the given caught time field based on the given C time.
 *
 * \param time_in C time to convert
 * \param caught_data
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if time_in or caught_data is NULL
 */

PKSAV_API enum pksav_error pksav_gen2_set_caught_data_time_field(
    const time_t* p_ctime,
    uint16_t* p_caught_data
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_TIME_H */
