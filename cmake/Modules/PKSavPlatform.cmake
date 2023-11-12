#
# Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Set build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    SET(PKSAV_GCC TRUE)
ELSEIF(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    SET(PKSAV_CLANG TRUE)
ENDIF(${CMAKE_C_COMPILER_ID} STREQUAL "GNU")

IF(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    SET(PKSAV_64BIT TRUE)
ELSE()
    SET(PKSAV_32BIT TRUE)
ENDIF(${CMAKE_SIZEOF_VOID_P} EQUAL 8)

IF(PKSAV_GCC OR PKSAV_CLANG)
    SET(PKSAV_C_FLAGS "-std=gnu99 -Wall -Wextra -Wno-error -fvisibility=hidden")
    IF(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        SET(PKSAV_C_FLAGS "-O3 ${PKSAV_C_FLAGS}")
    ENDIF()
ELSEIF(MSVC)
    ADD_DEFINITIONS(/MP)                       # Multi-threaded build
    ADD_DEFINITIONS(/EHsc)                     # Exception handling
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS) # Ignore deprecation warnings
    ADD_DEFINITIONS(/W3)                       # Warnings become errors
ENDIF(PKSAV_GCC OR PKSAV_CLANG)

#
# Currently, there is no Boostless way to check endianness when
# cross-compiling. CMake has a function, but it doesn't work with
# cross-compiling. As such, the user needs to specify it.
#
# If there is no specification, assume little-endian, as the majority
# of platforms are.
#
MESSAGE(STATUS "")
IF(NOT PKSAV_LITTLE_ENDIAN AND NOT PKSAV_BIG_ENDIAN)
    MESSAGE(STATUS "PKSav: no endianness specified. Assuming little endian.")
    SET(PKSAV_BIG_ENDIAN FALSE)
    SET(PKSAV_LITTLE_ENDIAN TRUE)
ELSEIF(PKSAV_BIG_ENDIAN)
    MESSAGE(STATUS "PKSav: big endian specified.")
    SET(PKSAV_BIG_ENDIAN TRUE)
    SET(PKSAV_LITTLE_ENDIAN FALSE)
ELSEIF(PKSAV_LITTLE_ENDIAN)
    MESSAGE(STATUS "PKSav: little endian specified.")
    SET(PKSAV_BIG_ENDIAN FALSE)
    SET(PKSAV_LITTLE_ENDIAN TRUE)
ENDIF(NOT PKSAV_LITTLE_ENDIAN AND NOT PKSAV_BIG_ENDIAN)

# Checks for required headers
INCLUDE(CheckIncludeFile)
SET(CMAKE_REQUIRED_FLAGS "${PKSAV_C_FLAGS}")
CHECK_INCLUDE_FILE(stdbool.h HAVE_STDBOOL_H)
IF(NOT HAVE_STDBOOL_H)
    MESSAGE(FATAL_ERROR "PKSav requires the header stdbool.h to compile.")
ENDIF(NOT HAVE_STDBOOL_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
IF(NOT HAVE_STDINT_H)
    MESSAGE(FATAL_ERROR "PKSav requires the header stdint.h to compile.")
ENDIF(NOT HAVE_STDINT_H)

# Checks for platform-specific headers
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)

# Set compiler name for CMake display
IF(MSVC)
    IF(MSVC12)
        SET(PKSAV_COMPILER_NAME "Microsoft Visual Studio 2013")
    ELSEIF(MSVC14)
        SET(PKSAV_COMPILER_NAME "Microsoft Visual Studio 2015")
    ELSE()
        SET(PKSAV_COMPILER_NAME "${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
    ENDIF(MSVC12)
ELSEIF(MINGW)
    SET(PKSAV_COMPILER_NAME "MinGW-${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
ELSE()
    SET(PKSAV_COMPILER_NAME "${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
ENDIF(MSVC)

MESSAGE(STATUS "")
