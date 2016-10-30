/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen4.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    char trainer_name[8];
    memset(trainer_name, 0, 8);
    pksav_text_from_gen4(
        gen4_save.trainer_name,
        trainer_name, 7
    );
    printf("Trainer: %s (PID: %05d, SID: %05d)\n",
           trainer_name,
           (int)pksav_littleendian16(gen4_save.trainer_id->pid),
           (int)pksav_littleendian16(gen4_save.trainer_id->sid));

    // TODO: time played

    printf("Money: %u\n", pksav_littleendian32(*gen4_save.money));

    char rival_name[7];
    pksav_text_from_gen4(
        gen4_save.rival_name,
        rival_name, 7
    );
    printf("Rival: %s\n", rival_name);

    if(gen4_save.gen4_game == PKSAV_GEN4_HGSS) {
        printf("\nJohto Badges:\n");
        printf(" * Zephyr Badge:  %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_ZEPHYR_BADGE) ? "Yes" : "No"));
        printf(" * Hive Badge:    %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_HIVE_BADGE) ? "Yes" : "No"));
        printf(" * Plain Badge:   %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_PLAIN_BADGE) ? "Yes" : "No"));
        printf(" * Fog Badge:     %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_FOG_BADGE) ? "Yes" : "No"));
        printf(" * Storm Badge:   %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_STORM_BADGE) ? "Yes" : "No"));
        printf(" * Mineral Badge: %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_MINERAL_BADGE) ? "Yes" : "No"));
        printf(" * Glacier Badge: %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_GLACIER_BADGE) ? "Yes" : "No"));
        printf(" * Rising Badge:  %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_RISING_BADGE) ? "Yes" : "No"));

        printf("\nKanto Badges:\n");
        printf(" * Boulder Badge: %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_BOULDER_BADGE) ? "Yes" : "No"));
        printf(" * Cascade Badge: %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_CASCADE_BADGE) ? "Yes" : "No"));
        printf(" * Thunder Badge: %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_THUNDER_BADGE) ? "Yes" : "No"));
        printf(" * Rainbow Badge: %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_RAINBOW_BADGE) ? "Yes" : "No"));
        printf(" * Soul Badge:    %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_SOUL_BADGE) ? "Yes" : "No"));
        printf(" * Marsh Badge:   %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_MARSH_BADGE) ? "Yes" : "No"));
        printf(" * Volcano Badge: %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_VOLCANO_BADGE) ? "Yes" : "No"));
        printf(" * Earth Badge:   %s\n", ((*gen4_save.hgss_kanto_badges & PKSAV_GEN4_EARTH_BADGE) ? "Yes" : "No"));
    } else {
        printf("\nBadges:\n");
        printf(" * Zephyr Badge: %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_COAL_BADGE) ? "Yes" : "No"));
        printf(" * Forest Badge: %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_FOREST_BADGE) ? "Yes" : "No"));
        printf(" * Cobble Badge: %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_COBBLE_BADGE) ? "Yes" : "No"));
        printf(" * Fen Badge:    %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_FEN_BADGE) ? "Yes" : "No"));
        printf(" * Relic Badge:  %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_RELIC_BADGE) ? "Yes" : "No"));
        printf(" * Mine Badge:   %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_MINE_BADGE) ? "Yes" : "No"));
        printf(" * Icicle Badge: %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_ICICLE_BADGE) ? "Yes" : "No"));
        printf(" * Beacon Badge: %s\n", ((*gen4_save.sinnoh_johto_badges & PKSAV_GEN4_BEACON_BADGE) ? "Yes" : "No"));
    }

    char nickname[11];
    char otname[8];
    memset(nickname, 0, 11);
    memset(otname, 0, 8);
    printf("\nPokémon Party (size %d):\n", pksav_littleendian32(gen4_save.pokemon_party->count));
    for(uint8_t i = 0; i < pksav_littleendian32(gen4_save.pokemon_party->count); ++i) {
        pksav_text_from_gen4(
            gen4_save.pokemon_party->party[i].pc.blocks.blockC.nickname,
            nickname, 10
        );
        pksav_text_from_gen4(
            gen4_save.pokemon_party->party[i].pc.blocks.blockD.otname,
            otname, 7
        );
        printf(" * %s\n", nickname);
        printf("   * Level: %d\n", gen4_save.pokemon_party->party[i].party_data.level);
        printf("   * OT: %s (PID: %d, SID: %d)\n", otname, 
           (int)pksav_littleendian16(gen4_save.pokemon_party->party[i].pc.blocks.blockA.ot_id.pid),
           (int)pksav_littleendian16(gen4_save.pokemon_party->party[i].pc.blocks.blockA.ot_id.sid));
    }

    char box_name[11];
    printf("\nPokémon PC:\n");
    for(uint8_t i = 0; i < 18; ++i) {
        if(gen4_save.gen4_game == PKSAV_GEN4_HGSS) {
            pksav_text_from_gen4(
                gen4_save.pokemon_pc->hgss.box_names[i].name,
                box_name, 11
            );
        } else {
            pksav_text_from_gen4(
                gen4_save.pokemon_pc->dppt.box_names[i].name,
                box_name, 11
            );
        }

        printf("Box %d (%s)\n", (int)(i+1), box_name);

        for(uint8_t j = 0; j < 30; ++j) {
            pksav_text_from_gen4(
                gen4_save.pokemon_pc->hgss.boxes[i].entries[j].blocks.blockC.nickname,
                nickname, 10
            );
            printf(" * %s\n", nickname);
        }
    }

    pksav_gen4_save_free(&gen4_save);

    return EXIT_SUCCESS;
}
