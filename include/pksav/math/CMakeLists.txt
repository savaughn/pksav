#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_math_headers
    endian.h
)

IF(PKSAV_ENABLE_GEN1 OR PKSAV_ENABLE_GEN2)
    LIST(APPEND pksav_math_headers
        base256.h
        bcd.h
    )
ENDIF(PKSAV_ENABLE_GEN1 OR PKSAV_ENABLE_GEN2)

INSTALL(
    FILES ${pksav_math_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/math
    COMPONENT Headers
)
