/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_ERROR_H
#define PKSAV_ERROR_H

#include <pksav/config.h>

typedef enum {
    PKSAV_ERROR_NONE = 0,

    PKSAV_ERROR_FILE_IO,

    PKSAV_ERROR_INVALID_PARAM,
    PKSAV_ERROR_PARAM_OUT_OF_RANGE,

    PKSAV_ERROR_INVALID_GENERATION,

    PKSAV_ERROR_INVALID_STAT,
    PKSAV_ERROR_INVALID_EV,
    PKSAV_ERROR_INVALID_IV,

    PKSAV_ERROR_INVALID_SAVE
} pksav_error_t;

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API const char* pksav_strerror(
    pksav_error_t error_code
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_ERROR_H */
