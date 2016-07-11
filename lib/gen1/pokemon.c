/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen1/pokemon.h>
#include <pksav/gen1/text.h>

#include <string.h>
#include <wchar.h>

pksav_error_t pksav_gen1_pokemon_party_get_otname_as_text(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    char* output_text
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_from_gen1(
        pokemon_party->otnames[index],
        output_text, 7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_party_get_otname_as_widetext(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    wchar_t* output_text
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_from_gen1(
        pokemon_party->otnames[index],
        output_text, 7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_party_get_nickname_as_text(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    char* output_text
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_from_gen1(
        pokemon_party->nicknames[index],
        output_text, 10
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_party_get_nickname_as_widetext(
    const pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    wchar_t* output_text
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_from_gen1(
        pokemon_party->nicknames[index],
        output_text, 10
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_get_otname_as_text(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    char* output_text
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_from_gen1(
        pokemon_box->otnames[index],
        output_text, 7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_get_otname_as_widetext(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    wchar_t* output_text
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_from_gen1(
        pokemon_box->otnames[index],
        output_text, 7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_get_nickname_as_text(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    char* output_text
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_from_gen1(
        pokemon_box->nicknames[index],
        output_text, 10
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_get_nickname_as_widetext(
    const pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    wchar_t* output_text
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_from_gen1(
        pokemon_box->nicknames[index],
        output_text, 10
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_party_set_otname_from_text(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const char* otname
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_to_gen1(
        otname,
        pokemon_party->otnames[index],
        7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_party_set_otname_from_widetext(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const wchar_t* otname
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_to_gen1(
        otname,
        pokemon_party->otnames[index],
        7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_party_set_nickname_from_text(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const char* nickname
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_to_gen1(
        nickname,
        pokemon_party->nicknames[index],
        10
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_party_set_nickname_from_widetext(
    pksav_gen1_pokemon_party_t* pokemon_party,
    uint8_t index,
    const wchar_t* nickname
) {
    if(index > 5) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_to_gen1(
        nickname,
        pokemon_party->nicknames[index],
        10
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_set_otname_from_text(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const char* otname
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_to_gen1(
        otname,
        pokemon_box->otnames[index],
        7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_set_otname_from_widetext(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const wchar_t* otname
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_to_gen1(
        otname,
        pokemon_box->otnames[index],
        7
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_set_nickname_from_text(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const char* nickname
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_text_to_gen1(
        nickname,
        pokemon_box->nicknames[index],
        10
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen1_pokemon_box_set_nickname_from_widetext(
    pksav_gen1_pokemon_box_t* pokemon_box,
    uint8_t index,
    const wchar_t* nickname
) {
    if(index > 20) {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    pksav_widetext_to_gen1(
        nickname,
        pokemon_box->nicknames[index],
        10
    );

    return PKSAV_ERROR_NONE;
}
