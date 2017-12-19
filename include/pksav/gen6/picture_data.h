/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_PICTURE_DATA_H
#define PKSAV_GEN6_PICTURE_DATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint16_t title[12];
    uint16_t null_terminator;

    uint8_t unused[58];

    uint8_t jpeg_data[0xE004];
} pksav_gen6_picture_data_t;

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN6_PICTURE_DATA_H */
