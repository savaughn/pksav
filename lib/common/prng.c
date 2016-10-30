/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/prng.h>

#include <math.h>
#include <time.h>

pksav_error_t pksav_mtrng_populate(
    pksav_mtrng_t* mtrng
) {
    if(!mtrng) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    srand((unsigned int)time(NULL));
    for(size_t i = 0; i < 624; ++i) {
        mtrng->nums[i] = (uint32_t)rand();
    }
    mtrng->index = 0;

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_mtrng_next(
    pksav_mtrng_t* mtrng,
    uint32_t* next_out
) {
    if(!mtrng || !next_out) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    uint32_t ret = mtrng->nums[mtrng->index];
    if(mtrng->index == 623) {
        (void)pksav_mtrng_populate(mtrng);
    } else {
        ++mtrng->index;
    }

    *next_out = ret;

    return PKSAV_ERROR_NONE;
}
