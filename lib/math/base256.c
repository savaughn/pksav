/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/math/base256.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

size_t pksav_from_base256(
    const uint8_t* buffer,
    size_t num_bytes
) {
    size_t ret = 0;
    float exp = 0.0;

    for(ssize_t i = (ssize_t)(num_bytes-1); i >= 0; i--) {
        ret += buffer[i] * (size_t)(pow(256.0f, exp++));
    }

    return ret;
}

void pksav_to_base256(
    size_t num,
    uint8_t* buffer_out
) {
    size_t num_bytes = (size_t)((log(num) / log(256) + 1));
    memset(buffer_out, 0, num_bytes);
    float exp = 1.0;

    buffer_out[num_bytes-1] = (uint8_t)(num % 256);
    for(ssize_t i = (ssize_t)(num_bytes-2); i >= 0; i--) {
        num -= buffer_out[i+1];
        buffer_out[i] = (uint8_t)((size_t)(num / pow(256,exp)) % (size_t)(pow(256,exp)));
        exp++;
    }
}
