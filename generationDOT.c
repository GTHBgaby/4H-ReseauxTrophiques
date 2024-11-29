#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generationDOT.h"

void lire_fichier_dot(const char* nom_fichier, Especes* especes, int* nb_especes) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../GrapheDOT/%s.dot", nom_fichier);

    FILE* f = fopen(filepath, "r");
    if (!f) {
        printf("Impossible d'ouvrir le fichier %s\n", filepath);
        return;
    }

    char ligne[MAX_LIGNE];
    *nb_especes = 0;

    while (fgets(ligne, MAX_LIGNE, f)) {
        if (strstr(ligne, "label=") && strstr(ligne, "\\n[") && !strstr(ligne, "->")) {
            char* debut_nom = strchr(ligne, '"');
            if (debut_nom) {
                debut_nom++;
                char* fin_nom = strchr(debut_nom, '"');
                if (fin_nom) {
                    size_t longueur = fin_nom - debut_nom;
                    strncpy(especes[*nb_especes].nom, debut_nom, longueur);
                    especes[*nb_especes].nom[longueur] = '\0';

                    char* pop_debut = strstr(ligne, "\\n[") + 3;
                    char* taux_debut = strstr(pop_debut, "\\n") + 2;
                    if (pop_debut) {
                        especes[*nb_especes].population = atoi(pop_debut);
                        especes[*nb_especes].taux_accroissement = atof(taux_debut);
                        /*printf("Espece trouvee : %s (population: %d)\n",
                               especes[*nb_especes].nom,
                               especes[*nb_especes].population);*/
                        (*nb_especes)++;
                    }
                }
            }
        }
    }

    printf("\nTotal des especes trouvees : %d\n", *nb_especes);
    printf("Liste des especes disponibles :\n");
    for (int i = 0; i < *nb_especes; i++) {
        printf("- %s\n  Population : %d\n  Taux d'accroissement : %.2f%%\n", especes[i].nom, especes[i].population, especes[i].taux_accroissement);
    }
    printf("\n");

    fclose(f);
}

int modifier_espece(Especes* especes, int nb_especes, const char* nom_espece) {
    int i;
    int trouve = 0;
    int choix;

    for (i = 0; i < nb_especes; i++) {
        //printf("Comparaison avec : '%s'\n", especes[i].nom);
        if (strcmp(especes[i].nom, nom_espece) == 0) {
            trouve = 1;
            printf("\nModification de l'espece %s\n", nom_espece);
            printf("Population actuelle: %d\nTaux d'accroissement: %.2f%%\n", especes[i].population, especes[i].taux_accroissement);

            do {
                printf("Que voulez-vous modifier ?\n");
                printf("1. Modifier population\n");
                printf("2. Modifier taux d'accroissement\n");
                printf("3. Modifier l'annee\n");
                printf("4. Quitter\n");
                scanf("%d", &choix);

                switch(choix) {
                    case 1:
                        printf("Nouvelle population:\n");
                        scanf("%d", &especes[i].population);
                        break;
                    case 2:
                        printf("Nouveau taux d'accroissement (%%):\n");
                        scanf("%f", &especes[i].taux_accroissement);
                        break;
                    case 3:
                        printf("Nouvelle annee:\n");
                        break;
                }
            } while (choix != 4);

            return 1;
        }
    }

    if (!trouve) {
        printf("\nL'espece '%s' n'existe pas dans ce fichier.\n", nom_espece);
        return 0;
    }
}

void mettre_a_jour_fichier_dot(const char* nom_fichier, Especes* especes, int nb_especes) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../GrapheDOT/%s.dot", nom_fichier);

    FILE* f_in = fopen(filepath, "r");
    if (!f_in) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filepath);
        return;
    }

    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "../GrapheDOT/%s.dot.tmp", nom_fichier);
    FILE* f_out = fopen(temp_filename, "w");
    if (!f_out) {
        fclose(f_in);
        printf("Erreur lors de la creation du fichier temporaire\n");
        return;
    }

    char ligne[MAX_LIGNE];
    while (fgets(ligne, MAX_LIGNE, f_in)) {
        char ligne_modifiee[MAX_LIGNE];
        strcpy(ligne_modifiee, ligne);

        for (int i = 0; i < nb_especes; i++) {
            char recherche[100];
            sprintf(recherche, "\"%s\" [label=\"%s\\n[", especes[i].nom, especes[i].nom);

            if (strstr(ligne, recherche)) {
                snprintf(ligne_modifiee, MAX_LIGNE, "    \"%s\" [label=\"%s\\n[%d]\\n%.2f%%\"]\n",
                         especes[i].nom, especes[i].nom,
                         especes[i].population, especes[i].taux_accroissement);
                break;
            }
        }
        fputs(ligne_modifiee, f_out);
    }

    fclose(f_in);
    fclose(f_out);

    remove(filepath);
    rename(temp_filename, filepath);
}

/*void generer_dot(Espece* especes, int nb_especes, int (*relations)[2], int nb_relations, const char* filename) {
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
}*/

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