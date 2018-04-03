/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
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

static inline uint32_t arng_next(
    uint32_t seed
)
{
    return (0x6C078965 * seed) + 1;
}

static inline uint32_t lcrng32_next(
    uint32_t seed
)
{
    return (0x41C64E6D * seed) + 0x6073;
}

static inline uint64_t lcrng64_next(
    uint64_t seed
)
{
    return (0x5D588B656C078965UL * seed) + 0x269EC3;
}

#define PKSAV_MTRNG_SIZE 624

struct pksav_mtrng
{
    uint32_t nums[PKSAV_MTRNG_SIZE];
    size_t index;
};

/*!
 * @brief Populate an MTRNG struct's random numbers.
 *
 * \param mtrng MTRNG struct to populate
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER
 */
PKSAV_API enum pksav_error pksav_mtrng_populate(
    struct pksav_mtrng* mtrng_ptr
);

PKSAV_API enum pksav_error pksav_mtrng_next(
    struct pksav_mtrng* mtrng_ptr,
    uint32_t* next_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_PRNG_H */
