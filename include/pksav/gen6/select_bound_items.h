/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_SELECT_BOUND_ITEMS_H
#define PKSAV_GEN6_SELECT_BOUND_ITEMS_H

#include <stdint.h>

#pragma pack(push, 1)

typedef struct
{
    uint16_t up_index;
    uint16_t right_index;
    uint16_t down_index;
    uint16_t left_index;
} pksav_gen6_select_bound_items_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_SELECT_BOUND_ITEMS_H */
