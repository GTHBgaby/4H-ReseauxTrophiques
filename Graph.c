#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

#define TAUX_BASE 15
#define POP_BASE 100
#define CAP_BASE 100

Graph* lireGraphFichier(const char* nomFichier, Arc* man) {
    int nbArrete, n , p, a;
    FILE* fichier = fopen("../CoursDeau.txt", "r");
    if (!fichier) return NULL;

    Graph* graph = malloc(sizeof(Graph));

    // Lecture du nombre d'espèces
    fscanf(fichier, "%d" ,&graph->nbEspeces);
    fscanf(fichier, "%d" , &nbArrete);

    // Allouer les matrices
    graph->especes = malloc(graph->nbEspeces * sizeof(Especes));

    for (int i = 1; i <= graph->nbEspeces; i++) {
        graph->especes[i].id = i;
        fscanf(fichier, "%s", graph->especes[i].nom);

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
    for (int i = 0; i < nbArrete; i++) {
        a = 1;
        fscanf(fichier, "%d%d", &n, &p);
        while(graph->especes[n].pred[a] != -1){
            ++a;
        }
        graph->especes[n].pred[a] = p;
        a = 1;
        while(graph->especes[p].suc[a] != -1){
            ++a;
        }
        graph->especes[p].suc[a] = n;
    }
    for (int i = 1; i <= graph->nbEspeces; i++) {
        Arc* dernierArc = NULL;
        for (int j = 0; graph->especes[i].suc[j] != -1 && j < MAX_connexion; j++) {
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
    if(graph->especes[3].arc == NULL){printf("prout");}
    fclose(fichier);
    return graph;
}
Especes* CreerToutesAretes(Graph* graph) {
    // Créer les nouveaux arcs
    for (int i = 0; i < graph->nbEspeces; i++) {
        Arc* dernierArc = NULL;

        for (int j = 0; graph->especes[i].suc[j] != -1 && j < MAX_connexion; j++) {
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

    return graph->especes;
}
void printEcosysteme(Graph* g) {
    if (g == NULL) {
        printf("Erreur: Graphe invalide\n");
        return;
    }

    // Affichage du titre
    printf("\n=== Ecosysteme: %s ===\n", g->nom);
    printf("Nombre despeces: %d\n\n", g->nbEspeces);

    // 1. Affichage de la liste des sommets (espèces) avec leurs informations
    printf("LISTE DES ESPECES:\n");
    printf("------------------\n");
    for (int i = 1; i <= g->nbEspeces; i++) {
        Especes esp = g->especes[i];
        printf("Espece %d: %s\n", esp.id, esp.nom);
        printf("  Population: %.2f\n", esp.population);
        printf("  Taux de croissance: %.2f\n", esp.taux_accroissement);
        printf("  Capacite maximale: %.2f\n", esp.capacite);
        printf("  Niveau trophique: %d\n\n", esp.niveauTrophique);
    }

    // 2. Affichage de la liste des arcs avec leurs pondérations
    printf("LISTE DES INTERACTIONS:\n");
    printf("----------------------\n");
    for (int i = 1; i <= g->nbEspeces; i++) {
        Arc* arc_courant = g->especes[i].arc;
        while (arc_courant != NULL) {
            printf("Arc %d -> %d: Influence = %.2f\n",
                   arc_courant->IDb,
                   arc_courant->IDs,
                   arc_courant->infl);
            arc_courant = arc_courant->arcsuivant;
        }
    }
    printf("\n");

    // 3. Pour chaque sommet, affichage des successeurs et prédécesseurs
    printf("SUCCESSEURS ET PREDECESSEURS:\n");
    printf("----------------------------\n");
    for (int i = 1; i <= g->nbEspeces; i++) {
        Especes esp = g->especes[i];
        printf("Espece %d (%s):\n", esp.id, esp.nom);

        // Affichage des successeurs
        printf("  Successeurs: ");
        int j = 0;
        while (j < MAX_connexion && esp.suc[j] != 0) {
            if(esp.suc[j]!=-1){
                printf("%d ", esp.suc[j]);
            }
            j++;
        }
        printf("\n");

        // Affichage des prédécesseurs
        printf("  Predecesseurs: ");
        j = 0;
        while (j < MAX_connexion && esp.pred[j] != 0) {
            if(esp.pred[j]!=-1){
                printf("%d ", esp.pred[j]);
            }
            j++;
        }
        printf("\n\n");
    }
}

void libererGraph(Graph* graph) {
    for (int i = 1; i <= graph->nbEspeces; i++) {
        // Libérer les arcs
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

