/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav/math/endian.h>

static void byteswap16_test() {
    uint16_t num1 = 0xABCD;
    uint16_t num2 = pksav_byteswap16(num1);
    TEST_ASSERT_EQUAL(num2, 0xCDAB);
}

static void byteswap32_test() {
    uint32_t num1 = 0xABCDEF12;
    uint32_t num2 = pksav_byteswap32(num1);
    TEST_ASSERT_EQUAL(num2, 0x12EFCDAB);
}

PKSAV_C_TEST_MAIN(
    PKSAV_C_TEST(byteswap16_test)
    PKSAV_C_TEST(byteswap32_test)
)
