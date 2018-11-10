/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_COMMON_H
#define PKSAV_GEN1_COMMON_H

#include <pksav/common/constants.h>

#define PKSAV_GEN1_TRAINER_NAME_LENGTH PKSAV_STANDARD_TRAINER_NAME_LENGTH

#define PKSAV_GEN1_NUM_POKEMON_BOXES (12)

enum pksav_gen1_save_type
{
    PKSAV_GEN1_SAVE_TYPE_NONE,
    PKSAV_GEN1_SAVE_TYPE_RED_BLUE,
    PKSAV_GEN1_SAVE_TYPE_YELLOW
};

#endif /* PKSAV_GEN1_COMMON_H */
