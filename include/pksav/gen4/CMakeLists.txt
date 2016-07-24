#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gen4_headers
    items.h
    pokemon.h
    save.h
    save_structs.h
    text.h
)

INSTALL(
    FILES ${pksav_gen4_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gen4
    COMPONENT Headers
)
