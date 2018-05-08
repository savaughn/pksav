/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_BADGES_H
#define PKSAV_GEN2_BADGES_H

// TODO: verify

enum pksav_gen2_johto_badge_mask
{
    //! Zephyr Badge (Falkner, Badge #1)
    PKSAV_GEN2_JOHTO_ZEPHYR_BADGE   = 0x01,
    //! Hive Badge (Bugsy, Badge #2)
    PKSAV_GEN2_JOHTO_HIVE_BADGE     = 0x02,
    //! Plain Badge (Whitney, Badge #3)
    PKSAV_GEN2_JOHTO_PLAIN_BADGE    = 0x04,
    //! Fog Badge (Morty, Badge #4)
    PKSAV_GEN2_JOHTO_FOG_BADGE      = 0x08,
    //! Soul Badge (Chuck, Badge #5)
    PKSAV_GEN2_JOHTO_STORM_BADGE    = 0x10,
    //! Marsh Badge (Jasmine, Badge #6)
    PKSAV_GEN2_JOHTO_MINERAL_BADGE  = 0x20,
    //! Volcano Badge (Pryce, Badge #7)
    PKSAV_GEN2_JOHTO_GLACIER_BADGE  = 0x40,
    //! Earth Badge (Clair, Badge #8)
    PKSAV_GEN2_JOHTO_RISING_BADGE   = 0x80
};

enum pksav_gen2_kanto_badge_mask
{
    //! Boulder Badge (Brock, Badge #1)
    PKSAV_GEN2_KANTO_BOULDER_BADGE  = 0x01,
    //! Cascade Badge (Misty, Badge #2)
    PKSAV_GEN2_KANTO_CASCADE_BADGE  = 0x02,
    //! Thunder Badge (Lt. Surge, Badge #3)
    PKSAV_GEN2_KANTO_THUNDER_BADGE  = 0x04,
    //! Rainbow Badge (Erika, Badge #4)
    PKSAV_GEN2_KANTO_RAINBOW_BADGE  = 0x08,
    //! Soul Badge (Janine, Badge #5)
    PKSAV_GEN2_KANTO_SOUL_BADGE     = 0x10,
    //! Marsh Badge (Sabrina, Badge #6)
    PKSAV_GEN2_KANTO_MARSH_BADGE    = 0x20,
    //! Volcano Badge (Blaine, Badge #7)
    PKSAV_GEN2_KANTO_VOLCANO_BADGE  = 0x40,
    //! Earth Badge (Blue, Badge #8)
    PKSAV_GEN2_KANTO_EARTH_BADGE    = 0x80
};

#endif /* PKSAV_GEN2_BADGES_H */
