#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gen6_headers
    text.h
)

INSTALL(
    FILES ${pksav_gen6_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gen6
    COMPONENT Headers
)
