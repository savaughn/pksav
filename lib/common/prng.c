/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/prng.h>

#include <math.h>
#include <time.h>

void pksav_mtrng_populate(
    pksav_mtrng_t* mtrng
) {
    srand(time(NULL));
    for(size_t i = 0; i < 624; ++i) {
        mtrng->nums[i] = (uint32_t)rand();
    }
    mtrng->index = 0;
}

uint32_t pksav_mtrng_next(
    pksav_mtrng_t* mtrng
) {
    uint32_t ret = mtrng->nums[mtrng->index];
    if(mtrng->index == 623) {
        pksav_mtrng_populate(mtrng);
    } else {
        ++mtrng->index;
    }

    return ret;
}
