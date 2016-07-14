/*!
 * @file    pksav/gen1/pokemon.h
 * @ingroup PKSav
 * @brief   Native formats and convenience functions for Pokémon in Generation I games.
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

typedef struct {
    uint8_t species;
    uint16_t current_hp;
    uint8_t level;
    uint8_t status;
    uint8_t types[2];
    uint8_t catch_rate;
    uint8_t moves[4];
    uint16_t ot_id;
    uint8_t exp[3];
    uint16_t ev_hp;
    uint16_t ev_atk;
    uint16_t ev_def;
    uint16_t ev_spd;
    uint16_t ev_spcl;
    uint16_t iv_data;
    uint8_t move_pps[4]; // TODO: PP, PP Up mask
} pksav_gen1_pc_pokemon_t;

typedef struct {
    uint8_t level;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spcl;
} pksav_gen1_pokemon_party_data_t;

typedef struct {
    pksav_gen1_pc_pokemon_t pc;
    pksav_gen1_pokemon_party_data_t party_data;
} pksav_gen1_party_pokemon_t;

typedef struct {
    uint8_t count;
    uint8_t species[7];
    pksav_gen1_party_pokemon_t party[6];
    uint8_t otnames[6][11];
    uint8_t nicknames[6][11];
} pksav_gen1_pokemon_party_t;

typedef struct {
    uint8_t count;
    uint8_t species[21];
    pksav_gen1_pc_pokemon_t entries[20];
    uint8_t otnames[20][11];
    uint8_t nicknames[20][11];
} pksav_gen1_pokemon_box_t;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

static PKSAV_INLINE uint32_t pksav_gen1_pokemon_get_experience(
    const pksav_gen1_pc_pokemon_t* pc_pokemon
) {
    return (uint32_t)(pksav_from_base256(pc_pokemon->exp, 3));
}

static PKSAV_INLINE void pksav_gen1_pokemon_set_experience(
    pksav_gen1_pc_pokemon_t* pc_pokemon,
    uint32_t new_experience
) {
    pksav_to_base256(new_experience, pc_pokemon->exp);
}

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_get_otname_as_text(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    char* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_get_otname_as_widetext(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    wchar_t* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_get_nickname_as_text(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    char* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_get_nickname_as_widetext(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    wchar_t* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_get_otname_as_text(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    char* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_get_otname_as_widetext(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    wchar_t* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_get_nickname_as_text(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    char* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_get_nickname_as_widetext(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    wchar_t* output_text
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_set_otname_from_text(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const char* otname
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_set_otname_from_widetext(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const wchar_t* otname
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_set_nickname_from_text(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const char* nickname
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_party_set_nickname_from_widetext(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const wchar_t* nickname
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_set_otname_from_text(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const char* otname
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_set_otname_from_widetext(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const wchar_t* otname
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_set_nickname_from_text(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const char* nickname
);

PKSAV_API pksav_error_t pksav_gen1_pokemon_box_set_nickname_from_widetext(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const wchar_t* nickname
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN1_POKEMON_H */
