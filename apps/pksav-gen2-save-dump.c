/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    printf("PKSav - Generation II Save Dump\n\n");

    if(argc != 2) {
        fprintf(stderr, "Usage: pksav-gen2-save-dump <filename>\n");
    }

    const char* filepath = argv[1];
    printf("Attempting to load file at \"%s\"...", filepath);

    pksav_gen2_save_t gen2_save;
    pksav_error_t error_code = pksav_gen2_save_load(filepath, &gen2_save);
    if(error_code) {
       printf("failed with error: %s\n", pksav_strerror(error_code));
       return EXIT_FAILURE;
    } else {
        const char* save_type = (gen2_save.gen2_game == PKSAV_GEN2_CRYSTAL) ? "Crystal"
                                                                            : "Gold/Silver";
        printf("found %s save file.\n\n", save_type);
    }

    char trainer_name[8];
    memset(trainer_name, 0, 8);
    pksav_text_from_gen2(
        gen2_save.trainer_name,
        trainer_name, 7
    );
    printf("Trainer: %s (%d)\n", trainer_name, pksav_bigendian16(*gen2_save.trainer_id));

    printf("Time played: %d:%d:%d.%d\n", gen2_save.time_played->hours,
                                         gen2_save.time_played->minutes,
                                         gen2_save.time_played->seconds,
                                         gen2_save.time_played->frames);

    size_t money = pksav_from_bcd(
                       gen2_save.money,
                       3
                   );
    printf("Money: %zu\n", money);

    char rival_name[7];
    pksav_text_from_gen2(
        gen2_save.rival_name,
        rival_name, 7
    );
    printf("Rival: %s\n", rival_name);

    char nickname[11];
    char otname[8];
    memset(nickname, 0, 11);
    memset(otname, 0, 8);
    printf("\nPokémon Party (size %d):\n", gen2_save.pokemon_party->count);
    for(uint8_t i = 0; i < gen2_save.pokemon_party->count; ++i) {
        pksav_text_from_gen2(
            gen2_save.pokemon_party->nicknames[i],
            nickname, 10
        );
        pksav_text_from_gen2(
            gen2_save.pokemon_party->otnames[i],
            otname, 7
        );
        printf(" * %s\n", nickname);
        printf("   * Level: %d\n", gen2_save.pokemon_party->party[i].pc.level);
        printf("   * OT: %s (%d)\n", otname, pksav_bigendian16(gen2_save.pokemon_party->party[i].pc.ot_id));
    }

    printf("\nCurrent Pokémon Box (%d, size %d):\n",
               (*gen2_save.current_pokemon_box_num),
               gen2_save.current_pokemon_box->count);
    for(uint8_t j = 0; j < gen2_save.current_pokemon_box->count; ++j) {
        pksav_text_from_gen2(
            gen2_save.current_pokemon_box->nicknames[j],
            nickname, 10
        );
        pksav_text_from_gen2(
            gen2_save.current_pokemon_box->otnames[j],
            otname, 7
        );
        printf(" * %s\n", nickname);
        printf("   * Level: %d\n", gen2_save.current_pokemon_box->entries[j].level);
        printf("   * OT: %s (%d)\n", otname, pksav_bigendian16(gen2_save.current_pokemon_box->entries[j].ot_id));
    }

    for(uint8_t i = 0; i < 14; ++i) {
        char box_name[11];
        pksav_text_from_gen2(
            gen2_save.pokemon_box_names->names[i],
            box_name, 11
        );

        printf("\nPokémon Box %d (name: \"%s\", size: %d):\n",
               i, box_name, gen2_save.pokemon_pc->boxes[i].count);

        // Some boxes have invalid counts
        if(gen2_save.pokemon_pc->boxes[i].count > 20) {
            continue;
        }

        for(uint8_t j = 0; j < gen2_save.pokemon_pc->boxes[i].count; ++j) {
            pksav_text_from_gen2(
                gen2_save.pokemon_pc->boxes[i].nicknames[j],
                nickname, 10
            );
            pksav_text_from_gen2(
                gen2_save.pokemon_pc->boxes[i].otnames[j],
                otname, 7
            );
            printf(" * %s\n", nickname);
            printf("   * Level: %d\n", gen2_save.pokemon_pc->boxes[i].entries[j].level);
            printf("   * OT: %s (%d)\n", otname, pksav_bigendian16(gen2_save.pokemon_pc->boxes[i].entries[j].ot_id));
        }
    }

    pksav_gen2_save_free(&gen2_save);

    return EXIT_SUCCESS;
}
