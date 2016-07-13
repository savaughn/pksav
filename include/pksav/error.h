/*!
 * @file    error.h
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
    //! A parameter passed into the function was invalid in some way
    PKSAV_ERROR_INVALID_PARAM,
    //! A numeric parameter passed into the function was outside the valid range
    PKSAV_ERROR_PARAM_OUT_OF_RANGE,
    //! A string parameter passed into the function was too long
    PKSAV_ERROR_STRING_PARAM_TOO_LONG,
    //! The given generation was invalid for the function.
    PKSAV_ERROR_INVALID_GENERATION,
    //! The given stat did not exist in the given generation
    /*!
     * The "Special Attack" and "Special Defense" stat was introduced in Generation II,
     * so passing these values into a function corresponding to Generation I will
     * produce this error.
     */
    PKSAV_ERROR_INVALID_STAT,
    //! The EV passed into the function was outside the valid range
    /*!
     * For Generations I-II, this range is 0-65535. In later generations, this
     * range is 0-255.
     */
    PKSAV_ERROR_INVALID_EV,
    //! The IV passed into the function was outside the valid range
    /*!
     * For Generations I-II, this range is 0-15. In later generations, this
     * range is 0-31.
     */
    PKSAV_ERROR_INVALID_IV,
    //! The filepath passed in the function is not a valid save file for the given generation.
    PKSAV_ERROR_INVALID_SAVE
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
