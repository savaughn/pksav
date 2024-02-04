/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef pksav_gen3_COMMON_H
#define pksav_gen3_COMMON_H

#include <pksav/common/constants.h>

#define pksav_gen3_TRAINER_NAME_LENGTH PKSAV_STANDARD_TRAINER_NAME_LENGTH

enum pksav_gen3_save_type
{
    pksav_gen3_SAVE_TYPE_NONE,
    pksav_gen3_SAVE_TYPE_RS,
    pksav_gen3_SAVE_TYPE_EMERALD,
    pksav_gen3_SAVE_TYPE_FRLG
};

#endif /* pksav_gen3_COMMON_H */
