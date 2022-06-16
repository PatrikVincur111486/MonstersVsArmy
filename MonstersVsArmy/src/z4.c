#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rng.h"
#include "data.h"

int main(int argc, char *argv[]) {
    srnd(atoi(argv[3]));
    if (argc == 6) {
        FILE *f;
        f = fopen(argv[5], "r");
        if (f == NULL) return 2;
        char meno[20];
        int utok, defen, temp;
        memset(meno, 0, 20);
        for (int i = 0; i < ENEMY_TYPE_COUNT; i++) {
            temp = fscanf(f, "%s %d %d", &meno, &utok, &defen);
            if (temp != 3) return 3;
            strcpy(enemy_types[i].name, meno);
            enemy_types[i].att = utok;
            enemy_types[i].def = defen;}
        fclose(f);}
    int indexm = -1;
    for (int i = 0; i < 3; i++)
        if (strcmp(argv[1], monster_types[i].name) == 0) {
            indexm = i;break;}
    if (indexm == -1) return 1;
    Unit monstrum = {monster_types + indexm, MONSTER_INITIAL_HP, 0};
    int pocet_n = atoi(argv[2]);
    int alive = pocet_n;
    Unit pole_n[pocet_n];
    for (int i = 0; i < pocet_n; i++) {
        pole_n[i].type = enemy_types + rnd(0, ENEMY_TYPE_COUNT - 1);
        pole_n[i].hp = rnd(ENEMY_MIN_INIT_HP, ENEMY_MAX_INIT_HP);
        pole_n[i].level = rnd(0, UNIT_MAX_LEVEL);}
    printf("%s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", monstrum.type->name, monstrum.type->att, monstrum.type->def,monstrum.hp, monstrum.level);
    for (int i = 0; i < pocet_n; i++)
        printf("[%d] %s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", i, pole_n[i].type->name, pole_n[i].type->att,pole_n[i].type->def, pole_n[i].hp, pole_n[i].level);
    printf("\n");
    int DMGM = 0, DMGE = 0;
    while (monstrum.hp > 0 && alive != 0) {
        int target;
        for (int i = 0; i < pocet_n; i++)
            if (pole_n[i].hp > 0) {target = i;break;}
        for (int i = 0; i < pocet_n; i++) if ((pole_n[i].hp > 0) && (pole_n[i].hp < pole_n[target].hp)) target = i;
        int DMG = ((30 + monstrum.level - pole_n[target].level) * (100 + rnd(1, monstrum.hp) + monstrum.type->att)) /(100 + rnd(1, pole_n[target].hp) + pole_n[target].type->def);
        printf("%s => %d => [%d] %s\n", monstrum.type->name, DMG, target, pole_n[target].type->name);
        DMGM = DMGM + DMG;
        pole_n[target].hp = (pole_n[target].hp) - DMG;
        if (pole_n[target].hp < 0) alive--;
        for (int i = 0; i < pocet_n; i++)
            if (pole_n[i].hp > 0) {
                DMG = ((30 + pole_n[i].level - monstrum.level) * (100 + rnd(1, pole_n[i].hp) + pole_n[i].type->att)) /(100 + rnd(1, monstrum.hp) + monstrum.type->def);
                monstrum.hp = (monstrum.hp) - DMG;
                DMGE = DMGE + DMG;
                printf("[%d] %s => %d => %s\n", i, pole_n[i].type->name, DMG, monstrum.type->name);
                if (monstrum.hp <= 0) break;}
        if (monstrum.level < UNIT_MAX_LEVEL && monstrum.hp > 0) monstrum.level = monstrum.level + 1;
        printf("\n");
        printf("%s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", monstrum.type->name, monstrum.type->att, monstrum.type->def,monstrum.hp, monstrum.level);
        for (int i = 0; i < pocet_n; i++)
            printf("[%d] %s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", i, pole_n[i].type->name, pole_n[i].type->att,pole_n[i].type->def, pole_n[i].hp, pole_n[i].level);
        printf("\n");}
    if (monstrum.hp > 0) printf("Winner: %s\n", monstrum.type->name);
    else printf("Winner: Enemy\n");
    printf("Total monster DMG: %d\nTotal enemies DMG: %d", DMGM, DMGE);
    return 0;
}