/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen2/pokemon.h>

void pksav_gen2_pokemon_set_caught_data_time_field(
    const time_t* time_in,
    pksav_gen2_pc_pokemon_t* gen2_pokemon
) {
    gen2_pokemon->caught_data &= ~PKSAV_GEN2_TIME_OF_DAY_MASK;

    struct tm* gmtm = localtime(time_in);
    if(gmtm->tm_hour >= 2 && gmtm->tm_hour <= 8) {
        gen2_pokemon->caught_data |= (PKSAV_GEN2_MORNING << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    } else if(gmtm->tm_hour >= 9 && gmtm->tm_hour <= 16) {
        gen2_pokemon->caught_data |= (PKSAV_GEN2_DAY << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    } else {
        gen2_pokemon->caught_data |= (PKSAV_GEN2_NIGHT << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    }
}
