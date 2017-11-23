/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_C_TEST_COMMON_H
#define INCLUDED_C_TEST_COMMON_H

#include <Unity/unity.h>

#define PKSAV_TEST(test_func) \
{ \
    Unity.CurrentTestName = #test_func; \
    ++Unity.NumberOfTests; \
    if(TEST_PROTECT()) { \
        test_func(); \
    } \
    UnityConcludeTest(); \
}

#define PKSAV_TEST_MAIN(...) \
    int main(int argc, char** argv) { \
        (void)argc; \
        (void)argv; \
        UnityBegin(__FILE__); \
        __VA_ARGS__; \
        return (UnityEnd()); \
    }

#endif /* INCLUDED_C_TEST_COMMON_H */
