/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pksav_getopt.h"

#include <pksav/gen1.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    printf("PKSav - Generation I Save Dump\n\n");

    if(pksav_getopt(argc, argv)) {
        return EXIT_FAILURE;
    }

    printf("Attempting to load file at \"%s\"...", filepath);

    pksav_gen1_save_t gen1_save;
    pksav_error_t error_code = pksav_gen1_save_load(filepath, &gen1_save);
    if(error_code) {
       printf("failed with error: %s\n", pksav_strerror(error_code));
       return EXIT_FAILURE;
    } else {
        const char* save_type = (gen1_save.yellow) ? "Yellow" : "Red/Blue";
        printf("found %s save file.\n\n", save_type);
    }

    char trainer_name[8];
    pksav_text_from_gen1(
        gen1_save.trainer_name,
        trainer_name, 7
    );
    printf("Trainer: %s (%d)\n", trainer_name, pksav_bigendian16(*gen1_save.trainer_id));

    printf("Time played: %d:%d:%d\n", gen1_save.time_played->hours,
                                      gen1_save.time_played->minutes,
                                      gen1_save.time_played->seconds);

    size_t money = pksav_from_bcd(
                       gen1_save.money,
                       3
                   );
    printf("Money: %zu\n", money);

    char rival_name[7];
    pksav_text_from_gen1(
        gen1_save.rival_name,
        rival_name, 7
    );
    printf("Rival: %s\n", rival_name);

    size_t casino_coins = pksav_from_bcd(
                              gen1_save.casino_coins,
                              2
                          );
    printf("Casino coins: %zu\n", casino_coins);

    if(gen1_save.yellow) {
        printf("Pikachu friendship: %d\n", *gen1_save.pikachu_friendship);
    }

    printf("\nBadges:\n");
    printf(" * Boulder Badge: %s\n", ((*gen1_save.badges & PKSAV_GEN1_BOULDER_BADGE) ? "Yes" : "No"));
    printf(" * Cascade Badge: %s\n", ((*gen1_save.badges & PKSAV_GEN1_CASCADE_BADGE) ? "Yes" : "No"));
    printf(" * Thunder Badge: %s\n", ((*gen1_save.badges & PKSAV_GEN1_THUNDER_BADGE) ? "Yes" : "No"));
    printf(" * Rainbow Badge: %s\n", ((*gen1_save.badges & PKSAV_GEN1_RAINBOW_BADGE) ? "Yes" : "No"));
    printf(" * Soul Badge:    %s\n", ((*gen1_save.badges & PKSAV_GEN1_SOUL_BADGE) ? "Yes" : "No"));
    printf(" * Marsh Badge:   %s\n", ((*gen1_save.badges & PKSAV_GEN1_MARSH_BADGE) ? "Yes" : "No"));
    printf(" * Volcano Badge: %s\n", ((*gen1_save.badges & PKSAV_GEN1_VOLCANO_BADGE) ? "Yes" : "No"));
    printf(" * Earth Badge:   %s\n", ((*gen1_save.badges & PKSAV_GEN1_EARTH_BADGE) ? "Yes" : "No"));

    char nickname[11];
    if(pksav_getopt_party || pksav_getopt_all) {
        printf("\nPokémon Party (size %d):\n", gen1_save.pokemon_party->count);
        for(uint8_t i = 0; i < gen1_save.pokemon_party->count; ++i) {
            pksav_text_from_gen1(
                gen1_save.pokemon_party->nicknames[i],
                nickname, 10
            );
            pksav_text_from_gen1(
                gen1_save.pokemon_party->otnames[i],
                trainer_name, 7
            );
            printf(" * %s\n", nickname);
            printf("   * Level: %d\n", gen1_save.pokemon_party->party[i].party_data.level);
            printf("   * OT: %s (%d)\n", trainer_name, pksav_bigendian16(gen1_save.pokemon_party->party[i].pc.ot_id));
        }
    }

    if(pksav_getopt_pc || pksav_getopt_all) {
        printf("\nCurrent Pokémon Box (%d, size %d):\n",
                   (*gen1_save.current_pokemon_box_num & PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK),
                   gen1_save.current_pokemon_box->count);
        for(uint8_t j = 0; j < gen1_save.current_pokemon_box->count; ++j) {
            pksav_text_from_gen1(
                gen1_save.current_pokemon_box->nicknames[j],
                nickname, 10
            );
            pksav_text_from_gen1(
                gen1_save.current_pokemon_box->otnames[j],
                trainer_name, 7
            );
            printf(" * %s\n", nickname);
            printf("   * Level: %d\n", gen1_save.current_pokemon_box->entries[j].level);
            printf("   * OT: %s (%d)\n", trainer_name, pksav_bigendian16(gen1_save.current_pokemon_box->entries[j].ot_id));
        }

        for(uint8_t i = 0; i < 12; ++i) {
            printf("\nPokémon Box %d (size %d):\n", i, gen1_save.pokemon_boxes[i]->count);
            for(uint8_t j = 0; j < gen1_save.pokemon_boxes[i]->count; ++j) {
                pksav_text_from_gen1(
                    gen1_save.pokemon_boxes[i]->nicknames[j],
                    nickname, 10
                );
                pksav_text_from_gen1(
                    gen1_save.pokemon_boxes[i]->otnames[j],
                    trainer_name, 7
                );
                printf(" * %s\n", nickname);
                printf("   * Level: %d\n", gen1_save.pokemon_boxes[i]->entries[j].level);
                printf("   * OT: %s (%d)\n", trainer_name, pksav_bigendian16(gen1_save.pokemon_boxes[i]->entries[j].ot_id));
            }
        }
    }

    if(pksav_getopt_items || pksav_getopt_all) {
        // TODO
    }

    pksav_gen1_save_free(&gen1_save);

    return EXIT_SUCCESS;
}
