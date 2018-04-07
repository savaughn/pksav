/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_RIBBONS_H
#define PKSAV_GBA_RIBBONS_H

/*!
 * @brief What level of contest ribbon a given Pokémon has.
 *
 * For each contest type, there is a 3-bit slot in the Pokémon's Hoenn
 * ribbon field that stores which level of contest the Pokémon has one for
 * that particular type. This enum represents all valid values.
 */
enum pksav_gba_contest_ribbon_level
{
    //! No contest won.
    PKSAV_GBA_CONTEST_RIBBON_NONE   = 0,
    //! Normal contest.
    PKSAV_GBA_CONTEST_RIBBON_NORMAL = 1,
    //! Super/Great contest.
    PKSAV_GBA_CONTEST_RIBBON_SUPER  = 2,
    //! Hyper/Ultra contest.
    PKSAV_GBA_CONTEST_RIBBON_HYPER  = 3,
    //! Master contest.
    PKSAV_GBA_CONTEST_RIBBON_MASTER = 4
};

/*!
 * @brief The offsets for the 3-bit slots for each Pokémon contest type.
 */
enum pksav_gba_contest_ribbons_offset
{
    //! The offset for the level of Cool contests won.
    PKSAV_GBA_COOL_RIBBONS_OFFSET   = 0,
    //! The offset for the level of Beauty contests won.
    PKSAV_GBA_BEAUTY_RIBBONS_OFFSET = 3,
    //! The offset for the level of Cute contests won.
    PKSAV_GBA_CUTE_RIBBONS_OFFSET   = 6,
    //! The offset for the level of Smart contests won.
    PKSAV_GBA_SMART_RIBBONS_OFFSET  = 9,
    //! The offset for the level of Tough contests won.
    PKSAV_GBA_TOUGH_RIBBONS_OFFSET  = 12,
};

enum pksav_gba_contest_ribbons_mask
{
    //! The mask for the level of Cool contests won.
    PKSAV_GBA_COOL_RIBBONS_MASK   = (7 << PKSAV_GBA_COOL_RIBBONS_OFFSET),
    //! The mask for the level of Beauty contests won.
    PKSAV_GBA_BEAUTY_RIBBONS_MASK = (7 << PKSAV_GBA_BEAUTY_RIBBONS_OFFSET),
    //! The mask for the level of Cute contests won.
    PKSAV_GBA_CUTE_RIBBONS_MASK   = (7 << PKSAV_GBA_CUTE_RIBBONS_OFFSET),
    //! The mask for the level of Smart contests won.
    PKSAV_GBA_SMART_RIBBONS_MASK  = (7 << PKSAV_GBA_SMART_RIBBONS_OFFSET),
    //! The mask for the level of Tough contests won.
    PKSAV_GBA_TOUGH_RIBBONS_MASK  = (7 << PKSAV_GBA_TOUGH_RIBBONS_OFFSET)
};

#define PKSAV_GBA_COOL_CONTEST_LEVEL(field) \
    (((field) & PKSAV_GBA_COOL_RIBBONS_MASK) >> PKSAV_GBA_COOL_RIBBONS_OFFSET)

#define PKSAV_GBA_BEAUTY_CONTEST_LEVEL(field) \
    (((field) & PKSAV_GBA_BEAUTY_RIBBONS_MASK) >> PKSAV_GBA_BEAUTY_RIBBONS_OFFSET)

#define PKSAV_GBA_CUTE_CONTEST_LEVEL(field) \
    (((field) & PKSAV_GBA_CUTE_RIBBONS_MASK) >> PKSAV_GBA_CUTE_RIBBONS_OFFSET)

#define PKSAV_GBA_SMART_CONTEST_LEVEL(field) \
    (((field) & PKSAV_GBA_SMART_RIBBONS_MASK) >> PKSAV_GBA_SMART_RIBBONS_OFFSET)

#define PKSAV_GBA_TOUGH_CONTEST_LEVEL(field) \
    (((field) & PKSAV_GBA_TOUGH_RIBBONS_MASK) >> PKSAV_GBA_TOUGH_RIBBONS_OFFSET)

enum pksav_gba_ribbon_mask
{
    //! Champion Ribbon.
    PKSAV_GBA_CHAMPION_RIBBON_MASK = (1 << 15),
    //! Winning Ribbon.
    PKSAV_GBA_WINNING_RIBBON_MASK  = (1 << 16),
    //! Victory Ribbon.
    PKSAV_GBA_VICTORY_RIBBON_MASK  = (1 << 17),
    //! Artist Ribbon.
    PKSAV_GBA_ARTIST_RIBBON_MASK   = (1 << 18),
    //! Effort Ribbon.
    PKSAV_GBA_EFFORT_RIBBON_MASK   = (1 << 19),
    //! Marine Ribbon.
    PKSAV_GBA_MARINE_RIBBON_MASK   = (1 << 20),
    //! Land Ribbon.
    PKSAV_GBA_LAND_RIBBON_MASK     = (1 << 21),
    //! Sky Ribbon.
    PKSAV_GBA_SKY_RIBBON_MASK      = (1 << 22),
    //! Country Ribbon.
    PKSAV_GBA_COUNTRY_RIBBON_MASK  = (1 << 23),
    //! National Ribbon.
    PKSAV_GBA_NATIONAL_RIBBON_MASK = (1 << 24),
    //! Earth Ribbon.
    PKSAV_GBA_EARTH_RIBBON_MASK    = (1 << 25),
    //! World Ribbon.
    PKSAV_GBA_WORLD_RIBBON_MASK    = (1 << 26)
};

#endif /* PKSAV_GBA_RIBBONS_H */
