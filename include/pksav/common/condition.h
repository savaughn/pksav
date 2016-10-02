/*!
 * @file    pksav/common/condition.h
 * @ingroup PKSav
 * @brief   Possible values for how a Pokémon was caught in Generation IV.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_CONDITION_H
#define PKSAV_COMMON_CONDITION_H

//! Valid values (with one mask) for a Pokémon's possible conditions.
typedef enum {
    //! No status ailment.
    PKSAV_CONDITION_NONE       = 0,
    /*!
     * @brief Sleep mask.
     *
     * This condition differs from the others in that this field stores how
     * many turns (0-7) are left for the Pokémon to sleep instead of the
     * Pokémon simply having the condition.
     */
    PKSAV_CONDITION_SLEEP_MASK = 0x7,
    //! Poison.
    PKSAV_CONDITION_POISON     = (1 << 3),
    //! Burn.
    PKSAV_CONDITION_BURN       = (1 << 4),
    //! Frozen.
    PKSAV_CONDITION_FROZEN     = (1 << 5),
    //! Paralysis.
    PKSAV_CONDITION_PARALYZ    = (1 << 6),
    //! Bad poison (from Toxic, etc).
    PKSAV_CONDITION_BAD_POISON = (1 << 7)
} pksav_condition_mask_t;

#endif /* PKSAV_COMMON_CONDITION_H */
