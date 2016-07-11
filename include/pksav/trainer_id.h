/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_TRAINER_ID_H
#define PKSAV_TRAINER_ID_H

#include <stdint.h>

#pragma pack(push,1)
typedef union {
    struct {
        uint16_t sid;
        uint16_t pid;
    };
    uint32_t id;
} pksav_trainer_id_t;
#pragma pack(pop)
