/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "fs.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

enum pksav_error pksav_fs_filesize(
    const char* filepath,
    size_t* filesize_out
)
{
    assert(filepath != NULL);
    assert(filesize_out != NULL);

    enum pksav_error error = PKSAV_ERROR_NONE;

    FILE* input_file = fopen(filepath, "rb");
    if(input_file)
    {
        if(fseek(input_file, 0, SEEK_END))
        {
            error = PKSAV_ERROR_FILE_IO;
        }
        else
        {
            long int ftell_output = ftell(input_file);
            if(ftell_output >= 0)
            {
                if(fclose(input_file))
                {
                    error = PKSAV_ERROR_FILE_IO;
                }
                else
                {
                    *filesize_out = (size_t)ftell_output;
                }
            }
            else
            {
                error = PKSAV_ERROR_FILE_IO;

                // This is already an error state, but try and close if
                // possible.
                fclose(input_file);
            }
        }
    }
    else
    {
        error = PKSAV_ERROR_FILE_IO;
    }

    return error;
}

enum pksav_error pksav_fs_read_file_to_buffer(
    const char* filepath,
    uint8_t** buffer_ptr,
    size_t* buffer_len_out
)
{
    assert(filepath != NULL);
    assert(buffer_ptr != NULL);
    assert(buffer_len_out != NULL);

    enum pksav_error error = PKSAV_ERROR_NONE;

    size_t filesize = 0;

    error = pksav_fs_filesize(filepath, &filesize);
    if(!error)
    {
        FILE* input_file = fopen(filepath, "rb");
        if(input_file)
        {
            void* file_contents = calloc(filesize, 1);
            if(file_contents)
            {
                size_t num_read = fread(file_contents, 1, filesize, input_file);
                if(num_read == filesize)
                {
                    *buffer_ptr = file_contents;
                    *buffer_len_out = filesize;
                }
                else
                {
                    error = PKSAV_ERROR_FILE_IO;
                }

                if(fclose(input_file))
                {
                    // Even if error was already set to this, the result
                    // is the same.
                    error = PKSAV_ERROR_FILE_IO;
                }
            }
            else
            {
                // TODO: internal error instead?
                error = PKSAV_ERROR_FILE_IO;

                // This is already an error state, but try and close if
                // possible.
                fclose(input_file);
            }
        }
        else
        {
            error = PKSAV_ERROR_FILE_IO;
        }
    }

    return error;
}

enum pksav_error pksav_fs_write_buffer_to_file(
    const char* filepath,
    const uint8_t* buffer,
    size_t buffer_len
)
{
    assert(filepath != NULL);
    assert(buffer != NULL);
    assert(buffer_len > 0);

    enum pksav_error error = PKSAV_ERROR_NONE;

    FILE* output_file = fopen(filepath, "wb");
    if(output_file)
    {
        size_t num_written = fwrite(
                                 buffer,
                                 1,
                                 buffer_len,
                                 output_file
                             );
        if(num_written == buffer_len)
        {
            if(fclose(output_file))
            {
                error = PKSAV_ERROR_FILE_IO;
            }
        }
        else
        {
            error = PKSAV_ERROR_FILE_IO;

            // This is already an error state, but try and close if
            // possible.
            fclose(output_file);
        }
    }
    else
    {
        error = PKSAV_ERROR_FILE_IO;
    }

    return error;
}
