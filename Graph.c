#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

#define TAUX_BASE 15
#define POP_BASE 100
#define CAP_BASE 100

Graph* lireGraphFichier(const char* nomFichier, Arc* man) {
    int nbArrete, n, p, a;
    FILE* fichier = fopen("../CoursDeau.txt", "r");
    if (!fichier) return NULL;

    Graph* graph = malloc(sizeof(Graph));

    // Lecture du nombre d'espèces
    fscanf(fichier, "%d", &graph->nbEspeces);
    fscanf(fichier, "%d", &nbArrete);

    // Allouer les matrices
    // +1 pour tenir compte de l'indexation à partir de 1
    graph->especes = malloc((graph->nbEspeces + 1) * sizeof(Especes));

    // Initialisation des espèces
    for (int i = 0; i <= graph->nbEspeces; i++) {  // Commencer à 0 pour éviter les problèmes d'accès
        graph->especes[i].id = i;
        graph->especes[i].nom[0] = '\0';  // Initialiser le nom comme chaîne vide

        // Initialiser les tableaux pred et suc
        for (int j = 0; j < MAX_connexion; j++) {
            graph->especes[i].pred[j] = -1;
            graph->especes[i].suc[j] = -1;
        }
        graph->especes[i].arc = NULL;
        graph->especes[i].niveauTrophique = 0;
        graph->especes[i].population = POP_BASE;
        graph->especes[i].taux_accroissement = TAUX_BASE;
        graph->especes[i].capacite = CAP_BASE;
    }

    // Lecture des noms à partir de l'index 1
    for (int i = 1; i <= graph->nbEspeces; i++) {
        fscanf(fichier, "%s", graph->especes[i].nom);
    }

    // Lecture des arêtes
    for (int i = 0; i < nbArrete; i++) {
        a = 0;  // Commencer à l'index 0 pour les tableaux pred et suc
        fscanf(fichier, "%d%d", &n, &p);

        // Vérifier que les indices sont valides
        if (n > 0 && n <= graph->nbEspeces && p > 0 && p <= graph->nbEspeces) {
            while (a < MAX_connexion && graph->especes[n].pred[a] != -1) {
                ++a;
            }
            if (a < MAX_connexion) {
                graph->especes[n].pred[a] = p;
            }

            a = 0;
            while (a < MAX_connexion && graph->especes[p].suc[a] != -1) {
                ++a;
            }
            if (a < MAX_connexion) {
                graph->especes[p].suc[a] = n;
            }
        }
    }

    // Création des arcs
    for (int i = 1; i <= graph->nbEspeces; i++) {
        Arc* dernierArc = NULL;
        for (int j = 0; j < MAX_connexion && graph->especes[i].suc[j] != -1; j++) {
            Arc* nouvelArc = (Arc*)malloc(sizeof(Arc));
            if (!nouvelArc) continue;

            nouvelArc->IDb = i;
            nouvelArc->IDs = graph->especes[i].suc[j];
            nouvelArc->infl = 5.0;
            nouvelArc->arcsuivant = NULL;

            if (!graph->especes[i].arc) {
                graph->especes[i].arc = nouvelArc;
            } else {
                dernierArc->arcsuivant = nouvelArc;
            }
            dernierArc = nouvelArc;
        }
    }

    fclose(fichier);
    return graph;
}

void printEcosysteme(Graph* g) {
    if (g == NULL) {
        printf("Erreur: Graphe invalide\n");
        return;
    }

    // Affichage du titre
    printf("\n=== Ecosysteme ===\n");
    printf("Nombre d'especes: %d\n\n", g->nbEspeces);

    // Affichage des espèces
    printf("LISTE DES ESPECES:\n");
    printf("------------------\n");
    for (int i = 1; i <= g->nbEspeces; i++) {
        if (g->especes[i].nom[0] != '\0') {  // Vérifier que l'espèce existe
            printf("Espece %d: %s\n", g->especes[i].id, g->especes[i].nom);
            printf("  Population: %.2f\n", g->especes[i].population);
            printf("  Taux de croissance: %.2f\n", g->especes[i].taux_accroissement);
            printf("  Capacite maximale: %.2f\n", g->especes[i].capacite);
            printf("  Niveau trophique: %d\n\n", g->especes[i].niveauTrophique);
        }
    }

    // Affichage des interactions
    printf("SUCCESSEURS ET PREDECESSEURS:\n");
    printf("----------------------------\n");
    for (int i = 1; i <= g->nbEspeces; i++) {
        if (g->especes[i].nom[0] != '\0') {  // Vérifier que l'espèce existe
            printf("Espece %d (%s):\n", g->especes[i].id, g->especes[i].nom);

            // Affichage des successeurs
            printf("  Successeurs: ");
            for (int j = 0; j < MAX_connexion && g->especes[i].suc[j] != -1; j++) {
                printf("%d ", g->especes[i].suc[j]);
            }
            printf("\n");

            // Affichage des prédécesseurs
            printf("  Predecesseurs: ");
            for (int j = 0; j < MAX_connexion && g->especes[i].pred[j] != -1; j++) {
                printf("%d ", g->especes[i].pred[j]);
            }
            printf("\n\n");
        }
    }
}



void libererGraph(Graph* graph) {
    if (!graph) return;

    for (int i = 1; i <= graph->nbEspeces; i++) {
        Arc* courant = graph->especes[i].arc;
        while (courant) {
            Arc* suivant = courant->arcsuivant;
            free(courant);
            courant = suivant;
        }
    }
    free(graph->especes);
    free(graph);
}