/*!
 * @file    pksav/error.h
 * @ingroup PKSav
 * @brief   Error codes returned from PKSav functions.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_ERROR_H
#define PKSAV_ERROR_H

#include <pksav/config.h>

typedef enum {
    //! Function completed with no error
    PKSAV_ERROR_NONE = 0,
    //! A problem occurred reading/writing a save file
    PKSAV_ERROR_FILE_IO,
    //! A numeric parameter passed into the function was outside the valid range
    PKSAV_ERROR_PARAM_OUT_OF_RANGE,
    //! The given stat did not exist in the given generation
    /*!
     * The "Special Attack" and "Special Defense" stat was introduced in Generation II,
     * so passing these values into a function corresponding to Generation I will
     * produce this error.
     */
    PKSAV_ERROR_INVALID_STAT,
    //! The filepath passed in the function is not a valid save file for the given generation.
    PKSAV_ERROR_INVALID_SAVE,
    //! A null pointer was passed as a parameter.
    PKSAV_ERROR_NULL_POINTER
} pksav_error_t;

#ifdef __cplusplus
extern "C" {
#endif

//! Returns a string corresponding to the given error code
PKSAV_API const char* pksav_strerror(
    pksav_error_t error_code
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_ERROR_H */
