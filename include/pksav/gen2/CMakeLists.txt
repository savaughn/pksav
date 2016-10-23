#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gen2_headers
    items.h
    pokemon.h
    save.h
    text.h
    time_of_day.h
)

INSTALL(
    FILES ${pksav_gen2_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gen2
    COMPONENT Headers
)
