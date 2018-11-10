#
# Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_common_headers
    condition.h
    constants.h
    contest_stats.h
    datetime.h
    gen4_encounter_type.h
    item.h
    markings.h
    nature.h
    nds_pokemon.h
    pokedex.h
    pokerus.h
    stats.h
    trainer_id.h
)

INSTALL(
    FILES ${pksav_common_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/common
    COMPONENT Headers
)
