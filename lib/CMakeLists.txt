#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include
)

SET(pksav_c_sources
    checksum.c
    datetime.c
    error.c
    prng.c
    stats.c
    text.c
)

SET_SOURCE_FILES_PROPERTIES(${pksav_c_sources}
    PROPERTIES COMPILE_FLAGS "${PKSAV_C_FLAGS}"
)

#
# Add DLL resource file to Windows builds
#
IF(WIN32)
    IF(MINGW)
        SET(LIB_PREFIX "lib")
    ENDIF(MINGW)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/pksav.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/pksav.rc
    @ONLY)
    SET(pksav_rc ${CMAKE_CURRENT_BINARY_DIR}/pksav.rc)
ENDIF(WIN32)

ADD_LIBRARY(pksav SHARED ${pksav_c_sources} ${pksav_rc})
SET_TARGET_PROPERTIES(pksav PROPERTIES DEFINE_SYMBOL "PKSAV_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpksav_EXPORTS)

INSTALL(
    TARGETS pksav
    LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .so
    ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .lib
    RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Libraries # .dll
)

#
# pkg-config file
#
FIND_PACKAGE(PkgConfig)
IF(PKG_CONFIG_FOUND)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/pksav.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/pksav.pc
    @ONLY)
    INSTALL(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/pksav.pc
        DESTINATION ${LIBRARY_DIR}/pkgconfig
    )
ENDIF(PKG_CONFIG_FOUND)
