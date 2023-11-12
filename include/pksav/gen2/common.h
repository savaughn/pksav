/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_COMMON_H
#define PKSAV_GEN2_COMMON_H

#include <pksav/common/constants.h>

#define PKSAV_GEN2_TRAINER_NAME_LENGTH PKSAV_STANDARD_TRAINER_NAME_LENGTH

#define PKSAV_GEN2_NUM_POKEMON_BOXES (14)

enum pksav_gen2_save_type
{
    PKSAV_GEN2_SAVE_TYPE_NONE,
    PKSAV_GEN2_SAVE_TYPE_GS,
    PKSAV_GEN2_SAVE_TYPE_CRYSTAL
};

enum pksav_gen2_gender
{
    PKSAV_GEN2_GENDER_MALE,
    PKSAV_GEN2_GENDER_FEMALE
};

#endif /* PKSAV_GEN2_COMMON_H */
