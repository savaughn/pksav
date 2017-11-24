/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GCN_SHA1_H
#define PKSAV_GCN_SHA1_H

#include <stdint.h>

typedef union {
    uint8_t u8[20];
    uint32_t u32[5];
} pksav_digest_t;

void pksav_sha1(
    const uint8_t* start,
    const uint8_t* end,
    pksav_digest_t* digest
);

#endif /* PKSAV_GCN_SHA1_H */
