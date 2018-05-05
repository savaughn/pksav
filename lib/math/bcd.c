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

    // Double the size, to be sure.
    const size_t temp_buffer_size = num_bytes*2;
    char* p_temp_buffer = calloc(temp_buffer_size, 1);

    printf("---\n");
    for(size_t index = 0; index < num_bytes; ++index)
    {
        uint8_t num1 = (p_buffer[index] & 0xF0) >> 4;
        uint8_t num2 = p_buffer[index] & 0xF;

        printf("%u %u\n", num1, num2);

        char num_as_string[1] = {0};

        if(num1 < 0xA)
        {
            num_as_string[0] = (char)(num1 + '0');
            printf("%c\n", num_as_string[0]);
            strncat(
                p_temp_buffer,
                num_as_string,
                temp_buffer_size-strlen(p_temp_buffer)
            );
            if(num2 < 0xA)
            {
                num_as_string[0] = (char)(num2 + '0');
                printf("%c\n", num_as_string[0]);
                strncat(
                    p_temp_buffer,
                    num_as_string,
                    temp_buffer_size-strlen(p_temp_buffer)
                );
            }
        }
        else
        {
            break;
        }
    }
    printf("---\n");

    char* p_num_start = p_temp_buffer;
    for(size_t char_index = 0;
        (char_index < strlen(p_temp_buffer)) && (*p_num_start == '0');
        ++p_num_start);

    printf("%s %s\n", p_temp_buffer, p_num_start);
    *p_result_out = strtoul(p_num_start, NULL, 10);
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
