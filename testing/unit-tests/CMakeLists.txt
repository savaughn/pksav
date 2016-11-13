#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

MACRO(PKSAV_ADD_UNIT_TEST test_name)
    SET_SOURCE_FILES_PROPERTIES(${test_name}.c
        PROPERTIES COMPILE_FLAGS "${PKSAV_C_FLAGS}"
    )
    ADD_EXECUTABLE(${test_name} ${test_name}.c)
    TARGET_LINK_LIBRARIES(${test_name} pksav unity)

    IF(NOT CMAKE_CROSSCOMPILING)
        IF(WIN32)
            SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}")
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            CONFIGURE_FILE(
                ${CMAKE_CURRENT_SOURCE_DIR}/../test_template.bat.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
            @ONLY)
            ADD_UNIT_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
        ELSE()
            # If Valgrind is found, use it.
            IF(VALGRIND_FOUND)
                SET(TEST_CMD "${VALGRIND_PROGRAM} --leak-check=full --track-origins=yes --error-exitcode=1 ${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
            ELSE()
                SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
            ENDIF(VALGRIND_FOUND)
            IF(APPLE)
                SET(DY "DY")
            ENDIF(APPLE)
            CONFIGURE_FILE(
                ${CMAKE_CURRENT_SOURCE_DIR}/../test_template.sh.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
            @ONLY)
            ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
        ENDIF(WIN32)
    ENDIF(NOT CMAKE_CROSSCOMPILING)
ENDMACRO(PKSAV_ADD_UNIT_TEST)

SET(unit_tests
    byteswap_test
    math_test
    null_pointer_test
)

FOREACH(test ${unit_tests})
    PKSAV_ADD_UNIT_TEST(${test})
ENDFOREACH(test ${unit_tests})
