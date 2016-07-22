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

MACRO(PKSAV_BUILD_APP app_name)
    ADD_EXECUTABLE(${app_name} ${app_name}.c)
    TARGET_LINK_LIBRARIES(${app_name} pksav)
    INSTALL(
        TARGETS ${app_name}
        DESTINATION ${RUNTIME_DIR}
        COMPONENT Applications
    )
ENDMACRO(PKSAV_BUILD_APP)

SET(pksav_apps
    pksav-gen1-save-dump
)

FOREACH(app ${pksav_apps})
    PKSAV_BUILD_APP(${app})
ENDFOREACH(app ${pksav_apps})
