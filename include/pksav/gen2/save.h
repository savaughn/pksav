/*!
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_SAVE_H
#define PKSAV_GEN2_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gen2/items.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/time.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PKSAV_GEN2_SAVE_SIZE 0x8000
#define PKSAV_GEN2_NUM_POKEMON_BOXES 14

enum pksav_gen2_save_type
{
    PKSAV_GEN2_SAVE_TYPE_NONE,
    PKSAV_GEN2_SAVE_TYPE_GS,
    PKSAV_GEN2_SAVE_TYPE_CRYSTAL
};

struct pksav_gen2_pokedex_lists
{
    /*!
     * @brief A pointer to the list of Pokémon seen by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     */
    uint8_t* seen_ptr;

    /*!
     * @brief A pointer to the list of Pokémon owned by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     */
    uint8_t* owned_ptr;
};

struct pksav_gen2_pokemon_storage
{
    struct pksav_gen2_pokemon_party* pokemon_party_ptr;

    /*!
     * @brief Pointers to the trainer's Pokémon boxes.
     *
     * The boxes are not stored contiguously in the save file, so these pointers
     * point to their actual positions in the file.
     */
    struct pksav_gen2_pokemon_box* pokemon_box_ptrs[PKSAV_GEN2_NUM_POKEMON_BOXES];

    struct pksav_gen2_pokemon_box_names* box_names_ptr;

    /*!
     * @brief The number of the current Pokémon box (0-11).
     *
     * There are 12 Pokémon boxes, and this value (0-based) shows which one will be
     * deposited and withdrawn from.
     *
     * The ::PKSAV_GEN2_current_box_NUM_MASK should be used to access or set
     * this value.
     */
    uint8_t* current_box_num_ptr;

    /*!
     * @brief A pointer to the current Pokémon box.
     *
     * Pokémon are only deposited or withdrawn from this box, and its data is
     * switched out when the current box is changed.
     */
    struct pksav_gen2_pokemon_box* current_box_ptr;
};

struct pksav_gen2_item_storage
{
    //! A pointer to the trainer's item bag.
    struct pksav_gen2_item_bag* item_bag_ptr;

    //! A pointer to the trainer's item PC.
    struct pksav_gen2_item_pc* item_pc_ptr;
};

struct pksav_gen2_trainer_info
{
    /*!
     * @brief A pointer to the trainer's ID (stored in big-endian).
     *
     * This value should be accessed and modified with ::pksav_bigendian16.
     */
    uint16_t* id_ptr;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gen2 with a num_chars
     * value of 7.
     *
     * This value should be set with ::pksav_text_to_gen2 with a num_chars
     * value of 7.
     */
    uint8_t* name_ptr;

    uint8_t* gender_ptr;

    uint8_t* palette_ptr;

    /*!
     * @brief A pointer to how much money the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_from_bcd, with a num_bytes value
     * of 3. It should be set with ::pksav_to_bcd, with a maximum value of 999999.
     */
    uint8_t* money_ptr;

    /*!
     * @brief A pointer to the list of Johto badges the trainer has earned.
     *
     * This value should be manipulated with the bitmasks given in the
     * ::pksav_gen2_johto_badge_t enum.
     */
    uint8_t* johto_badges_ptr;

    /*!
     * @brief A pointer to the list of Kanto badges the trainer has earned.
     *
     * This value should be manipulated with the bitmasks given in the
     * ::pksav_gen2_kanto_badge_t enum.
     */
    uint8_t* kanto_badges_ptr;
};

struct pksav_gen2_misc_fields
{
    /*!
     * @brief A pointer to the rival's name.
     *
     * This value should be accessed with ::pksav_text_from_gen1 with a num_chars
     * value of 7.
     *
     * This value should be set with ::pksav_text_to_gen1 with a num_chars
     * value of 7.
     */
    uint8_t* rival_name_ptr;
};

struct pksav_gen2_save_internal
{
    uint8_t* raw_save_ptr;

    uint8_t* checksum1_ptr;
    uint8_t* checksum2_ptr;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_SAVE_H */
