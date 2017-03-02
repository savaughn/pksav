/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav/math/endian.h>

static void byteswap16_test() {
    uint16_t num1 = 0xABCD;
    uint16_t num2 = pksav_byteswap16(num1);
    TEST_ASSERT_EQUAL(0xCDAB, num2);
}

static void native_byteswap16_test() {
    uint16_t num = 0xABCD;
#if PKSAV_LITTLE_ENDIAN
    TEST_ASSERT_EQUAL(num, pksav_littleendian16(num));
#else
    TEST_ASSERT_EQUAL(num, pksav_bigendian16(num));
#endif
}

static void byteswap32_test() {
    uint32_t num1 = 0xABCDEF12;
    uint32_t num2 = pksav_byteswap32(num1);
    TEST_ASSERT_EQUAL(0x12EFCDAB, num2);
}

static void native_byteswap32_test() {
    uint32_t num = 0xABCDEF12;
#if PKSAV_LITTLE_ENDIAN
    TEST_ASSERT_EQUAL(num, pksav_littleendian32(num));
#else
    TEST_ASSERT_EQUAL(num, pksav_bigendian32(num));
#endif
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(byteswap16_test)
    PKSAV_TEST(native_byteswap16_test)
    PKSAV_TEST(byteswap32_test)
    PKSAV_TEST(native_byteswap32_test)
)
