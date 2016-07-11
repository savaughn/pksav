/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_CONTEST_STATS_H
#define PKSAV_COMMON_CONTEST_STATS_H

#include <stdint.h>

#pragma pack(push,1)
typedef struct {
    uint8_t cool;
    uint8_t beauty;
    uint8_t cute;
    uint8_t smart;
    uint8_t tough;
    union {
        uint8_t feel;
        uint8_t sheen;
    };
} pksav_contest_stats_t;
#pragma pack(pop)

#endif /* PKSAV_COMMON_CONTEST_STATS_H */
