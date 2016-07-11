/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_CHECKSUM_H
#define PKSAV_CHECKSUM_H

#include <pksav/config.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint16_t first;
    uint16_t second;
} pksav_gen2_checksums_t;

PKSAV_API void pksav_get_gen2_save_checksums(
    bool crystal,
    const uint8_t* data,
    pksav_gen2_checksums_t* checksums_out
);

PKSAV_API void pksav_set_gen2_save_checksums(
    bool crystal,
    uint8_t* data
);

// TODO: Gen III

PKSAV_API uint16_t pksav_get_nds_block_checksum(
    uint8_t* data,
    size_t len
);

#endif /* PKSAV_CHECKSUM_H */
