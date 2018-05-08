/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_TYPE_H
#define PKSAV_GEN1_TYPE_H

/*!
 * @brief Valid values for a Pokémon's types.
 *
 * This enum applies to the indices of the pksav_gen1_pc_pokemon_t.types field.
 */
enum pksav_gen1_type
{
    //! Normal.
    PKSAV_GEN1_TYPE_NORMAL   = 0x00,
    //! Fighting.
    PKSAV_GEN1_TYPE_FIGHTING = 0x01,
    //! Flying.
    PKSAV_GEN1_TYPE_FLYING   = 0x02,
    //! Poison.
    PKSAV_GEN1_TYPE_POISON   = 0x03,
    //! Ground.
    PKSAV_GEN1_TYPE_GROUND   = 0x04,
    //! Rock.
    PKSAV_GEN1_TYPE_ROCK     = 0x05,
    //! Bug.
    PKSAV_GEN1_TYPE_BUG      = 0x07,
    //! Ghost.
    PKSAV_GEN1_TYPE_GHOST    = 0x08,
    //! Fire.
    PKSAV_GEN1_TYPE_FIRE     = 0x14,
    //! Water.
    PKSAV_GEN1_TYPE_WATER    = 0x15,
    //! Grass.
    PKSAV_GEN1_TYPE_GRASS    = 0x16,
    //! Electric.
    PKSAV_GEN1_TYPE_ELECTRIC = 0x17,
    //! Psychic.
    PKSAV_GEN1_TYPE_PSYCHIC  = 0x18,
    //! Ice.
    PKSAV_GEN1_TYPE_ICE      = 0x19,
    //! Dragon.
    PKSAV_GEN1_TYPE_DRAGON   = 0x1A
};

#endif /* PKSAV_GEN1_TYPE_H */
