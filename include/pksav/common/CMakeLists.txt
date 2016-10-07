#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_common_headers
    condition.h
    contest_stats.h
    coordinates.h
    datetime.h
    gen3_ribbons.h
    gen4_encounter_type.h
    markings.h
    nature.h
    nds_pokemon.h
    pokedex.h
    pokerus.h
    prng.h
    stats.h
    trainer_card_signature.h
    trainer_id.h
)

INSTALL(
    FILES ${pksav_common_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/common
    COMPONENT Headers
)
