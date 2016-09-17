/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* GBA_GAME_NAMES[] = {
    "Ruby/Sapphire", "Emerald", "FireRed/LeafGreen"
};

int main(int argc, char* argv[]) {
    printf("PKSav - Game Boy Advance Save Dump\n\n");

    if(argc != 2) {
        fprintf(stderr, "Usage: pksav-gba-save-dump <filename>\n");
    }

    const char* filepath = argv[1];
    printf("Attempting to load file at \"%s\"...", filepath);

    pksav_gba_save_t gba_save;
    pksav_error_t error_code = pksav_gba_save_load(filepath, &gba_save);
    if(error_code) {
       printf("failed with error: %s\n", pksav_strerror(error_code));
       return EXIT_FAILURE;
    } else {
        printf("found %s save file.\n\n", GBA_GAME_NAMES[gba_save.gba_game]);
    }

    char trainer_name[8];
    memset(trainer_name, 0, 8);
    pksav_text_from_gba(
        gba_save.trainer_info->name,
        trainer_name, 7
    );
    printf("Trainer: %s (PID: %d, SID: %d)\n", trainer_name,
                                               pksav_littleendian16(gba_save.trainer_info->trainer_id.pid),
                                               pksav_littleendian16(gba_save.trainer_info->trainer_id.sid));

    printf("Time played: %d:%d:%d.%d\n", gba_save.trainer_info->time_played.hours,
                                         gba_save.trainer_info->time_played.minutes,
                                         gba_save.trainer_info->time_played.seconds,
                                         gba_save.trainer_info->time_played.frames);

    printf("Money: %u\n", pksav_littleendian32(*gba_save.money));

    if(gba_save.gba_game == PKSAV_GBA_FRLG) {
        char rival_name[7];
        pksav_text_from_gba(
            gba_save.rival_name,
            rival_name, 7
        );
        printf("Rival: %s\n", rival_name);
    }

    char nickname[11];
    char otname[8];
    memset(nickname, 0, 11);
    memset(otname, 0, 8);
    printf("\nPokémon Party (size %d):\n", gba_save.pokemon_party->count);
    for(uint8_t i = 0; i < gba_save.pokemon_party->count; ++i) {
        pksav_text_from_gba(
            gba_save.pokemon_party->party[i].pc.nickname,
            nickname, 10
        );
        pksav_text_from_gba(
            gba_save.pokemon_party->party[i].pc.otname,
            otname, 7
        );
        printf(" * %s\n", nickname);
        printf("   * Level: %d\n", gba_save.pokemon_party->party[i].party_data.level);
        printf("   * OT: %s (%d)\n", otname, pksav_littleendian16(gba_save.pokemon_party->party[i].pc.ot_id.pid));
    }

    for(uint8_t i = 0; i < 14; ++i) {
        printf("\nPokémon Box %d:\n", i);

        for(uint8_t j = 0; j < 30; ++j) {
            pksav_text_from_gba(
                gba_save.pokemon_pc->boxes[i].entries[j].nickname,
                nickname, 10
            );
            pksav_text_from_gba(
                gba_save.pokemon_pc->boxes[i].entries[j].otname,
                otname, 7
            );
            printf(" * %s\n", nickname);
            printf("   * OT: %s (%d)\n", otname, pksav_littleendian16(gba_save.pokemon_pc->boxes[i].entries[j].ot_id.pid));
        }
    }

    pksav_gba_save_free(&gba_save);

    return EXIT_SUCCESS;
}
