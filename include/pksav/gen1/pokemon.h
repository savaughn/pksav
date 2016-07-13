/*!
 * @file    pksav/gen1/pokemon.h
 * @ingroup PKSav
 * @brief   Native formats for Pokémon in Generation I games.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_POKEMON_H
#define PKSAV_GEN1_POKEMON_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/math/base256.h>

#include <stdint.h>

#pragma pack(push,1)

/*!
 * @brief Native format for the persistent data of a Pokémon in Generation I.
 *
 * This data is available both when the Pokémon is in the trainer's party or in the
 * PC.
 */
typedef struct {
    //! Species index.
    uint8_t species;
    /*!
     * @brief The Pokémon's current HP.
     *
     * This value should never be higher than pksav_gen1_pokemon_party_data_t.max_hp.
     */
    uint16_t current_hp;
    /*!
     * @brief The Pokémon's level.
     *
     * This value is also stored in pksav_gen1_pokemon_party_data_t.level.
     */
    uint8_t level;
    /*!
     * @brief The Pokémon's status ailments, if any.
     *
     * TODO: enum for status ailments
     */
    uint8_t status;
    /*!
     * @brief Indices for each of this Pokémon's types.
     *
     * If the Pokémon has a single type, then both of these values are equal to that
     * type's index.
     *
     * These fields have no real use in a save editing program and should have simply been
     * in a lookup table on the ROM itself.
     *
     * TODO: enum for type indices
     */
    uint8_t types[2];
    /*!
     * @brief The Pokémon's catch rate.
     *
     * This value has no real use in a save editing program and should have simply been
     * in a lookup table on the ROM itself.
     */
    uint8_t catch_rate;
    /*!
     * @brief Indices for each of this Pokémon's moves.
     */
    uint8_t moves[4];
    /*!
     * @brief The Pokémon's original trainer's ID (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ot_id;
    /*!
     * @brief The Pokémon's current total experience points (stored in Base-256).
     *
     * This value should be accessed with ::pksav_from_base256 (with a num_bytes
     * value of 3) and set with ::pksav_to_base256.
     */
    uint8_t exp[3];
    /*!
     * @brief The Pokémon's HP EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_hp;
    /*!
     * @brief The Pokémon's Attack EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_atk;
    /*!
     * @brief The Pokémon's Defense EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_def;
    /*!
     * @brief The Pokémon's Speed EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_spd;
    /*!
     * @brief The Pokémon's Special EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_spcl;
    /*!
     * @brief The Pokémon's IV's, stored as a bitset.
     *
     * These values should be accessed with ::pksav_get_gb_IV and set
     * with ::pksav_set_gb_IV.
     */
    uint16_t iv_data;
    /*!
     * @brief The Pokémon's PPs and number of PP Ups used for each move.
     *
     * TODO: add masks
     */
    uint8_t move_pps[4]; // TODO: PP, PP Up mask
} pksav_gen1_pc_pokemon_t;

/*!
 * @brief Data generated when a Pokémon is added to a trainer's party in Generation I.
 *
 * All of this information is generated from values stored in
 * pksav_gen1_pc_pokemon_t.
 */
typedef struct {
    /*!
     * @brief The Pokémon's level.
     *
     * This value is also stored in pksav_gen1_pc_pokemon_t.level.
     */
    uint8_t level;
    /*!
     * @brief The Pokémon's maximum HP.
     *
     * This value is calculated using the Pokémon's base HP stat, HP
     * EV, and HP IV.
     */
    uint16_t max_hp;
    /*!
     * @brief The Pokémon's Attack stat.
     *
     * This value is calculated using the Pokémon's base Attack stat,
     * Attack EV, and Attack IV.
     */
    uint16_t atk;
    /*!
     * @brief The Pokémon's Defense stat.
     *
     * This value is calculated using the Pokémon's base Defense stat,
     * Defense EV, and Defense IV.
     */
    uint16_t def;
    /*!
     * @brief The Pokémon's Speed stat.
     *
     * This value is calculated using the Pokémon's base Speed stat,
     * Speed EV, and Speed IV.
     */
    uint16_t spd;
    /*!
     * @brief The Pokémon's Special stat.
     *
     * This value is calculated using the Pokémon's base Special stat,
     * Special EV, and Special IV.
     */
    uint16_t spcl;
} pksav_gen1_pokemon_party_data_t;

//! Native format for a Pokémon in the trainer's party in Generation I.
typedef struct {
    pksav_gen1_pc_pokemon_t pc;
    pksav_gen1_pokemon_party_data_t party_data;
} pksav_gen1_party_pokemon_t;

//! Native format for a trainer's Pokémon party in Generation I.
typedef struct {
    //! The actual number of Pokémon in the party (1-6).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the party.
     *
     * When the party is viewed in-game, it is this value that determines
     * what Pokémon is showed, not the pksav_gen1_pc_pokemon_t.species value.
     *
     * The final index of this field should always be set to 0xFF.
     */
    uint8_t species[7];
    //! The actual Pokémon in the party.
    pksav_gen1_party_pokemon_t party[6];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen1
     * or ::pksav_widetext_from_gen1 (with a num_chars value of 10).
     */
    uint8_t otnames[6][11];
    /*!
     * @brief The nicknames of each Pokémon in the party.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen1
     * or ::pksav_widetext_from_gen1 (with a num_chars value of 10).
     */
    uint8_t nicknames[6][11];
} pksav_gen1_pokemon_party_t;

//! Native format for a Pokémon PC box in Generation I.
typedef struct {
    //! The actual number of Pokémon in the box (1-20).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the box.
     *
     * When the box is viewed in-game, it is this value that determines
     * what Pokémon is showed, not the pksav_gen1_pc_pokemon_t.species value.
     *
     * The final index of this field should always be set to 0xFF.
     */
    uint8_t species[21];
    //! The actual Pokémon in the box.
    pksav_gen1_pc_pokemon_t entries[20];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen1
     * or ::pksav_widetext_from_gen1 (with a num_chars value of 10).
     */
    uint8_t otnames[20][11];
    /*!
     * @brief The nicknames of each Pokémon in the box.
     *
     * To access this value, you should use the functions ::pksav_text_from_gen1
     * or ::pksav_widetext_from_gen1 (with a num_chars value of 10).
     */
    uint8_t nicknames[20][11];
} pksav_gen1_pokemon_box_t;

#pragma pack(pop)

#endif /* PKSAV_GEN1_POKEMON_H */
