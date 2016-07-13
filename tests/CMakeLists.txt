#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

MACRO(PKSAV_ADD_TEST test_name)
    ADD_EXECUTABLE(${test_name} ${test_name}.c)
    TARGET_LINK_LIBRARIES(${test_name} pksav)
    IF(CMAKE_CROSSCOMPILING)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name})
    ELSE()
        SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
        IF(WIN32)
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            CONFIGURE_FILE(
                ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_template.bat.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
            @ONLY)
            ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
        ELSE()
            IF(APPLE)
                SET(DY "DY")
            ENDIF(APPLE)
            CONFIGURE_FILE(
                ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_template.sh.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
            @ONLY)
            ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
        ENDIF(WIN32)
    ENDIF(CMAKE_CROSSCOMPILING)
ENDMACRO(PKSAV_ADD_TEST)

SET(unit_tests
    math_test
)

FOREACH(test ${unit_tests})
    PKSAV_ADD_TEST(${test})
ENDFOREACH(test ${unit_tests})
