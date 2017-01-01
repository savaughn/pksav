#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PACKAGE(VALGRIND)

# Test dependencies
ADD_SUBDIRECTORY(getopt)
ADD_SUBDIRECTORY(Unity)

# Set locations of test saves.
SET(POKEMON_SAVE_DIR "${PKSAV_SOURCE_DIR}/testing/pksav-test-saves")

SET(POKEMON_RED_SAV     "${POKEMON_SAVE_DIR}/red_blue/pokemon_red.sav")
SET(POKEMON_YELLOW_SAV  "${POKEMON_SAVE_DIR}/yellow/pokemon_yellow.sav")
SET(POKEMON_GOLD_SAV    "${POKEMON_SAVE_DIR}/gold_silver/pokemon_gold.sav")
SET(POKEMON_CRYSTAL_SAV "${POKEMON_SAVE_DIR}/crystal/pokemon_crystal.sav")
SET(POKEMON_RUBY_SAV    "${POKEMON_SAVE_DIR}/ruby_sapphire/pokemon_ruby.sav")
SET(POKEMON_EMERALD_SAV "${POKEMON_SAVE_DIR}/emerald/pokemon_emerald.sav")
SET(POKEMON_FIRERED_SAV "${POKEMON_SAVE_DIR}/firered_leafgreen/pokemon_firered.sav")

# Fix Windows paths
IF(WIN32)
    STRING(REPLACE "/" "\\" POKEMON_RED_SAV "${POKEMON_RED_SAV}")
    STRING(REPLACE "/" "\\" POKEMON_YELLOW_SAV "${POKEMON_YELLOW_SAV}")
    STRING(REPLACE "/" "\\" POKEMON_GOLD_SAV "${POKEMON_GOLD_SAV}")
    STRING(REPLACE "/" "\\" POKEMON_CRYSTAL_SAV "${POKEMON_CRYSTAL_SAV}")
    STRING(REPLACE "/" "\\" POKEMON_RUBY_SAV "${POKEMON_RUBY_SAV}")
    STRING(REPLACE "/" "\\" POKEMON_EMERALD_SAV "${POKEMON_EMERALD_SAV}")
    STRING(REPLACE "/" "\\" POKEMON_FIRERED_SAV "${POKEMON_FIRERED_SAV}")
ENDIF(WIN32)

# Tests
ADD_SUBDIRECTORY(save-tests)
ADD_SUBDIRECTORY(unit-tests)
