/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef pksav_gen3_ROAMER_H
#define pksav_gen3_ROAMER_H

#include <stdint.h>

#pragma pack(push,1)

struct pksav_gen3_roamer
{
    uint32_t IVs;
    uint32_t personality;
    uint16_t species;
    uint16_t hp;
    uint8_t level;
    uint8_t status;
    uint8_t cool;
    uint8_t beauty;
    uint8_t cute;
    uint8_t smart;
    uint8_t tough;
    uint8_t is_active_bool;
    uint8_t filler[8];
};

#pragma pack(pop)

#endif /* pksav_gen3_ROAMER_H */
