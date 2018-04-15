/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/math/bcd.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * We know what we're doing in converting the size_t to ssize_t. Subtracting
 * from a size_t equalling 0 will underflow, and we don't want that.
 */
#ifdef _MSC_VER
#    pragma warning(disable: 4552) // expected operator with side effect
#endif

enum pksav_error pksav_import_bcd(
    const uint8_t* buffer,
    size_t num_bytes,
    size_t* result_out
)
{
    if(!buffer || !result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *result_out = 0;

    for(size_t index = 0;
        (index < num_bytes) && (buffer[index] != 0);
         ++index)
    {
        if(index > 0)
        {
            *result_out *= 100;
        }

        *result_out += (buffer[index] & 0x0F);
        *result_out += (10 * (((buffer[index] & 0xF0)) >> 4));
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_export_bcd(
    size_t num,
    uint8_t* buffer_out,
    size_t num_bytes
)
{
    if(!buffer_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    memset(buffer_out, 0, num_bytes);

    // Find the actual number of needed bytes.
    size_t log10_num = (size_t)log10((double)num);
    size_t num_needed_bytes = (size_t)((log10_num + 1) / 2);
    if((log10_num % 2) == 0)
    {
        ++num_needed_bytes;
    }

    size_t actual_num_bytes = (num_bytes < num_needed_bytes) ? num_bytes
                                                             : num_needed_bytes;

    for(ssize_t index = (actual_num_bytes-1); index >= 0; --index)
    {
        buffer_out[index] = (uint8_t)(num % 10);
        buffer_out[index] |= ((uint8_t)((num / 10) % 10) << 4);

        num /= 100;
    }

    return PKSAV_ERROR_NONE;
}
