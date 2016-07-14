/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/datetime.h>

pksav_error_t pksav_time_t_to_date_t(
    const time_t* time_in,
    pksav_date_t* date_out
) {
    struct tm* gmtm = gmtime(time_in);
    if(gmtm->tm_year < 2000) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    date_out->year  = (uint8_t)gmtm->tm_year+10;
    date_out->month = (uint8_t)gmtm->tm_mon+1;
    date_out->day   = (uint8_t)gmtm->tm_mday;

    return PKSAV_ERROR_NONE;
}
