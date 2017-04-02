/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "test-utils.h"

#include <pksav/config.h>

#include <stdio.h>
#include <string.h>

#ifdef PKSAV_PLATFORM_WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

int get_filesize(
    const char* filename,
    size_t* result_out
) {
    if(!filename || !result_out) {
        return -1;
    }

    FILE* file = fopen(filename, "r");
    if(!file) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    *result_out = ftell(file);
    fclose(file);

    return 0;
}

int read_file_into_buffer(
    const char* filename,
    uint8_t* buffer,
    size_t buffer_len
) {
    if(!filename || !buffer) {
        return -1;
    }

    FILE* file = fopen(filename, "rb");
    if(!file) {
        return -1;
    }
    PKSAV_UNUSED(size_t num_read) = fread((void*)buffer, 1, buffer_len, file);

    if(fclose(file)) {
        return -1;
    }

    return 0;
}

int do_files_differ(
    const char* filename1,
    const char* filename2,
    bool* result_out
) {
    if(!filename1 || !filename2 || !result_out) {
        return -1;
    }

    size_t file1size = 0;
    size_t file2size = 0;

    if(get_filesize(filename1, &file1size) || get_filesize(filename2, &file2size)) {
        return -1;
    }

    if(file1size == file2size) {
        uint8_t* buffer1 = malloc(file1size);
        uint8_t* buffer2 = malloc(file2size);

        if(read_file_into_buffer(filename1, buffer1, file1size)) {
            free(buffer1);
            free(buffer2);
            return -1;
        }
        if(read_file_into_buffer(filename2, buffer2, file2size)) {
            free(buffer1);
            free(buffer2);
            return -1;
        }

        *result_out = (memcmp(buffer1, buffer2, file1size) != 0);
        free(buffer1);
        free(buffer2);
        return 0;
    } else {
        *result_out = true;
        return 0;
    }

    // Should not reach here
    return -1;
}

int get_pid() {
#ifdef PKSAV_PLATFORM_WIN32
    return (int)GetCurrentProcessId();
#else
    return (int)getpid();
#endif
}

// Direct port of boost::archive::tmpdir()
const char* get_tmp_dir() {
    const char* ret = NULL;

    ret = getenv("TMP");
    if(ret) {
        return ret;
    }
    ret = getenv("TMPDIR");
    if(ret) {
        return ret;
    }
    ret = getenv("TEMP");
    if(ret) {
        return ret;
    }

    // Somehow, none of these exist, so just guess.
    ret = "/tmp";

    return ret;
}

int delete_file(
    const char* filepath
) {
    if(!filepath) {
        return -1;
    }

#ifdef PKSAV_PLATFORM_WIN32
    return (DeleteFile(filepath) != 0) ? 0 : 1;
#else
    return remove(filepath);
#endif
}
