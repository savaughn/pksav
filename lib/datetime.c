/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/datetime.h>

int pksav_time_t_to_date_t(
    const time_t* time_in,
    struct pksav_date_t* date_out
) {
    struct tm* gmtm = gmtime(time_in);
    if(gmtm->tm_year < 2000) {
        return 1;
    }

    date_out->year  = (uint8_t)gmtm->tm_year+10;
    date_out->month = (uint8_t)gmtm->tm_mon+1;
    date_out->day   = (uint8_t)gmtm->tm_mday;
    return 0;
}
