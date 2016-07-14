#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

#
# Add component-specific files
#
IF(ENABLE_GEN1)
    ADD_SUBDIRECTORY(gen1)
ENDIF(ENABLE_GEN1)
IF(ENABLE_GEN2)
    ADD_SUBDIRECTORY(gen2)
ENDIF(ENABLE_GEN2)
IF(ENABLE_GBA)
    ADD_SUBDIRECTORY(gba)
ENDIF(ENABLE_GBA)
IF(ENABLE_GCN)
    ADD_SUBDIRECTORY(gcn)
ENDIF(ENABLE_GCN)
IF(ENABLE_NDS)
    ADD_SUBDIRECTORY(nds)
ENDIF(ENABLE_NDS)

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(math)

SET(pksav_c_sources
    checksum.c
    error.c
    prng.c
    stats.c
    ${pksav_common_sources}
    ${pksav_math_sources}
    ${pksav_gen1_sources}
    ${pksav_gen2_sources}
    ${pksav_gba_sources}
    ${pksav_gcn_sources}
    ${pksav_nds_sources}
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

IF(UNIX)
    TARGET_LINK_LIBRARIES(pksav m)
ENDIF(UNIX)

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
