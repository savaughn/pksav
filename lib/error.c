/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/error.h>

static const char* pksav_error_strs[] = {
    "No error",
    "File I/O error",
    "Parameter out of range",
    "Invalid stat enum",
    "Invalid save file",
    "Null pointer passed as parameter"
};

const char* pksav_strerror(
    pksav_error_t error_code
) {
    return pksav_error_strs[error_code];
}
