/*!
 * @file    pksav/gen2/pokemon.h
 * @ingroup PKSav
 * @brief   Native formats and convenience functions for Pokémon in Generation IIII games.
 *
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_POKEMON_H
#define PKSAV_GEN2_POKEMON_H

#include <stdint.h>

/*!
 * @brief The mask for a move's PP in the PP field.
 *
 * Mask the value of one of the indices of pksav_gen2_pc_pokemon_t.move_pps to
 * get the PP of that move.
 */
#define PKSAV_GEN2_MOVE_PP_MASK ((uint8_t)0x3F)

/*!
 * @brief The mask for the number of PP Ups used on a move (0-3).
 *
 * Mask the value of one of the indices of pksav_gen2_pc_pokemon_t.move_pps to
 * get the number of PP Ups used. If a PP Max has been used, this value will be 3.
 */
#define PKSAV_GEN2_MOVE_PP_UP_MASK ((uint8_t)0xC0)

#define GEN2_TIME_OF_DAY_MASK    ((uint16_t)0xC000)
#define GEN2_TIME_OF_DAY_OFFSET  14

#define GEN2_LEVEL_CAUGHT_MASK   ((uint16_t)0x3F00)
#define GEN2_LEVEL_CAUGHT_OFFSET 8

#define GEN2_OT_GENDER_MASK      ((uint16_t)0x80)

#define GEN2_LOCATION_MASK       ((uint16_t)0x7F)

#pragma pack(push,1)

/*!
 * @brief Native format for the persistent data of a Pokémon in Generation IIII.
 *
 * This data is available both when the Pokémon is in the trainer's party or in the
 * PC.
 */
typedef struct {
    uint8_t species;
    uint8_t held_item;
    uint8_t moves[4];
    uint16_t ot_id;
    uint8_t exp[3];
    uint16_t ev_hp;
    uint16_t ev_atk;
    uint16_t ev_def;
    uint16_t ev_spd;
    uint16_t ev_spcl;
    uint16_t iv_data;
    uint8_t move_pps[4];
    uint8_t friendship;
    uint8_t pokerus;
    uint16_t caught_data;
    uint8_t level;
} pksav_gen2_pc_pokemon_t;

/*!
 * @brief Data generated when a Pokémon is added to a trainer's party in Generation III.
 *
 * All of this information is generated from values stored in
 * pksav_gen2_pc_pokemon_t.
 */
typedef struct {
    uint8_t status;
    uint8_t unused;
    uint16_t current_hp;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spatk;
    uint16_t spdef;
} pksav_gen2_pokemon_party_data_t;

//! Native format for a Pokémon in the trainer's party in Generation III.
typedef struct {
    /*!
     * @brief PC data.
     *
     * This data is accessible whether the Pokémon is in the PC or party.
     */
    pksav_gen2_pc_pokemon_t pc;

    /*!
     * @brief Party data.
     *
     * This data is generated when the Pokémon is moved into the party. As
     * all of it can be generated from the PC data, it is not stored until
     * it needs to be used.
     */
    pksav_gen2_pokemon_party_data_t party_data;
} pksav_gen2_party_pokemon_t;

//! Native format for a trainer's Pokémon party in Generation II.
typedef struct {
    //! The actual number of Pokémon in the party (0-6).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the party.
     *
     * When the party is viewed in-game, it is this value that determines
     * what Pokémon is shown, not the pksav_gen2_pc_pokemon_t.species value.
     *
     * If this field is set to 0xFD, the Pokémon will be in an egg. Its field
     * in pksav_gen2_pc_pokemon_t will correspond to its species when hatched.
     *
     * The final index of this field should always be set to 0xFF.
     */
    uint8_t species[7];
    //! The actual Pokémon in the party.
    pksav_gen2_party_pokemon_t party[6];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen2
     * or ::pksav_widetext_from_gen2 (with a num_chars value of 10).
     */
    uint8_t otnames[6][11];
    /*!
     * @brief The nicknames of each Pokémon in the party.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen2
     * or ::pksav_widetext_from_gen2 (with a num_chars value of 10).
     */
    uint8_t nicknames[6][11];
} pksav_gen2_pokemon_party_t;

//! Native format for a Pokémon PC box in Generation II.
typedef struct {
    //! The actual number of Pokémon in the box (0-20).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the box.
     *
     * When the box is viewed in-game, it is this value that determines
     * what Pokémon is showed, not the pksav_gen2_pc_pokemon_t.species value.
     *
     * If this field is set to 0xFD, the Pokémon will be in an egg. Its field
     * in pksav_gen2_pc_pokemon_t will correspond to its species when hatched.
     *
     * The final index of this field should always be set to 0xFF.
     */
    uint8_t species[21];
    //! The actual Pokémon in the box.
    pksav_gen2_pc_pokemon_t entries[20];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen2
     * or ::pksav_widetext_from_gen2 (with a num_chars value of 10).
     */
    uint8_t otnames[20][11];
    /*!
     * @brief The nicknames of each Pokémon in the box.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen2
     * or ::pksav_widetext_from_gen2 (with a num_chars value of 10).
     */
    uint8_t nicknames[20][11];
    //! Padding.
    uint8_t padding[2];
} pksav_gen2_pokemon_box_t;

typedef struct {
    pksav_gen2_pokemon_box_t boxes[14];
} pksav_gen2_pokemon_pc_t;

typedef struct {
    uint8_t names[14][9];
} pksav_gen2_pokemon_box_names_t;

#pragma pack(pop)

#endif /* PKSAV_GEN2_POKEMON_H */
