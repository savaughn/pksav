/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_DATETIME_H
#define PKSAV_COMMON_DATETIME_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <time.h>

#pragma pack(push,1)

//! Representation of a date in Generation IV+.
typedef struct
{
    //! Year (since 2000).
    uint8_t year;
    //! Month (1-12).
    uint8_t month;
    //! Day (1-31).
    uint8_t day;
} pksav_date_t;

typedef struct
{
    uint16_t hours;
    uint8_t minutes;
    uint8_t seconds;
} pksav_time_t;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Convert a C time_t to an in-game date representation.
 *
 * \param time_in C time to convert
 * \param date_out Where to return converted date
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_PARAM_OUT_OF_RANGE if time_t's year is pre-2000
 * \returns PKSAV_ERROR_NULL_POINTER if time_in or date_out is NULL
 */
PKSAV_API pksav_error_t pksav_time_t_to_date_t(
    const time_t* time_in,
    pksav_date_t* date_out
);

/*!
 * @brief Convert a C time_t to the number of seconds since 1/1/2000 0:00:00.
 *
 * \param time_in C time to convert
 * \param uint32_out Where to return converted date
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_PARAM_OUT_OF_RANGE if time_t's year is pre-2000
 * \returns PKSAV_ERROR_NULL_POINTER if time_in or uint32_out is NULL
 */
PKSAV_API pksav_error_t pksav_time_t_to_uint32_t(
    const time_t* time_in,
    uint32_t* uint32_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_DATETIME_H */
