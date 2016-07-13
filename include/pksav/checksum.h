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

PKSAV_API uint16_t pksav_get_nds_block_checksum(
    uint8_t* data,
    size_t len
);

#endif /* PKSAV_CHECKSUM_H */
