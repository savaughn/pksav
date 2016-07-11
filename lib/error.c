/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/error.h>

static const char* pksav_error_strs[] = {
    "No error",
    "Invalid parameter",
    "Parameter out of range",
    "Invalid generation",
    "Invalid stat enum",
    "Invalid EV value",
    "Invalid IV value"
};

const char* pksav_strerror(
    pksav_error_t error_code
) {
    return pksav_error_strs[error_code];
}
