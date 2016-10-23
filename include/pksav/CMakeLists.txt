#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
@ONLY)

#
# Create version header based on CMake variables
#
MATH(EXPR PKSAV_VERSION_NUMBER "(${PKSAV_VERSION_MAJOR} * 1000000) +
                                (${PKSAV_VERSION_MINOR} * 1000) +
                                 ${PKSAV_VERSION_PATCH}")

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
@ONLY)

IF(NOT PKSAV_DONT_INSTALL_HEADERS)
    SET(pksav_headers
        error.h
        ${CMAKE_CURRENT_BINARY_DIR}/config.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
    )

    ADD_SUBDIRECTORY(common)
    ADD_SUBDIRECTORY(math)

    IF(PKSAV_ENABLE_GEN1)
        LIST(APPEND pksav_headers gen1.h)
        ADD_SUBDIRECTORY(gen1)
    ENDIF(PKSAV_ENABLE_GEN1)
    IF(PKSAV_ENABLE_GEN2)
        LIST(APPEND pksav_headers gen2.h)
        ADD_SUBDIRECTORY(gen2)
    ENDIF(PKSAV_ENABLE_GEN2)
    IF(PKSAV_ENABLE_GBA)
        LIST(APPEND pksav_headers gba.h)
        ADD_SUBDIRECTORY(gba)
    ENDIF(PKSAV_ENABLE_GBA)
    IF(PKSAV_ENABLE_GCN)
        ADD_SUBDIRECTORY(gcn)
    ENDIF(PKSAV_ENABLE_GCN)
    IF(PKSAV_ENABLE_GEN4)
        ADD_SUBDIRECTORY(gen4)
    ENDIF(PKSAV_ENABLE_GEN4)
    IF(PKSAV_ENABLE_GEN5)
        ADD_SUBDIRECTORY(gen5)
    ENDIF(PKSAV_ENABLE_GEN5)

    INSTALL(
        FILES ${pksav_headers}
        DESTINATION ${INCLUDE_DIR}/pksav
        COMPONENT Headers
    )
ENDIF(NOT PKSAV_DONT_INSTALL_HEADERS)
