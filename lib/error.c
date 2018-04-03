/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/error.h>

#include <stdlib.h>

static const char* PKSAV_ERROR_STRS[] =
{
    "No error",
    "File I/O error",
    "Parameter out of range",
    "Invalid stat enum",
    "Invalid save file",
    "Null pointer passed as parameter"
};
static const size_t NUM_ERRORS =
    sizeof(PKSAV_ERROR_STRS)/sizeof(PKSAV_ERROR_STRS[0]);

const char* pksav_strerror(
    enum pksav_error error_code
)
{
    const char* ret = NULL;
    if(((int)error_code >= 0) && ((size_t)error_code < NUM_ERRORS))
    {
        ret = PKSAV_ERROR_STRS[error_code];
    }

    return ret;
}
