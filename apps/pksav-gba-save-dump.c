/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pksav_getopt.h"

#include <pksav/gba.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* GBA_GAME_NAMES[] = {
    "Ruby/Sapphire", "Emerald", "FireRed/LeafGreen"
};

int main(int argc, char* argv[]) {
    printf("PKSav - Game Boy Advance Save Dump\n\n");

    if(pksav_getopt(argc, argv)) {
        return EXIT_FAILURE;
    }

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
    pksav_text_from_gba(
        gba_save.trainer_info->name,
        trainer_name, 7
    );
    printf("Trainer: %s (PID: %05d, SID: %05d)\n", trainer_name,
                                               pksav_littleendian16(gba_save.trainer_info->trainer_id.pid),
                                               pksav_littleendian16(gba_save.trainer_info->trainer_id.sid));

    printf("Time played: %02d:%02d:%02d:%02d\n", gba_save.trainer_info->time_played.hours,
                                                 gba_save.trainer_info->time_played.minutes,
                                                 gba_save.trainer_info->time_played.seconds,
                                                 gba_save.trainer_info->time_played.frames);

    printf("Money: %u\n", pksav_littleendian32(*gba_save.money));
    printf("Casino Coins: %u\n", pksav_littleendian32(*gba_save.casino_coins));

    if(gba_save.gba_game == PKSAV_GBA_FRLG) {
        char rival_name[7];
        pksav_text_from_gba(
            gba_save.rival_name,
            rival_name, 7
        );
        printf("Rival: %s\n", rival_name);
    }

    char nickname[11];
    if(pksav_getopt_party || pksav_getopt_all) {
        printf("\nPokémon Party (size %u):\n", pksav_littleendian32(gba_save.pokemon_party->count));
        for(uint8_t i = 0; i < gba_save.pokemon_party->count; ++i) {
            if(gba_save.pokemon_party->party[i].pc.blocks.misc.iv_egg_ability & PKSAV_GBA_EGG_MASK) {
                puts(" * EGG");
            } else {
                pksav_text_from_gba(
                    gba_save.pokemon_party->party[i].pc.nickname,
                    nickname, 10
                );
                printf(" * %s\n", nickname);
            }
            printf("   * Level: %d\n", gba_save.pokemon_party->party[i].party_data.level);

            pksav_text_from_gba(
                gba_save.pokemon_party->party[i].pc.otname,
                trainer_name, 7
            );
            printf("   * OT: %s (%05d)\n", trainer_name, pksav_littleendian16(gba_save.pokemon_party->party[i].pc.ot_id.pid));
        }
    }

    if(pksav_getopt_pc || pksav_getopt_all) {
        for(uint8_t i = 0; i < 14; ++i) {
            char box_name[9];
            memset(box_name, 0, 9);
            pksav_text_from_gba(
                gba_save.pokemon_pc->box_names[i],
                box_name, 9
            );

            printf("\nPokémon Box %d (%s):\n", (i+1), box_name);

            for(uint8_t j = 0; j < 30; ++j) {
                if(gba_save.pokemon_pc->boxes[i].entries[j].ot_id.id == 0) {
                    continue;
                }

                if(gba_save.pokemon_pc->boxes[i].entries[j].blocks.misc.iv_egg_ability & PKSAV_GBA_EGG_MASK) {
                    puts(" * EGG");
                } else {
                    pksav_text_from_gba(
                        gba_save.pokemon_pc->boxes[i].entries[j].nickname,
                        nickname, 10
                    );
                    printf(" * %s\n", nickname);
                }
                pksav_text_from_gba(
                    gba_save.pokemon_pc->boxes[i].entries[j].otname,
                    trainer_name, 7
                );
                printf("   * OT: %s (%05d)\n", trainer_name, pksav_littleendian16(gba_save.pokemon_pc->boxes[i].entries[j].ot_id.pid));
            }
        }
    }

    if(pksav_getopt_items || pksav_getopt_all) {
        // TODO
    }

    pksav_gba_save_free(&gba_save);

    return EXIT_SUCCESS;
}
