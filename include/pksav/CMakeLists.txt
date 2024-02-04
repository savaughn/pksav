#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
        gen1.h
        gen2.h
        gen3.h
    )

    ADD_SUBDIRECTORY(common)
    ADD_SUBDIRECTORY(math)
    ADD_SUBDIRECTORY(gen1)
    ADD_SUBDIRECTORY(gen2)
    ADD_SUBDIRECTORY(gen3)
    ADD_SUBDIRECTORY(gen4)
    ADD_SUBDIRECTORY(gen5)

    INSTALL(
        FILES ${pksav_headers}
        DESTINATION ${INCLUDE_DIR}/pksav
        COMPONENT Headers
    )
ENDIF(NOT PKSAV_DONT_INSTALL_HEADERS)
