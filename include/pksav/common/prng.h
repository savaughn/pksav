/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_PRNG_H
#define PKSAV_COMMON_PRNG_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t seed;
} pksav_arng_t;

typedef struct {
    uint32_t seed;
} pksav_lcrng32_t;

typedef struct {
    uint32_t nums[624];
    size_t index;
} pksav_mtrng_t;

static PKSAV_INLINE uint32_t pksav_arng_next(
    pksav_arng_t* arng
) {
    arng->seed = (0x6C078965 * arng->seed) + 1;
    return arng->seed;
}

static PKSAV_INLINE uint16_t pksav_lcrng32_next(
    pksav_lcrng32_t* lcrng32
) {
    lcrng32->seed = (0x41C64E6D * lcrng32->seed) + 0x6073;
    return (uint16_t)(lcrng32->seed >> 16);
}

/*!
 * @brief Populate an MTRNG struct's random numbers.
 *
 * \param mtrng MTRNG struct to populate
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER
 */
PKSAV_API pksav_error_t pksav_mtrng_populate(
    pksav_mtrng_t* mtrng
);

PKSAV_API pksav_error_t pksav_mtrng_next(
    pksav_mtrng_t* mtrng,
    uint32_t* next_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_PRNG_H */
