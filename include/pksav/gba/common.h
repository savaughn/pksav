/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_COMMON_H
#define PKSAV_GBA_COMMON_H

#include <pksav/common/constants.h>

#define PKSAV_GBA_TRAINER_NAME_LENGTH PKSAV_STANDARD_TRAINER_NAME_LENGTH

enum pksav_gba_save_type
{
    PKSAV_GBA_SAVE_TYPE_NONE,
    PKSAV_GBA_SAVE_TYPE_RS,
    PKSAV_GBA_SAVE_TYPE_EMERALD,
    PKSAV_GBA_SAVE_TYPE_FRLG
};

#endif /* PKSAV_GBA_COMMON_H */
