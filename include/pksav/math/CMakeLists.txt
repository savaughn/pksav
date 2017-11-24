#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_math_headers
    base256.h
    bcd.h
    endian.h
)

INSTALL(
    FILES ${pksav_math_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/math
    COMPONENT Headers
)
