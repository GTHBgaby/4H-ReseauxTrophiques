#include "lecteurFichiers.h"
#include <stdio.h>
#include <string.h>



Graph* read_ecosystem_file(const char* filename) {

    FILE* file = fopen(filename, "r");

    if (!file) {
        printf("Impossible douvrir le fichier %s\n", filename);
        return NULL;
    }

    // Lecture du nombre d'espèces

    int nbEspeces;
    fscanf(file, "%d\n", &nbEspeces);

    // Création du graphe

    Graph* g = create_graph(nbEspeces);
    if (!g) {
        printf("Erreur lors de la creation du graphe\n");
        fclose(file);
        return NULL;
    }

    // Valeurs par défaut pour l'écosystème

    strcpy(g->nom, "Ecosysteme Forestier");

    // Lecture des espèces

    char name[longueur_Max];
    for (int i = 0; i < nbEspeces; i++) {

        // On lit la ligne complète pour le nom

        if (fgets(name, longueur_Max, file) == NULL) {
            printf("Erreur de lecture pour l'espece %d\n", i);
            destroy_graph(g);
            fclose(file);
            return NULL;
        }

        // Supprimer le \n à la fin du nom si présent

        name[strcspn(name, "\n")] = 0;

        // Initialisation des valeurs par défaut de l'espèce

        Especes* sp = &g->especes[i];
        sp->id = i;
        strncpy(sp->nom, name, longueur_Max - 1);
        sp->population = 100.0;        // Population initiale par défaut
        sp->tauxDeCroissance = 0.1;         // Taux de croissance par défaut
        sp->capacite = 150.0;          // Capacité par défaut
        sp->niveauTrophique = 1;         // Niveau trophique par défaut
    }

    fclose(file);
    printf("Lecture du fichier reussie : %d especes chargees\n", nbEspeces);
    return g;
}

void print_ecosystem(Graph* g) {
    if (!g) return;

    printf("\n=== Ecosysteme : %s ===\n", g->nom);
    printf("Nombre d especes : %d\n\n", g->nbEspeces);

    printf("ESPECES :\n");
    printf("%-4s %-25s %-12s %-12s %-12s %-12s\n",
           "ID", "Nom", "Population", "Croissance", "Capacite", "Niveau");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < g->nbEspeces; i++) {
        Especes* sp = &g->especes[i];
        printf("%-4d %-25s %-12.2f %-12.2f %-12.2f %-12d\n",
               sp->id,
               sp->nom,
               sp->population,
               sp->tauxDeCroissance,
               sp->capacite,
               sp->niveauTrophique);
    }
}















