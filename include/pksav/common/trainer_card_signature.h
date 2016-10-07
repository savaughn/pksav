/*!
 * @file    pksav/common/trainer_signature.h
 * @ingroup PKSav
 * @brief   Native storage for a Pokémon trainer's signature on a Trainer Card.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_TRAINER_CARD_SIGNATURE_H
#define PKSAV_COMMON_TRAINER_CARD_SIGNATURE_H

#include <stdint.h>

#pragma pack(push,1)

/*!
 * @brief Native representation of 8x8 pixels in a Trainer Card's signature.
 *
 * "Within each of those 8 byte structures, each byte represents a row within
 * the pixel group, the least significant byte being the topmost row. Each
 * bit within each byte represents a pixel (1 = Black, 0 = White) in that row,
 * with the least significant bit representing the leftmost pixel."
 *
 * <a href="https://projectpokemon.org/wiki/Trainer_Card_Signature">Source</a>: Project Pokémon
 */
typedef struct {
    //! All 8 rows in the group, with each index corresponding to an 8-pixel row.
    uint8_t rows[8];
} pksav_pixel_group_t;

//! A row of 8x8 pixel groups in a trainer card.
typedef struct {
    //! 8x8 pixel groups.
    pksav_pixel_group_t row[14];
} pksav_pixel_group_row_t;

//! Native representation of a Trainer Card's signature, parsable by rows.
typedef struct {
    //! All entries in a Trainer Card, parseable in 8-row groups.
    pksav_pixel_group_row_t rows[8];
} pksav_trainer_card_signature_t;

#pragma pack(pop)

#endif /* PKSAV_COMMON_TRAINER_CARD_SIGNATURE_H */
