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
