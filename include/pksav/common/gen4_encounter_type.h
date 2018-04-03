/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_GEN4_ENCOUNTER_TYPE_H
#define PKSAV_COMMON_GEN4_ENCOUNTER_TYPE_H

/*!
 * @brief Pokémon encounter types in Generation IV.
 *
 * This field is only set in Generation IV but is present in
 * Generation V-VI Pokémon structs.
 */
enum pksav_gen4_encounter_type
{
    //! In-game event.
    PKSAV_GEN4_ENCOUNTER_EVENT         = 0x0,
    //! Found in tall grass.
    PKSAV_GEN4_ENCOUNTER_TALL_GRASS    = 0x2,
    //! In-game Dialga/Palkia encounter.
    PKSAV_GEN4_ENCOUNTER_DIALGA_PALKIA = 0x4,
    //! Found in a cave.
    PKSAV_GEN4_ENCOUNTER_CAVE          = 0x5,
    //! Found on the water.
    PKSAV_GEN4_ENCOUNTER_SURFING       = 0x7,
    //! Found in a building.
    PKSAV_GEN4_ENCOUNTER_BUILDING      = 0x9,
    //! Found in Sinnoh's Great Marsh.
    PKSAV_GEN4_ENCOUNTER_GREAT_MARSH   = 0xA,
    //! A starter Pokémon.
    PKSAV_GEN4_ENCOUNTER_STARTER       = 0xC
};

#endif /* PKSAV_COMMON_GEN4_ENCOUNTER_TYPE_H */
