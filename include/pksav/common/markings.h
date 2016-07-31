/*!
 * @file    pksav/common/markings.h
 * @ingroup PKSav
 * @brief   Native format for storing a Pokémon's markings.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_MARKINGS_H
#define PKSAV_COMMON_MARKINGS_H

/*!
 * @brief Masks for getting/setting Pokémon markings.
 *
 * Markings have no functional use and only serve to help the
 * trainer categorize their Pokémon.
 */
typedef enum {
    //! Circle mask.
    PKSAV_MARKING_CIRCLE   = 0x01,
    //! Triangle mask.
    PKSAV_MARKING_TRIANGLE = 0x02,
    //! Square mask.
    PKSAV_MARKING_SQUARE   = 0x04,
    //! Heart mask.
    PKSAV_MARKING_HEART    = 0x08,
    //! Star mask (Generation IV+).
    PKSAV_MARKING_STAR     = 0x10,
    //! Diamond mask (Generation IV+).
    PKSAV_MARKING_DIAMOND  = 0x20,
} pksav_marking_t;

#endif /* PKSAV_COMMON_MARKINGS_H */
