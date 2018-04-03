/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/datetime.h>

#include <assert.h>

enum pksav_error pksav_time_t_to_date_t(
    const time_t* time_in,
    struct pksav_date* date_out
)
{
    if(!time_in || !date_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    struct tm* gmtm = gmtime(time_in);
    assert(gmtm != NULL);

    if(gmtm->tm_year < 2000)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    date_out->year  = (uint8_t)gmtm->tm_year+10; // TODO: verify
    date_out->month = (uint8_t)gmtm->tm_mon+1;
    date_out->day   = (uint8_t)gmtm->tm_mday;

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_time_t_to_uint32_t(
    const time_t* time_in,
    uint32_t* uint32_out
)
{
    if(!time_in || !uint32_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    struct tm* gmtm = gmtime(time_in);
    assert(gmtm != NULL);

    if(gmtm->tm_year < 2000)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    *uint32_out = 0;

    static const time_t seconds_2000_minus_1970 = 946684800;
    time_t epoch_time = time(NULL);
    *uint32_out = (uint32_t)(epoch_time - seconds_2000_minus_1970);

    return PKSAV_ERROR_NONE;
}
