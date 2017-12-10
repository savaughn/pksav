#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(PKSAV_USE_CLANGTIDY "${PKSAV_USE_CLANGTIDY}" CACHE BOOL "Enable Clang-tidy testing")

IF(PKSAV_USE_CLANGTIDY)
    if(NOT CLANG_TIDY_EXECUTABLE)
        FIND_PROGRAM(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
    ENDIF()
    MESSAGE(STATUS "Using clang-tidy: ${CLANG_TIDY_EXECUTABLE}")

    SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    SET(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
    SET(CLANG_TIDY_PROPERTY
        "${CLANG_TIDY_EXECUTABLE}"
        "-checks=clang-diagnostic-*"
    )
ENDIF()
