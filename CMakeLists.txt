#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# CMake configuration
########################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(pksav C)

SET(pksav_VERSION_MAJOR 0)
SET(pksav_VERSION_MINOR 0)
SET(pksav_VERSION_PATCH 1)
SET(pksav_VERSION "${pksav_VERSION_MAJOR}.${pksav_VERSION_MINOR}.${pksav_VERSION_PATCH}")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")

####################################################################
# Project configuration
####################################################################
INCLUDE(PKSavPlatform)

FIND_PACKAGE(Doxygen)

########################################################################
# Create uninstall target
########################################################################
CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)

ADD_CUSTOM_TARGET(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

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
# Actual building
####################################################################
ADD_SUBDIRECTORY(include)
#ADD_SUBDIRECTORY(lib)

####################################################################
# Final display
####################################################################
MESSAGE(STATUS "")
MESSAGE(STATUS "Building PKSav.")
MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS " - Compiler: ${CMAKE_C_COMPILER_ID}")
MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "")
