/*!
 * @file    pksav/gen1/badges.h
 * @ingroup PKSav
 * @brief   Enum for getting and setting badges in Generation I games.
 *
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_BADGES_H
#define PKSAV_GEN1_BADGES_H

/*!
 * @brief Bitmasks for checking if the player has a specific badge.
 *
 * The pksav_gen1_save_t.badges field points to the byte in the save file
 * that stores this bitmap. Use these enum values on this byte to access and
 * set whether or not the player has this badge.
 */
typedef enum
{
    //! Earth Badge (Giovanni, Badge #8)
    PKSAV_GEN1_EARTH_BADGE    = 0x01,
    //! Volcano Badge (Blaine, Badge #7)
    PKSAV_GEN1_VOLCANO_BADGE  = 0x02,
    //! Marsh Badge (Sabrina, Badge #6)
    PKSAV_GEN1_MARSH_BADGE    = 0x04,
    //! Soul Badge (Koga, Badge #5)
    PKSAV_GEN1_SOUL_BADGE     = 0x08,
    //! Rainbow Badge (Erika, Badge #4)
    PKSAV_GEN1_RAINBOW_BADGE  = 0x10,
    //! Thunder Badge (Lt. Surge, Badge #3)
    PKSAV_GEN1_THUNDER_BADGE  = 0x20,
    //! Cascade Badge (Misty, Badge #2)
    PKSAV_GEN1_CASCADE_BADGE  = 0x40,
    //! Boulder Badge (Brock, Badge #1)
    PKSAV_GEN1_BOULDER_BADGE  = 0x80
} pksav_gen1_badge_t;

#endif /* PKSAV_GEN1_BADGES_H */
