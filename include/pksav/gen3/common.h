/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN3_COMMON_H
#define PKSAV_GEN3_COMMON_H

#include <pksav/common/constants.h>

#define PKSAV_GEN3_TRAINER_NAME_LENGTH PKSAV_STANDARD_TRAINER_NAME_LENGTH

enum pksav_gen3_save_type
{
    PKSAV_GEN3_SAVE_TYPE_NONE,
    PKSAV_GEN3_SAVE_TYPE_RS,
    PKSAV_GEN3_SAVE_TYPE_EMERALD,
    PKSAV_GEN3_SAVE_TYPE_FRLG
};

#endif /* PKSAV_GEN3_COMMON_H */
