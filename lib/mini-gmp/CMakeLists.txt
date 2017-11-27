#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Mini-GMP needs to be separate because it's licensed as LGPL.
ADD_LIBRARY(mini-gmp SHARED
    mini-gmp.c
)

INSTALL(
    TARGETS mini-gmp
    LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .so
    RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Libraries # .dll
)
