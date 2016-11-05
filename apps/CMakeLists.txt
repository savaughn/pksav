#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKSAV_SOURCE_DIR}/include
    ${PKSAV_BINARY_DIR}/include
)

ADD_SUBDIRECTORY(getopt)

MACRO(PKSAV_BUILD_APP app_name)
    SET_SOURCE_FILES_PROPERTIES(${app_name}.c
        PROPERTIES COMPILE_FLAGS "${PKSAV_C_FLAGS}"
    )
    ADD_EXECUTABLE(${app_name} ${app_name}.c)
    TARGET_LINK_LIBRARIES(${app_name} pksav getopt)
    INSTALL(
        TARGETS ${app_name}
        DESTINATION ${RUNTIME_DIR}
        COMPONENT Applications
    )
ENDMACRO(PKSAV_BUILD_APP)

IF(PKSAV_ENABLE_GEN1)
    LIST(APPEND pksav_apps pksav-gen1-save-dump)
ENDIF(PKSAV_ENABLE_GEN1)
IF(PKSAV_ENABLE_GEN2)
    LIST(APPEND pksav_apps pksav-gen2-save-dump)
ENDIF(PKSAV_ENABLE_GEN2)
IF(PKSAV_ENABLE_GBA)
    LIST(APPEND pksav_apps pksav-gba-save-dump)
ENDIF(PKSAV_ENABLE_GBA)

FOREACH(app ${pksav_apps})
    PKSAV_BUILD_APP(${app})
ENDFOREACH(app ${pksav_apps})
