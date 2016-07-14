/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_POKEDEX_H
#define PKSAV_COMMON_POKEDEX_H

#include <pksav/config.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Check if a given Pokémon has been seen/caught in the Pokédex's native representation.
 *
 * In a seen/caught buffer, each Pokémon is represented by a single bit. This
 * function calculates which bit that is based on the National Pokédex number and
 * checks it.
 *
 * \param raw Pokédex buffer
 * \param pokedex_num Which Pokémon to check
 * \returns if Pokémon with the given Pokédex number has been seen/caught
 */
PKSAV_API bool pksav_get_pokedex_bit(
    const uint8_t* raw,
    uint16_t pokedex_num
);

/*!
 * @brief Set whether or not a given Pokémon has been seen/caught.
 *
 * In a seen/caught buffer, each Pokémon is represented by a single bit. This
 * function calculates which bit that is based on the National Pokédex number and
 * checks it.
 *
 * \param raw Pokédex buffer
 * \param pokedex_num Which Pokémon to set or unset
 * \param set Set whether or not Pokémon has been seen/caught
 */
PKSAV_API void pksav_set_pokedex_bit(
    uint8_t* raw,
    uint16_t pokedex_num,
    bool set
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_POKEDEX_H */
