#
# Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gba_headers
    box_wallpaper.h
    common.h
    daycare.h
    items.h
    language.h
    mail.h
    map.h
    options.h
    pokedex.h
    pokemon.h
    roamer.h
    save.h
    text.h
    time.h
)

INSTALL(
    FILES ${pksav_gba_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gba
    COMPONENT Headers
)
