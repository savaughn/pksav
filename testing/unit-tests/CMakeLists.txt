#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PKSAV_SOURCE_DIR}/include
    ${PKSAV_BINARY_DIR}/include
)

MACRO(PKSAV_ADD_UNIT_TEST test_name configure)
    IF(${configure})
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.c
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.c
        @ONLY)
        SET(src ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.c)
    ELSE()
        SET(src ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.c)
    ENDIF(${configure})

    SET_SOURCE_FILES_PROPERTIES(${src}
        PROPERTIES COMPILE_FLAGS "${PKSAV_C_FLAGS}"
    )
    ADD_EXECUTABLE(${test_name} ${src})
    TARGET_LINK_LIBRARIES(${test_name} pksav unity)

    IF(NOT CMAKE_CROSSCOMPILING)
        IF(WIN32)
            SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}")
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            CONFIGURE_FILE(
                ${CMAKE_CURRENT_SOURCE_DIR}/../test_template.bat.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
            @ONLY)
            ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
        ELSE()
            # If Valgrind is found, use it.
            IF(VALGRIND_FOUND AND TEST_WITH_VALGRIND)
                SET(TEST_CMD "${VALGRIND_PROGRAM} --leak-check=full --track-origins=yes --error-exitcode=1 ${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
            ELSE()
                SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
            ENDIF(VALGRIND_FOUND AND TEST_WITH_VALGRIND)
            IF(APPLE)
                SET(DY "DY")
            ENDIF(APPLE)
            CONFIGURE_FILE(
                ${CMAKE_CURRENT_SOURCE_DIR}/../test_template.sh.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
            @ONLY)

            IF(PKSAV_USED_AS_SUBPROJECT)
                ADD_TEST(pksav_${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
            ELSE()
                ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
            ENDIF(PKSAV_USED_AS_SUBPROJECT)

        ENDIF(WIN32)
    ENDIF(NOT CMAKE_CROSSCOMPILING)
ENDMACRO(PKSAV_ADD_UNIT_TEST)

SET(unit_tests
    byteswap_test
    math_test
    null_pointer_test
)

FOREACH(test ${unit_tests})
    PKSAV_ADD_UNIT_TEST(${test} FALSE)
ENDFOREACH(test ${unit_tests})

SET(unit_tests_configure
    save_detection_test
)

FOREACH(test ${unit_tests_configure})
    PKSAV_ADD_UNIT_TEST(${test} TRUE)
ENDFOREACH(test ${unit_tests_configure})
