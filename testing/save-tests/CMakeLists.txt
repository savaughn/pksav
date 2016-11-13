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

SET(SAVEDIR ${CMAKE_CURRENT_SOURCE_DIR}/../pksav-test-saves)

MACRO(PKSAV_ADD_SAVE_TEST test_name test_saves)
    SET_SOURCE_FILES_PROPERTIES(${test_name}.c
        PROPERTIES COMPILE_FLAGS "${PKSAV_C_FLAGS}"
    )
    ADD_EXECUTABLE(${test_name} ${test_name}.c)
    TARGET_LINK_LIBRARIES(${test_name} pksav getopt unity)

    IF(NOT CMAKE_CROSSCOMPILING)
        FOREACH(save ${test_saves})
            GET_FILENAME_COMPONENT(save_filename "${SAVEDIR}/${save}" NAME)
            GET_FILENAME_COMPONENT(save_base "${SAVEDIR}/${save}" NAME_WE)

            IF(WIN32)
                SET(TEST_CMD "\"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}\" --all --input=${SAVEDIR}/${save}")
                STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")

                CONFIGURE_FILE(
                    ${CMAKE_CURRENT_SOURCE_DIR}/../test_template.bat.in
                    ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_${save_base}.bat
                @ONLY)
                ADD_TEST(${test_name}_${save_base} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_${save_base}.bat)
            ELSE()
                # If Valgrind is found, use it.
                IF(VALGRIND_FOUND)
                    SET(TEST_CMD "${VALGRIND_PROGRAM} --leak-check=full --track-origins=yes --error-exitcode=1 ${CMAKE_CURRENT_BINARY_DIR}/${test_name} --all --input=${SAVEDIR}/${save}")
                ELSE()
                    SET(TEST_CMD "${CMAKE_CURRENT_BINARY_DIR}/${test_name} --all --input=${SAVEDIR}/${save}")
                ENDIF(VALGRIND_FOUND)
                IF(APPLE)
                    SET(DY "DY")
                ENDIF(APPLE)

                CONFIGURE_FILE(
                    ${CMAKE_CURRENT_SOURCE_DIR}/../test_template.sh.in
                    ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_${save_base}.sh
                @ONLY)
                ADD_TEST(${test_name}_${save_base} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_${save_base}.sh)
            ENDIF(WIN32)
        ENDFOREACH(save ${test_saves})
    ENDIF(NOT CMAKE_CROSSCOMPILING)
ENDMACRO(PKSAV_ADD_SAVE_TEST)

IF(PKSAV_ENABLE_GEN1)
    PKSAV_ADD_SAVE_TEST(gen1_save_test "red_blue/pokemon_red.sav;yellow/pokemon_yellow.sav")
ENDIF(PKSAV_ENABLE_GEN1)
IF(PKSAV_ENABLE_GEN2)
    PKSAV_ADD_SAVE_TEST(gen2_save_test "gold_silver/pokemon_gold.sav;crystal/pokemon_crystal.sav")
ENDIF(PKSAV_ENABLE_GEN2)
IF(PKSAV_ENABLE_GBA)
    PKSAV_ADD_SAVE_TEST(gba_save_test "ruby_sapphire/pokemon_ruby.sav;emerald/pokemon_emerald.sav;firered_leafgreen/pokemon_firered.sav")
ENDIF(PKSAV_ENABLE_GBA)
