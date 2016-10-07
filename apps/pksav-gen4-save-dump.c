/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen4.h>

#include <stdio.h>
#include <stdlib.h>

static const char* GEN4_GAME_NAMES[] = {
    "Diamond/Pearl", "Platinum", "HeartGold/SoulSilver"
};

int main(int argc, char* argv[]) {
    printf("PKSav - Generation IV Save Dump\n\n");

    if(argc != 2) {
        fprintf(stderr, "Usage: pksav-gen4-save-dump <filename>\n");
    }

    const char* filepath = argv[1];
    printf("Attempting to load file at \"%s\"...", filepath);

    pksav_gen4_save_t gen4_save;
    pksav_error_t error_code = pksav_gen4_save_load(filepath, &gen4_save);
    if(error_code) {
       printf("failed with error: %s\n", pksav_strerror(error_code));
       return EXIT_FAILURE;
    } else {
        printf("found %s save file.\n\n", GEN4_GAME_NAMES[gen4_save.gen4_game]);
    }

    pksav_gen4_save_free(&gen4_save);

    return EXIT_SUCCESS;
}
