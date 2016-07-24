#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MACRO(PKSAV_BUILD_DOXYGEN_DOCS files)
    SET(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/")
    MAKE_DIRECTORY(${DOXYGEN_OUTPUT_DIR})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY)
    ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        OUTPUT ${DOXYGEN_OUTPUT_DIR}/html/index.html DEPENDS ${files}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        COMMENT "Generating Doxygen documentation"
    )
    ADD_CUSTOM_TARGET(doxygen_docs ALL DEPENDS ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    INSTALL(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION ${DOCS_DIR} COMPONENT doxygen)
ENDMACRO(PKSAV_BUILD_DOXYGEN_DOCS)

FILE(GLOB_RECURSE h_files ${CMAKE_SOURCE_DIR}/include/*.h)
LIST(APPEND h_files
    ${CMAKE_SOURCE_DIR}/include/pksav/config.h.in
    ${CMAKE_SOURCE_DIR}/include/pksav/version.h.in
)

PKSAV_BUILD_DOXYGEN_DOCS("${h_files}" "c" "C")
