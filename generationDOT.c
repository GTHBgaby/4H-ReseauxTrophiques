#include <stdio.h>
#include "generationDOT.h"

void generer_dot(Espece* especes, int nb_especes, int (*relations)[2], int nb_relations, const char* filename) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../GrapheDOT/%s.dot", filename);

    FILE* f = fopen(filepath, "w");
    if (!f) return;

    fprintf(f, "digraph ReseauTrophique {\n");
    fprintf(f, "    rankdir=BT;\n");
    fprintf(f, "    node [shape=box, style=filled];\n");

    for (int i = 0; i < nb_especes; i++) {
        fprintf(f, "    %s [fillcolor=%s, label=\"%s\\n[%d]\\n%.1f%%\"];\n",
                especes[i].nom,
                especes[i].couleur,
                especes[i].nom,
                especes[i].population,
                especes[i].taux_accroissement);
    }

    for (int i = 0; i < nb_relations; i++) {
        fprintf(f, "    %s -> %s;\n",
                especes[relations[i][0]].nom,
                especes[relations[i][1]].nom);
    }

    fprintf(f, "}\n");
    fclose(f);
}

void modifier_espece(Espece* espece) {
    printf("Population actuelle de %s: %d\n", espece->nom, espece->population);
    printf("Nouvelle population: ");
    scanf("%d", &espece->population);

    printf("Taux d'accroissement actuel: %.1f%%\n", espece->taux_accroissement);
    printf("Nouveau taux d'accroissement (%%): ");
    scanf("%f", &espece->taux_accroissement);
}

/*
int main() {
    Espece especes[] = {
            {"phytoplancton", "lightgreen", 1, 100, 50.0},
            {"zooplancton", "lightyellow", 2, 100, 50.0}
    };

    int relations[][2] = {{0, 1}};
    char filename[100];
    int choix;

    printf("Nom du fichier de sortie (sans extension): ");
    scanf("%s", filename);

    do {
        printf("\n1. Modifier phytoplancton\n");
        printf("2. Modifier zooplancton\n");
        printf("3. Générer le graphe\n");
        printf("4. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                modifier_espece(&especes[0]);
                break;
            case 2:
                modifier_espece(&especes[1]);
                break;
            case 3:
                generer_dot(especes, 2, relations, 1, filename);
                printf("Graphe généré dans %s.dot\n", filename);
                break;
        }
    } while (choix != 4);

    return 0;
}
*/