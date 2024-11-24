#include "graph.h"
#include <stdio.h>
#include <stdlib.h>


Graph* create_graph(int nbEspeces) {

    // Vérification du nombre d'espèces

    if (nbEspeces <= 0 || nbEspeces > MAX_especes) {
        printf("Erreur : nombre d'espèces invalide\n");
        return NULL;
    }

    // Allocation de la structure principale

    Graph* g = malloc(sizeof(Graph));
    if (!g) {
        printf("Erreur : allocation du graphe échouée\n");
        return NULL;
    }

    // Allocation du tableau d'espèces

    g->especes = malloc(nbEspeces * sizeof(Especes));
    if (!g->especes) {
        printf("Erreur : allocation des espèces échouée\n");
        free(g);
        return NULL;
    }

    // Allocation de la matrice d'adjacence

    g->adjacence = malloc(nbEspeces * sizeof(int*));
    if (!g->adjacence) {
        printf("Erreur : allocation de la matrice d'adjacence échouée\n");
        free(g->especes);
        free(g);
        return NULL;
    }

    // Allocation des lignes de la matrice d'adjacence

    for (int i = 0; i < nbEspeces; i++) {
        g->adjacence[i] = calloc(nbEspeces, sizeof(int));
        if (!g->adjacence[i]) {
            // En cas d'erreur, libérer tout ce qui a été alloué
            for (int j = 0; j < i; j++) {
                free(g->adjacence[j]);
            }
            free(g->adjacence);
            free(g->especes);
            free(g);
            printf("Erreur : allocation de la ligne %d échouée\n", i);
            return NULL;
        }
    }

    // Initialisation des autres champs

    g->nbEspeces = nbEspeces;
    g->nom[0] = '\0';  // Chaîne vide pour le nom

    return g;
}

// Fonction complémentaire pour libérer la mémoire

void destroy_graph(Graph* g) {
    if (!g) return;

    // Libération de la matrice d'adjacence

    if (g->adjacence) {
        for (int i = 0; i < g->nbEspeces; i++) {
            free(g->adjacence[i]);
        }
        free(g->adjacence);
    }

    // Libération du tableau d'espèces

    free(g->especes);

    // Libération de la structure principale

    free(g);
}