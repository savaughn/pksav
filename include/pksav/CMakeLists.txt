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
    items.h
    prng.h
    stats.h
)

INSTALL(
    FILES ${pksav_headers}
    DESTINATION ${INCLUDE_DIR}/pksav
    COMPONENT Headers
)

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(math)

IF(ENABLE_GEN1)
    ADD_SUBDIRECTORY(gen1)
ENDIF(ENABLE_GEN1)
IF(ENABLE_GEN2)
    ADD_SUBDIRECTORY(gen2)
ENDIF(ENABLE_GEN2)
IF(ENABLE_GBA)
    ADD_SUBDIRECTORY(gba)
ENDIF(ENABLE_GBA)
IF(ENABLE_GEN4)
    ADD_SUBDIRECTORY(gen4)
ENDIF(ENABLE_GEN4)
IF(ENABLE_GEN5)
    ADD_SUBDIRECTORY(gen5)
ENDIF(ENABLE_GEN5)
