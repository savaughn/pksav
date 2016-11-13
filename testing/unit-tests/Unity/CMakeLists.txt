#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY(unity STATIC unity.c)

IF(MINGW)
    SET_TARGET_PROPERTIES(unity PROPERTIES
        COMPILE_FLAGS "-Werror"
    )
ELSEIF(NOT MSVC)
    SET_TARGET_PROPERTIES(unity PROPERTIES
        COMPILE_FLAGS "-fPIC -Werror"
    )
ENDIF(MINGW)
