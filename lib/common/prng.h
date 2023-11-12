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

struct pksav_lcrng
{
    uint32_t seed;
};

static inline enum pksav_error pksav_lcrng_next(
    struct pksav_lcrng* p_lcrng,
    uint32_t* p_next_value
)
{
    if(!p_lcrng)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(!p_next_value)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    p_lcrng->seed = (0x41C64E6D * p_lcrng->seed) + 0x6073;
    *p_next_value = p_lcrng->seed;

    return PKSAV_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_PRNG_H */
