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

//! Native storage for a Pokémon trainer's in-game ID (Generation III+).
typedef union {
    //! Convenience struct to allow individual access to secret and public IDs.
    struct {
        /*!
         * @brief Secret ID.
         *
         * This value is never shown in-game and exists to prevent
         * ID collision, as many more than 65536 people play these games.
         *
         * This value is stored in little-endian and should be accessed and
         * modified with ::pksav_littleendian16.
         */
        uint16_t sid;
        /*!
         * @brief Public ID.
         *
         * This is the value shown in-game.
         *
         * This value is stored in little-endian and should be accessed and
         * modified with ::pksav_littleendian16.
         */
        uint16_t pid;
    };
    /*!
     * @brief Full ID.
     *
     * This value is never shown in-game.
     *
     * This value is stored in little-endian and should be accessed and
     * modified with ::pksav_littleendian32.
     */
    uint32_t id;
} pksav_trainer_id_t;

#pragma pack(pop)

#endif /* PKSAV_COMMON_TRAINER_ID_H */
