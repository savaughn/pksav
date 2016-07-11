/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_NATIVE_DATETIME_H
#define PKSAV_NATIVE_DATETIME_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <time.h>

#pragma pack(push,1)
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t frames;
} pksav_gen2_time_t;

typedef struct {
    uint16_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t frames;
} pksav_gen3_time_t;

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
} pksav_date_t;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API pksav_error_t pksav_time_t_to_date_t(
    const time_t* time_in,
    pksav_date_t* date_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_NATIVE_DATETIME_H */
