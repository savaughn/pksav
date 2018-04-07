#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PKSAV_SOURCE_DIR}/include
    ${PKSAV_BINARY_DIR}/include
    ${PKSAV_SOURCE_DIR}/lib
)

ADD_LIBRARY(pksav-test-utils STATIC
    ${PKSAV_SOURCE_DIR}/lib/util/fs.c
    test-utils.c
)
IF(UNIX)
    SET_TARGET_PROPERTIES(pksav-test-utils
        PROPERTIES COMPILE_FLAGS "${PKSAV_C_FLAGS} -fPIC"
    )
ENDIF(UNIX)

MACRO(PKSAV_ADD_UNIT_TEST test_name)
    SET(src ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.c)
    SET(PKSAV_TEST_SAVES "${PKSAV_SOURCE_DIR}/testing/pksav-test-saves")

    SET_SOURCE_FILES_PROPERTIES(${src}
        PROPERTIES COMPILE_FLAGS "${PKSAV_C_FLAGS}"
    )
    ADD_EXECUTABLE(${test_name} ${src})
    TARGET_LINK_LIBRARIES(${test_name} pksav unity pksav-test-utils)

    IF(NOT CMAKE_CROSSCOMPILING)
        IF(MINGW)
            SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
            SET(TEST_LIB_DIR "lib")
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            STRING(REPLACE "/" "\\" TEST_LIB_DIR "${TEST_LIB_DIR}")
            STRING(REPLACE "/" "\\" PKSAV_TEST_SAVES "${PKSAV_TEST_SAVES}")
            CONFIGURE_FILE(
                ${CMAKE_CURRENT_SOURCE_DIR}/../test_template.bat.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
            @ONLY)
            ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
        ELSEIF(WIN32)
            SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}")
            SET(TEST_LIB_DIR "lib/${CMAKE_BUILD_TYPE}")
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            STRING(REPLACE "/" "\\" TEST_LIB_DIR "${TEST_LIB_DIR}")
            STRING(REPLACE "/" "\\" PKSAV_TEST_SAVES "${PKSAV_TEST_SAVES}")
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

        ENDIF()
    ENDIF(NOT CMAKE_CROSSCOMPILING)
ENDMACRO(PKSAV_ADD_UNIT_TEST)

SET(unit_tests
    byteswap_test
    error_test
    gen1_save_test
    gen2_save_test
    gba_save_test
    math_test
    null_pointer_test
    pokerus_test
    text_conversion_test
)

FOREACH(test ${unit_tests})
    PKSAV_ADD_UNIT_TEST(${test})
ENDFOREACH(test ${unit_tests})

# Build null_pointer_test.c as a C++ executable since it conveniently
# calls every function. This will fail to link if an 'extern "C"' declaration
# is missing from any header.
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/null_pointer_test.c
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_exe.cpp
)
ADD_EXECUTABLE(cpp_exe ${CMAKE_CURRENT_BINARY_DIR}/cpp_exe.cpp)
TARGET_LINK_LIBRARIES(cpp_exe pksav unity pksav-test-utils)
