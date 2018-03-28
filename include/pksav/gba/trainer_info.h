/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_TRAINER_INFO_H
#define PKSAV_GBA_TRAINER_INFO_H

#include <pksav/common/trainer_id.h>

#include <stdint.h>

#pragma pack(push,1)

/*!
 * @brief The representation of trainer info in Game Boy Advance games.
 */
struct pksav_gba_trainer_info
{
    /*!
     * @brief The player's name.
     *
     * This name should be accessed with ::pksav_text_from_gba and modified
     * with ::pksav_text_to_gba.
     *
     * In either case, the num_chars parameter should be 7.
     */
    uint8_t name[7];
    //! Padding. No relevant data is stored here.
    uint8_t padding1;
    //! Trainer gender (0: male, 1: female).
    uint8_t gender;
    //! Padding. No relevant data is stored here.
    uint8_t padding2;
    //! Trainer ID.
    union pksav_trainer_id trainer_id;
};

#pragma pack(pop)

#endif /* PKSAV_GBA_TRAINER_INFO_H */
