#
# Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gba_headers
    items.h
    pokemon.h
    save.h
    text.h
    time.h
    trainer_info.h
)

INSTALL(
    FILES ${pksav_gba_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gba
    COMPONENT Headers
)
