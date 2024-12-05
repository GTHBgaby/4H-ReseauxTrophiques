#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "star.h"


void choix_a_star() {
    int choix = 0;
    char* fichier = NULL;

    do {


        system("cls");
        printf("Choisissez l'environnement pour l'algorithme A* :\n");
        printf("1. Cours d'eau\n");
        printf("2. Foret Europeenne\n");
        printf("3. Savane\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                fichier = "../CoursDeau.txt";  // Fichier pour Cours d'eau
                break;
            case 2:
                fichier = "../ForetEuropeenne.txt";  // Fichier pour Forêt Européenne
                break;
            case 3:
                fichier = "../Savane.txt";  // Fichier pour Savane
                break;
            default:
                printf("Choix invalide. Veuillez choisir un numero valide.\n\n");
                continue;  // Continue tant que l'utilisateur n'a pas fait un bon choix
        }
    } while (choix < 1 || choix > 3);

    // Appeler la fonction A_star avec le fichier choisi
    A_star(fichier);
}

// Fonction A* intégrée avec chargement de fichier
void A_star(const char* filename) {
    // Charger le graphe à partir du fichier
    Graph* graph = lireGraphFichier(filename);
    if (!graph) {
        printf("Erreur : Impossible de charger le fichier %s.\n", filename);
        return;
    }

    // Afficher les espèces et leurs identifiants
    printf("\nListe des especes et leurs identifiants :\n");
    for (int i = 1; i <= graph->nbEspeces; i++) {
        printf("%d : %s\n", i, graph->especes[i].nom);
    }

    // Demander à l'utilisateur de choisir deux espèces
    int sourceId, destId;
    printf("\nEntrez le numero de l'espece source : ");
    scanf("%d", &sourceId);

    if (sourceId < 1 || sourceId > graph->nbEspeces) {
        printf("Erreur : numero source invalide.\n");
        libererGraph(graph);
        return;
    }

    printf("Entrez le numero de l'espece destination : ");
    scanf("%d", &destId);

    if (destId < 1 || destId > graph->nbEspeces) {
        printf("Erreur : numero destination invalide.\n");
        libererGraph(graph);
        return;
    }

    // Initialisation des structures de données pour A*
    double* dist = malloc((graph->nbEspeces + 1) * sizeof(double)); // Distance minimale
    int* prev = malloc((graph->nbEspeces + 1) * sizeof(int)); // Prédécesseur
    bool* visited = malloc((graph->nbEspeces + 1) * sizeof(bool)); // Visité ou non
    double* heuristic = malloc((graph->nbEspeces + 1) * sizeof(double)); // Heuristique

    for (int i = 1; i <= graph->nbEspeces; i++) {
        dist[i] = (i == sourceId) ? 0 : INT_MAX;
        prev[i] = -1;
        visited[i] = false;
        heuristic[i] = 0;  // Vous pouvez définir une heuristique ici
    }

    // A* algorithm
    for (int i = 1; i <= graph->nbEspeces; i++) {
        int u = -1;
        double minDist = INT_MAX;

        // Trouver le sommet avec la distance minimale + heuristique
        for (int j = 1; j <= graph->nbEspeces; j++) {
            if (!visited[j] && (dist[j] < minDist)) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break; // Aucun sommet à visiter
        visited[u] = true;

        // Mise à jour des voisins
        Arc* arc_courant = graph->especes[u].arc;
        while (arc_courant != NULL) {
            int v = arc_courant->IDs;
            double alt = dist[u] + arc_courant->infl;
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
            }
            arc_courant = arc_courant->arcsuivant;
        }
    }

    // Affichage du chemin
    if (dist[destId] == INT_MAX) {
        printf("Aucun chemin trouve entre %s et %s.\n", graph->especes[sourceId].nom, graph->especes[destId].nom);
    } else {
        printf("Le chemin le plus rapide entre %s et %s est :\n", graph->especes[sourceId].nom, graph->especes[destId].nom);
        int path[graph->nbEspeces + 1];
        int pathIndex = 0;
        for (int v = destId; v != -1; v = prev[v]) {
            path[pathIndex++] = v;
        }

        printf("Distance totale : %.2f\n", dist[destId]);
        printf("Chemin : ");
        for (int i = pathIndex - 1; i >= 0; i--) {
            printf("%s ", graph->especes[path[i]].nom);
        }
        printf("\n");
    }

    // Libération des ressources
    free(dist);
    free(prev);
    free(visited);
    free(heuristic);
    libererGraph(graph);
}
