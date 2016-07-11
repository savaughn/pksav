#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_headers
    checksum.h
    config.h
    datetime.h
    error.h
    gen1_save.h
    items.h
    pokemon.h
    pokerus.h
    prng.h
    stats.h
    text.h
)

INSTALL(
    FILES ${pksav_headers}
    DESTINATION ${INCLUDE_DIR}/pksav
    COMPONENT Headers
)

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(math)
