/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav/math/base256.h>
#include <pksav/math/bcd.h>

PKSAV_TEST_MAIN(
    PKSAV_TEST(base256_test,
        size_t num1 = 1338917;
        uint8_t base256_buffer1[3] = {20, 110, 37};

        size_t num2 = 0;
        uint8_t base256_buffer2[3] = {0, 0, 0};

        pksav_to_base256(num1, base256_buffer2);
        PKSAV_ASSERT(!memcmp(base256_buffer1, base256_buffer2, 3));
        num2 = pksav_from_base256(base256_buffer2, 3);
        PKSAV_ASSERT_SIZE_TS_EQUAL(num2, (size_t)1338917);
    )

    PKSAV_TEST(bcd_test_odd_count,
        size_t num1 = 23456;
        uint8_t bcd_buffer1[3] = {0x02, 0x34, 0x56};

        size_t num2 = 0;
        uint8_t bcd_buffer2[3] = {0x00, 0x00, 0x00};

        pksav_to_bcd(num1, bcd_buffer2);

        PKSAV_ASSERT(!memcmp(bcd_buffer1, bcd_buffer2, 3));
        num2 = pksav_from_bcd(bcd_buffer2, 3);
        PKSAV_ASSERT_SIZE_TS_EQUAL(num1, num2);
    )

    PKSAV_TEST(bcd_test_even_count,
        size_t num1 = 123456;
        uint8_t bcd_buffer1[3] = {0x12, 0x34, 0x56};

        size_t num2 = 0;
        uint8_t bcd_buffer2[3] = {0x00, 0x00, 0x00};

        pksav_to_bcd(num1, bcd_buffer2);

        PKSAV_ASSERT(!memcmp(bcd_buffer1, bcd_buffer2, 3));
        num2 = pksav_from_bcd(bcd_buffer2, 3);
    )
)
