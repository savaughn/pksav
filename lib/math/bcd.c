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
    const uint8_t* p_buffer,
    size_t num_bytes,
    size_t* p_result_out
)
{
    if(!p_buffer || !p_result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *p_result_out = 0;

    // Allocate a larger buffer than theoretically necessary. Practically,
    // this function will only be called with small buffers, so this allocation
    // will typically be on the scale of bytes anyway.
    const size_t temp_buffer_size = num_bytes * 3;
    char* p_temp_buffer = calloc(temp_buffer_size, 1);

    for(size_t index = 0; index < num_bytes; ++index)
    {
        uint8_t num1 = (p_buffer[index] & 0xF0) >> 4;
        uint8_t num2 = p_buffer[index] & 0xF;

        if(num1 < 0xA)
        {
            p_temp_buffer[strlen(p_temp_buffer)] = (char)(num1 + '0');
            if(num2 < 0xA)
            {
                p_temp_buffer[strlen(p_temp_buffer)] = (char)(num2 + '0');
            }
        }
        else
        {
            break;
        }
    }

    *p_result_out = strtoul(p_temp_buffer, NULL, 10);
    free(p_temp_buffer);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_export_bcd(
    size_t num,
    uint8_t* p_buffer_out,
    size_t num_bytes
)
{
    if(!p_buffer_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    memset(p_buffer_out, 0xFF, num_bytes);

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
        p_buffer_out[index] = (uint8_t)(num % 10);
        p_buffer_out[index] |= ((uint8_t)((num / 10) % 10) << 4);

        num /= 100;
    }

    return PKSAV_ERROR_NONE;
}
