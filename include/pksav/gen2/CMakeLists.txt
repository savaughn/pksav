#
# Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gen2_headers
    badges.h
    common.h
    daycare_data.h
    items.h
    mom_money_policy.h
    options.h
    palette.h
    pokemon.h
    save.h
    text.h
    time.h
)

INSTALL(
    FILES ${pksav_gen2_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gen2
    COMPONENT Headers
)
