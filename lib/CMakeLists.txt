#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${PKSAV_SOURCE_DIR}/include
    ${PKSAV_BINARY_DIR}/include
)

#
# Add component-specific files
#
IF(PKSAV_ENABLE_GEN1)
    ADD_SUBDIRECTORY(gen1)
ENDIF(PKSAV_ENABLE_GEN1)
IF(PKSAV_ENABLE_GEN2)
    ADD_SUBDIRECTORY(gen2)
ENDIF(PKSAV_ENABLE_GEN2)
IF(PKSAV_ENABLE_GBA)
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

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(math)

SET(pksav_c_sources
    error.c
    prng.c
    stats.c
    ${pksav_common_sources}
    ${pksav_math_sources}
    ${pksav_gen1_sources}
    ${pksav_gen2_sources}
    ${pksav_gba_sources}
    ${pksav_gcn_sources}
    ${pksav_gen4_sources}
    ${pksav_gen5_sources}
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

IF(PKSAV_STATIC)
    ADD_LIBRARY(pksav STATIC ${pksav_c_sources} ${pksav_rc})

    INSTALL(
        TARGETS pksav
        LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .a
        ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .lib
    )
ELSE()
    ADD_LIBRARY(pksav SHARED ${pksav_c_sources} ${pksav_rc})
    SET_TARGET_PROPERTIES(pksav PROPERTIES DEFINE_SYMBOL "PKSAV_DLL_EXPORTS")
    ADD_DEFINITIONS(-Dpksav_EXPORTS)

    INSTALL(
        TARGETS pksav
        LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .so
        ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .lib
        RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Libraries # .dll
    )
ENDIF(PKSAV_STATIC)

IF(UNIX)
    TARGET_LINK_LIBRARIES(pksav m)
ENDIF(UNIX)

#
# pkg-config file
#
IF(NOT PKSAV_USED_AS_SUBPROJECT)
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
ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)
