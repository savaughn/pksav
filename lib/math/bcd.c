/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/math/bcd.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

size_t pksav_from_bcd(
    const uint8_t* buffer,
    size_t num_bytes
) {
    size_t ret = 0;
    size_t mult = 100;

    ret = (buffer[num_bytes-1] & 0x0F) + ((buffer[num_bytes-1] >> 4) * 10);
    for(ssize_t i = (ssize_t)(num_bytes-2); i >= 0; i--) {
        ret  += ((buffer[i] & 0x0F) * mult);
        mult *= 10;
        ret  += ((buffer[i] >> 4) * mult);
        mult *= 10;
    }

    return ret;
}

void pksav_to_bcd(
    size_t num,
    uint8_t* buffer_out
) {
    // Find the number of needed bytes
    size_t num_bytes = (size_t)(log10(num) + 1);
    memset(buffer_out, 0, num_bytes);
    size_t mult = 100;

    buffer_out[num_bytes-1] = (((num % 100) / 10) << 4) | (num % 10);
    for(ssize_t i = (ssize_t)(num_bytes-2); i >= 0; i--) {
        buffer_out[i] = (uint8_t)((((num % (mult*100)) / (mult*10)) << 4) |
                                  ((num % (mult*10)) / mult));
        mult *= 100;
    }
}
