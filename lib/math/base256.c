/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/math/base256.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * We know what we're doing in converting the size_t to ssize_t. Subtracting
 * from a size_t equalling 0 will underflow, and we don't want that.
 */
#ifdef _MSC_VER
#    pragma warning(disable: 4552) // expected operator with side effect
#endif

enum pksav_error pksav_import_base256(
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
    float exp = 0.0;

    for(ssize_t index = (ssize_t)(num_bytes-1); index >= 0; --index)
    {
        (*p_result_out) += p_buffer[index] * (size_t)(pow(256.0f, exp++));
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_export_base256(
    size_t num,
    uint8_t* p_buffer_out,
    size_t buffer_size
)
{
    if(!p_buffer_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    memset(p_buffer_out, 0, buffer_size);

    float exp = 1.0f;
    p_buffer_out[buffer_size-1] = (uint8_t)(num % 256);
    for(ssize_t index = (ssize_t)(buffer_size-2); index >= 0; --index)
    {
        num -= p_buffer_out[index+1];
        p_buffer_out[index] = (uint8_t)((size_t)(num / pow(256,exp))
                            % (size_t)(pow(256,exp)));
        ++exp;
    }

    return PKSAV_ERROR_NONE;
}
