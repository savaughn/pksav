#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# CMake configuration
########################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(pksav C CXX)

SET(PKSAV_VERSION_MAJOR 2)
SET(PKSAV_VERSION_MINOR 1)
SET(PKSAV_VERSION_PATCH 0)
SET(PKSAV_VERSION "${PKSAV_VERSION_MAJOR}.${PKSAV_VERSION_MINOR}.${PKSAV_VERSION_PATCH}")

SET(PKSAV_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(PKSAV_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PKSAV_SOURCE_DIR})
    SET(PKSAV_USED_AS_SUBPROJECT TRUE)
    IF(NOT DEFINED PKSAV_DONT_INSTALL_HEADERS)
        SET(PKSAV_DONT_INSTALL_HEADERS TRUE)
    ENDIF(NOT DEFINED PKSAV_DONT_INSTALL_HEADERS)
ENDIF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PKSAV_SOURCE_DIR})

SET(CMAKE_MODULE_PATH "${PKSAV_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

####################################################################
# Project configuration
####################################################################
INCLUDE(PKSavPlatform)
INCLUDE(PKSavComponent)

IF(NOT PKSAV_USED_AS_SUBPROJECT)
    FIND_PACKAGE(Doxygen)
ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)

########################################################################
# Create uninstall target
########################################################################
IF(NOT PKSAV_USED_AS_SUBPROJECT)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY)

    ADD_CUSTOM_TARGET(uninstall
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)

####################################################################
# Set install subdirectories
####################################################################
IF(NOT DEFINED LIB_SUFFIX AND REDHAT AND CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
    SET(LIB_SUFFIX 64)
ENDIF()
SET(LIB_SUFFIX ${LIB_SUFFIX} CACHE STRING "lib directory suffix")
SET(RUNTIME_DIR bin)
SET(LIBRARY_DIR lib${LIB_SUFFIX})
SET(INCLUDE_DIR include)
SET(PKG_DATA_DIR share/pksav)
SET(DOCS_DIR share/doc/pksav)

####################################################################
# Components
####################################################################
PKSAV_REGISTER_COMPONENT("Library" PKSAV_ENABLE_LIBRARY ON "" OFF)

IF(NOT PKSAV_USED_AS_SUBPROJECT)
    PKSAV_REGISTER_COMPONENT("Doxygen Documentation" PKSAV_ENABLE_DOCS  ON "PKSAV_ENABLE_LIBRARY;DOXYGEN_FOUND" OFF)
    PKSAV_REGISTER_COMPONENT("Unit Tests"            PKSAV_ENABLE_TESTS ON "PKSAV_ENABLE_LIBRARY" OFF)
ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)

####################################################################
# Actual building
####################################################################
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(lib)

IF(PKSAV_ENABLE_DOCS)
    ADD_SUBDIRECTORY(docs)
ENDIF(PKSAV_ENABLE_DOCS)

IF(PKSAV_ENABLE_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(testing)
ENDIF(PKSAV_ENABLE_TESTS)

####################################################################
# Final display
####################################################################
PKSAV_PRINT_COMPONENT_SUMMARY()
IF(NOT PKSAV_USED_AS_SUBPROJECT)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Building PKSav.")
    MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS " - Compiler: ${CMAKE_C_COMPILER_ID}")
    MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "")
ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)
