#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(StaticAnalysisTools)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PKSAV_SOURCE_DIR}/include
    ${PKSAV_BINARY_DIR}/include
)

ADD_SUBDIRECTORY(gen1)
ADD_SUBDIRECTORY(gen2)
ADD_SUBDIRECTORY(gen3)

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(crypto)
ADD_SUBDIRECTORY(math)
ADD_SUBDIRECTORY(util)

SET(pksav_c_sources
    error.c
    ${pksav_common_sources}
    ${pksav_crypto_sources}
    ${pksav_math_sources}
    ${pksav_gen1_sources}
    ${pksav_gen2_sources}
    ${pksav_gen3_sources}
    ${pksav_util_sources}
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
ENDIF()

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
ENDIF()

IF(UNIX)
    TARGET_LINK_LIBRARIES(pksav m)
ENDIF()

#
# Static Analysis
#
IF(PKSAV_USE_CLANGTIDY)
    SET_TARGET_PROPERTIES(
        pksav PROPERTIES
        C_CLANG_TIDY "${CLANG_TIDY_PROPERTY}"
    )
ENDIF()

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
ENDIF()
