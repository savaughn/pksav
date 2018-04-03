/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/prng.h>

#include <math.h>
#include <time.h>

enum pksav_error pksav_mtrng_populate(
    struct pksav_mtrng* mtrng_ptr
)
{
    if(!mtrng_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    srand((unsigned int)time(NULL));
    for(size_t index = 0; index < 624; ++index)
    {
        mtrng_ptr->nums[index] = (uint32_t)rand();
    }
    mtrng_ptr->index = 0;

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_mtrng_next(
    struct pksav_mtrng* mtrng_ptr,
    uint32_t* next_out
)
{
    if(!mtrng_ptr || !next_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    uint32_t ret = mtrng_ptr->nums[mtrng_ptr->index];
    if(mtrng_ptr->index == 623)
    {
        (void)pksav_mtrng_populate(mtrng_ptr);
    }
    else
    {
        ++mtrng_ptr->index;
    }

    *next_out = ret;

    return PKSAV_ERROR_NONE;
}
