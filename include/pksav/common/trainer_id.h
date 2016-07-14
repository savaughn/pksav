/*!
 * @file    pksav/common/trainer_id.h
 * @ingroup PKSav
 * @brief   Native storage for a Pokémon trainer's in-game ID.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_TRAINER_ID_H
#define PKSAV_COMMON_TRAINER_ID_H

#include <stdint.h>

#pragma pack(push,1)

//! Native storage for a Pokémon trainer's in-game ID.
typedef union {
    struct {
        //! Secret ID.
        uint16_t sid;
        //! Public ID.
        uint16_t pid;
    };
    //! Full ID.
    uint32_t id;
} pksav_trainer_id_t;

#pragma pack(pop)

#endif /* PKSAV_COMMON_TRAINER_ID_H */
