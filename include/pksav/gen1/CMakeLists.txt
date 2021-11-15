#
# Copyright (c) 2016,2021 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gen1_headers
    items.h
    map_data.h
    pokemon.h
    save.h
    text.h
)

INSTALL(
    FILES ${pksav_gen1_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gen1
    COMPONENT Headers
)
