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

    // Double the size, to be sure.
    char* temp_buffer = calloc(num_bytes, 1);

    for(size_t index = 0; index < num_bytes; ++index)
    {
        uint8_t num1 = (buffer[index] & 0xF0) >> 4;
        uint8_t num2 = buffer[index] & 0xF;

        char num_as_string[1] = "";

        if(num1 < 0xA)
        {
            num_as_string[0] = (char)(num1 + '0');
            strncat(
                temp_buffer,
                num_as_string,
                sizeof(temp_buffer)-strlen(temp_buffer)
            );
            if(num2 < 0xA)
            {
                num_as_string[0] = (char)(num2 + '0');
                strncat(
                    temp_buffer,
                    num_as_string,
                    sizeof(temp_buffer)-strlen(temp_buffer)
                );
            }
        }
        else
        {
            break;
        }
    }

    *result_out = strtoul(temp_buffer, NULL, 10);
    free(temp_buffer);

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

    memset(buffer_out, 0xFF, num_bytes);

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
