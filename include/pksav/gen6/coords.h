/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_COORDS_H
#define PKSAV_GEN6_COORDS_H

#include <stdint.h>

#pragma pack(push,1)

typedef struct
{
    uint16_t map_id;
    float x_coord;
    float z_coord;
    float y_coord;
} pksav_gen6_coords_t;

#pragma pack(pop)

#endif /* PKSAV_GEN6_COORDS_H */
