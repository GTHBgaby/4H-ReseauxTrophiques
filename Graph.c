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
    printf("\n%d %d", graph->nbEspeces, nbArrete);

    // Allouer les matrices
    graph->especes = malloc(graph->nbEspeces * sizeof(Especes));

    for (int i = 1; i <= graph->nbEspeces; i++) {
        graph->especes[i].id = i;
        fscanf(fichier, "%s", graph->especes[i].nom);
        printf("\n %s", graph->especes[i].nom);

        // Initialiser les tableaux pred et suc
        for (int j = 0; j < MAX_connexion; j++) {
            graph->especes[i].pred[j] = -1;
            graph->especes[i].suc[j] = -1;
        }
        graph->especes[i].arc = NULL;
        graph->especes[i].population = POP_BASE;
        graph->especes[i].tauxDeCroissance = TAUX_BASE;
        graph->especes[i].capacite = CAP_BASE;
    }
    for (int i = 0; i < nbArrete -1; i++) {
        a = 0;
        fscanf(fichier, "%d%d", &n, &p);
        printf("\n%d %d",n ,p);
        while(graph->especes[n].pred[a] != -1){
            ++a;
        }
        graph->especes[n].pred[a] = p;
        a = 0;
        while(graph->especes[p].suc[a] != -1){
            ++a;
        }
        graph->especes[p].suc[a] = n;
        graph->especes = CreerArete(graph->especes, n, p, man);
    }
    fclose(fichier);
    return graph;
}
// Ajouter l'arête entre les sommets s1 et s2 du graphe
Especes* CreerArete(Especes* sommet,int s1,int s2,Arc* ark)
{
    if(1)
    {
        printf("pipi");
        printf("caca");
        ark->IDb=s2;
        ark->IDs=s1;
        ark->arcsuivant = NULL;
        ark->infl = 5.0;
        sommet[s2].arc = ark;
        return sommet;
    }

    else
    {
        Arc* temp = sommet[s2].arc;
        while(temp->arcsuivant !=NULL )
        {
            temp = temp->arcsuivant;
        }
        Arc* Newarc = malloc(sizeof(Arc));
        Newarc->IDb=s2;
        Newarc->IDs=s1;
        Newarc->arcsuivant = NULL;
        Newarc->infl = 5.0;
        temp->arcsuivant=Newarc;
        return sommet;
    }
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
    for (int i = 0; i < g->nbEspeces; i++) {
        Especes esp = g->especes[i];
        printf("Espece %d: %s\n", esp.id, esp.nom);
        printf("  Population: %.2f\n", esp.population);
        printf("  Taux de croissance: %.2f\n", esp.tauxDeCroissance);
        printf("  Capacite maximale: %.2f\n", esp.capacite);
        printf("  Niveau trophique: %d\n\n", esp.niveauTrophique);
    }

    // 2. Affichage de la liste des arcs avec leurs pondérations
    printf("LISTE DES INTERACTIONS:\n");
    printf("----------------------\n");
    for (int i = 0; i < g->nbEspeces; i++) {
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
    for (int i = 0; i < g->nbEspeces; i++) {
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

