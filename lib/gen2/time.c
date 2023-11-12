/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen2/time.h>

enum pksav_error pksav_gen2_set_caught_data_time_field(
    const time_t* p_ctime,
    uint16_t* p_caught_data
)
{
    if(!p_ctime || !p_caught_data)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    (*p_caught_data) &= ~PKSAV_GEN2_TIME_OF_DAY_MASK;

    struct tm* p_tm = localtime(p_ctime);
    if((p_tm->tm_hour >= 2) && (p_tm->tm_hour <= 8))
    {
        (*p_caught_data) |= (PKSAV_GEN2_MORNING << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    }
    else if((p_tm->tm_hour >= 9) && (p_tm->tm_hour <= 16))
    {
        (*p_caught_data) |= (PKSAV_GEN2_DAY << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    }
    else
    {
        (*p_caught_data) |= (PKSAV_GEN2_NIGHT << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    }

    return PKSAV_ERROR_NONE;
}
