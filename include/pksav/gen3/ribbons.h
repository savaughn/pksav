/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef pksav_gen3_RIBBONS_H
#define pksav_gen3_RIBBONS_H

/*!
 * @brief What level of contest ribbon a given Pokémon has.
 *
 * For each contest type, there is a 3-bit slot in the Pokémon's Hoenn
 * ribbon field that stores which level of contest the Pokémon has one for
 * that particular type. This enum represents all valid values.
 */
enum pksav_gen3_contest_ribbon_level
{
    //! No contest won.
    pksav_gen3_CONTEST_RIBBON_NONE   = 0,
    //! Normal contest.
    pksav_gen3_CONTEST_RIBBON_NORMAL = 1,
    //! Super/Great contest.
    pksav_gen3_CONTEST_RIBBON_SUPER  = 2,
    //! Hyper/Ultra contest.
    pksav_gen3_CONTEST_RIBBON_HYPER  = 3,
    //! Master contest.
    pksav_gen3_CONTEST_RIBBON_MASTER = 4
};

/*!
 * @brief The offsets for the 3-bit slots for each Pokémon contest type.
 */
enum pksav_gen3_contest_ribbons_offset
{
    //! The offset for the level of Cool contests won.
    pksav_gen3_COOL_RIBBONS_OFFSET   = 0,
    //! The offset for the level of Beauty contests won.
    pksav_gen3_BEAUTY_RIBBONS_OFFSET = 3,
    //! The offset for the level of Cute contests won.
    pksav_gen3_CUTE_RIBBONS_OFFSET   = 6,
    //! The offset for the level of Smart contests won.
    pksav_gen3_SMART_RIBBONS_OFFSET  = 9,
    //! The offset for the level of Tough contests won.
    pksav_gen3_TOUGH_RIBBONS_OFFSET  = 12,
};

enum pksav_gen3_contest_ribbons_mask
{
    //! The mask for the level of Cool contests won.
    pksav_gen3_COOL_RIBBONS_MASK   = (7 << pksav_gen3_COOL_RIBBONS_OFFSET),
    //! The mask for the level of Beauty contests won.
    pksav_gen3_BEAUTY_RIBBONS_MASK = (7 << pksav_gen3_BEAUTY_RIBBONS_OFFSET),
    //! The mask for the level of Cute contests won.
    pksav_gen3_CUTE_RIBBONS_MASK   = (7 << pksav_gen3_CUTE_RIBBONS_OFFSET),
    //! The mask for the level of Smart contests won.
    pksav_gen3_SMART_RIBBONS_MASK  = (7 << pksav_gen3_SMART_RIBBONS_OFFSET),
    //! The mask for the level of Tough contests won.
    pksav_gen3_TOUGH_RIBBONS_MASK  = (7 << pksav_gen3_TOUGH_RIBBONS_OFFSET)
};

#define pksav_gen3_COOL_CONTEST_LEVEL(field) \
    (((field) & pksav_gen3_COOL_RIBBONS_MASK) >> pksav_gen3_COOL_RIBBONS_OFFSET)

#define pksav_gen3_BEAUTY_CONTEST_LEVEL(field) \
    (((field) & pksav_gen3_BEAUTY_RIBBONS_MASK) >> pksav_gen3_BEAUTY_RIBBONS_OFFSET)

#define pksav_gen3_CUTE_CONTEST_LEVEL(field) \
    (((field) & pksav_gen3_CUTE_RIBBONS_MASK) >> pksav_gen3_CUTE_RIBBONS_OFFSET)

#define pksav_gen3_SMART_CONTEST_LEVEL(field) \
    (((field) & pksav_gen3_SMART_RIBBONS_MASK) >> pksav_gen3_SMART_RIBBONS_OFFSET)

#define pksav_gen3_TOUGH_CONTEST_LEVEL(field) \
    (((field) & pksav_gen3_TOUGH_RIBBONS_MASK) >> pksav_gen3_TOUGH_RIBBONS_OFFSET)

enum pksav_gen3_ribbon_mask
{
    //! Champion Ribbon.
    pksav_gen3_CHAMPION_RIBBON_MASK = (1 << 15),
    //! Winning Ribbon.
    pksav_gen3_WINNING_RIBBON_MASK  = (1 << 16),
    //! Victory Ribbon.
    pksav_gen3_VICTORY_RIBBON_MASK  = (1 << 17),
    //! Artist Ribbon.
    pksav_gen3_ARTIST_RIBBON_MASK   = (1 << 18),
    //! Effort Ribbon.
    pksav_gen3_EFFORT_RIBBON_MASK   = (1 << 19),
    //! Marine Ribbon.
    pksav_gen3_MARINE_RIBBON_MASK   = (1 << 20),
    //! Land Ribbon.
    pksav_gen3_LAND_RIBBON_MASK     = (1 << 21),
    //! Sky Ribbon.
    pksav_gen3_SKY_RIBBON_MASK      = (1 << 22),
    //! Country Ribbon.
    pksav_gen3_COUNTRY_RIBBON_MASK  = (1 << 23),
    //! National Ribbon.
    pksav_gen3_NATIONAL_RIBBON_MASK = (1 << 24),
    //! Earth Ribbon.
    pksav_gen3_EARTH_RIBBON_MASK    = (1 << 25),
    //! World Ribbon.
    pksav_gen3_WORLD_RIBBON_MASK    = (1 << 26)
};

#endif /* pksav_gen3_RIBBONS_H */
