/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_C_TEST_COMMON_H
#define INCLUDED_C_TEST_COMMON_H

/*
 * Minimal test framework
 */

#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pksav-c/error.h>

static int return_code;

static char error_buffer[1024];

#ifdef PKSAV_PLATFORM_MINGW

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"

#define PKSAV_ASSERT_SIZE_TS_EQUAL(num1, num2) \
    if(num1 != num2) { \
        memset(error_buffer, '\0', 1024); \
        sprintf(error_buffer, "Failed assertion: %s (%Iu) != %s (%Iu)", #num1, num1, #num2, num2); \
        longjmp(env, 1); \
    }

#else

#define PKSAV_ASSERT_SIZE_TS_EQUAL(num1, num2) \
    if(num1 != num2) { \
        memset(error_buffer, '\0', 1024); \
        sprintf(error_buffer, "Failed assertion: %s (%zu) != %s (%zu)", #num1, num1, #num2, num2); \
        longjmp(env, 1); \
    }

#endif

#define PKSAV_EXEC(...) \
    if(__VA_ARGS__) { \
        longjmp(env, 2); \
    }

#define PKSAV_ASSERT(...) \
    if(!(__VA_ARGS__)) { \
        memset(error_buffer, '\0', 1024); \
        sprintf(error_buffer, "Failed assertion: %s", #__VA_ARGS__); \
        longjmp(env, 1); \
    }

#define PKSAV_ASSERT_INTS_EQUAL(num1, num2) \
    if(num1 != num2) { \
        memset(error_buffer, '\0', 1024); \
        sprintf(error_buffer, "Failed assertion: %s (%d) != %s (%d)", #num1, num1, #num2, num2); \
        longjmp(env, 1); \
    }

#define PKSAV_ASSERT_FLOATS_CLOSE(f1, f2) \
    if(fabs(f1 - f2) > 0.0001) { \
        memset(error_buffer, '\0', 1024); \
        sprintf(error_buffer, "Failed assertion: %s (%f) != %s (%f)", #f1, f1, #f2, f2); \
        longjmp(env, 1); \
    }

#define PKSAV_ASSERT_STRINGS_EQUAL(str1, str2) \
    if(strcmp(str1, str2)) { \
        memset(error_buffer, '\0', 1024); \
        sprintf(error_buffer, "Failed assertion: \"%s\" != \"%s\"", str1, str2); \
        longjmp(env, 1); \
    }

#define PKSAV_REQUIRE_ERROR(...) \
    if(!(__VA_ARGS__)) { \
        memset(error_buffer, '\0', 1024); \
        sprintf(error_buffer, "This code should have failed: %s", #__VA_ARGS__); \
        longjmp(env, 1); \
    }

#define PKSAV_TEST(test_name, ...) \
    { \
        printf("Starting test %s...", #test_name); \
        jmp_buf env; \
        switch(setjmp(env)) { \
            case 0: {\
                __VA_ARGS__; \
                puts("success.\n"); \
                break; \
            } \
            case 1: \
                return_code = EXIT_FAILURE; \
                printf("failed with error:\n%s\n\n", error_buffer); \
                break; \
            default: \
                return_code = EXIT_FAILURE; \
                pksav_strerror(exception_buffer, 1024, &actual_exception_len); \
                printf("unknown failure.\n"); \
                break; \
        } \
    }

#define PKSAV_TEST_MAIN(...) \
    int main(int argc, char** argv) { \
        (void)argc; \
        (void)argv; \
        return_code = EXIT_SUCCESS; \
        __VA_ARGS__ \
        return return_code; \
    }

#endif /* INCLUDED_C_TEST_COMMON_H */
