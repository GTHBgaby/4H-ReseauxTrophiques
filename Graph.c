#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "generationDOT.h"

#define MAX_ANIMAUX 100
#define MAX_LIGNES 100
#define TAUX_BASE 30
#define POP_BASE 100.00
#define CAP_BASE 1200.00

int choix = 0;

Graph* lireGraphFichier(const char* nomFichier) {
    int nbArrete, n, p, a;
    double coef;
    FILE* fichier = fopen(nomFichier, "r");
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
        graph->especes[i].supp = false;
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
        fscanf(fichier, "%d%d%lf", &n, &p, &coef);

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

            // Création du nouvel arc
            Arc* nouveauArc = (Arc*)malloc(sizeof(Arc));
            if (nouveauArc != NULL) {
                nouveauArc->IDb = p;     // ID de base (source)
                nouveauArc->IDs = n;     // ID suivant (destination)
                nouveauArc->infl = coef; // Coefficient d'influence
                nouveauArc->arcsuivant = NULL;

                // Ajout de l'arc à la liste des arcs de l'espèce p
                if (graph->especes[p].arc == NULL) {
                    graph->especes[p].arc = nouveauArc;
                } else {
                    // On ajoute l'arc à la fin de la liste
                    Arc* current = graph->especes[p].arc;
                    while (current->arcsuivant != NULL) {
                        current = current->arcsuivant;
                    }
                    current->arcsuivant = nouveauArc;
                }
            } else {
                printf("Erreur d'allocation memoire pour l'arc\n");
            }
        }
    }

    // Création des arcs
    /*for (int i = 1; i <= graph->nbEspeces; i++) {
        Arc* dernierArc = NULL;
        for (int j = 0; j < MAX_connexion && graph->especes[i].suc[j] != -1; j++) {
            Arc* nouvelArc = (Arc*)malloc(sizeof(Arc));
            if (!nouvelArc) continue;

            nouvelArc->IDb = i;
            nouvelArc->IDs = graph->especes[i].suc[j];
            nouvelArc->infl = coef;
            nouvelArc->arcsuivant = NULL;

            if (!graph->especes[i].arc) {
                graph->especes[i].arc = nouvelArc;
            } else {
                dernierArc->arcsuivant = nouvelArc;
            }
            dernierArc = nouvelArc;
        }
    }*/
    fclose(fichier);
    return graph;
}

void printEcosysteme(Graph* g) {
    int fini;
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
    // 2. Affichage de la liste des arcs avec leurs pondérations
    printf("LISTE DES INTERACTIONS:\n");
    printf("----------------------\n");
    for (int i = 1; i <= g->nbEspeces; i++) {
            Arc *arc_courant = g->especes[i].arc;
            while (arc_courant != NULL) {
                printf("Arc %d -> %d: Influence = %.2f\n",
                       arc_courant->IDb,
                       arc_courant->IDs,
                       arc_courant->infl);
                arc_courant = arc_courant->arcsuivant;
            }
    }
    printf("\n");

    // Affichage des interactions
    printf("SUCCESSEURS ET PREDECESSEURS:\n");
    printf("----------------------------\n");
    for (int i = 1; i <= g->nbEspeces; i++) {
        if (g->especes[i].supp == false) {  // Vérifier que l'espèce existe
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
        }else{
            printf("Espece supprimee\n\n");
        }
    }
    printf("\nAppuyer sur n'importe quelle touche pour continuer\n");
    scanf("%d",&fini);
}



Graph* choisirGraph() {
    int a;
    Graph* ecosysteme = NULL;
    do {
        a = 0;
        printf("Choisissez votre graphe :\n");
        printf("1. Cours d'eau\n");
        printf("2. Foret Europeenne\n");
        printf("3. Savane\n");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ecosysteme = lireGraphFichier("../CoursDeau.txt");
                break;
            case 2:
                ecosysteme = lireGraphFichier("../ForetEuropeenne.txt");
                break;
            case 3:
                ecosysteme = lireGraphFichier("../Savane.txt");
                break;
        }
    } while (a);
    return ecosysteme;
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
Graph* modifierGraph(Graph* graph){
    int a;
    do{
        a = 0;
        printf("Que voulez vous modifier:\n");
        printf("1. Espece\n");
        printf("2. Coefficient d'influence\n");
        printf("3. Preset fonctionnel\n");
        printf("4. Supprimer une espece\n");
        printf("5. Quitter\n");
        scanf("%d",&a);
        switch(a){
            case 1:
                graph = modifierEspece(graph);
                break;
            case 2:
                graph = modifierCoeff(graph);
                break;
            case 3:
                graph = preset(graph);
                break;
            case 4:
                graph = supprEspece(graph);
                break;
            case 5:
                return graph;
            default:
                printf("Option non valide\n\n");
                break;
        }
    }while(1);
}
Graph* modifierEspece(Graph* graph){
    int a;
    int fini;
    int choix;
    char nom[longueur_Max];

    do {
        choix = 0;
        a = 0;
        fini = 0;
        printf("Quel espece voulez vous modifier:\n");
        scanf("%s", nom);
        for (int i = 1; i <= graph->nbEspeces; i++){
            if (strcmp(nom, graph->especes[i].nom) == 0){
                a = i;
            }
        }
        if (a == 0) {
            printf("L'espece n'existe pas\n");
        }else{
            printf("Que voulez vous modifier ?\n");
            printf("1. Population\n");
            printf("2. Taux d'accroissement\n");
            scanf("%d",&choix);
            switch(choix){
                case 1:
                    printf("Quel nouvelle valeur pour la population de %s (%0.f):\n",graph->especes[a].nom,graph->especes[a].population);
                    scanf("%f",&graph->especes[a].population);
                    break;
                case 2:
                    printf("Quel nouvelle valeur pour le taux d'accroissement de %s (%2.f):\n", graph->especes[a].nom, graph->especes[a].taux_accroissement);
                    scanf("%f",&graph->especes[a].taux_accroissement);
                    break;
                default:
                    printf("Choix non valide\n");
                    break;
            }
            printf("Voulez vous continuer ? (OUI/1) (NON/2):\n");
            scanf("%d", &fini);

            if(fini == 2){
                return graph;
            }
        }
    }while(1);
}



Graph* modifierCoeff(Graph* graph){
    int n,p;
    int fini;
    double coeff;
    do{
        fini = 1;
        printf("Quel arc voulez vous modifier :\n");
        scanf("%d%d",&n,&p);
        if(graph->especes[p].arc == NULL){
            printf("L'arc n'existe pas\n");
            return graph;
        }
        Arc* ark = graph->especes[p].arc;
        while(ark->IDs != n && ark->arcsuivant != NULL){
            ark = ark->arcsuivant;
        }
        if(ark->IDs != n){
            printf("L'arc n'existe pas\n");
            return graph;
        }
        printf("Quelle coefficient voulez vous mettre pour l'arc %d %d :\n",n,p);
        scanf("%lf", &ark->infl);

        printf("Voulez vous continuer ? (OUI/1) (NON/2):\n");
        scanf("%d", &fini);
        if(fini == 2){
            return graph;
        }
    }while(1);
}
Graph* supprEspece(Graph* graph){
    int a = 0;
    int f;
    int cur;
    char nom[longueur_Max];
    printf("Quel espece voulez vous modifier:\n");
    scanf("%s", nom);
    for (int i = 1; i <= graph->nbEspeces; i++){
        if (strcmp(nom, graph->especes[i].nom) == 0){
            a = i;
        }
    }
    if (a == 0) {
        printf("L'espece n'existe pas\n");
        return graph;
    }
    if(graph->especes[a].pred[0] != -1){
        Arc* neuil = NULL;
        Arc* ark = NULL;
        for (int i = 0; graph->especes[a].pred[i] != -1; i++){
            cur = graph->especes[a].pred[i];
            if(graph->especes[cur].arc->IDs == a){
                ark = graph->especes[cur].arc->arcsuivant;
                free(graph->especes[cur].arc);
                graph->especes[cur].arc = ark;

                graph->especes[cur].suc[0] = -1;
            }else{
                f = 0;
                neuil = graph->especes[cur].arc;
                while(neuil->IDs != a && neuil->arcsuivant !=NULL){
                    ++f;
                    ark = neuil;
                    neuil = neuil->arcsuivant;
                }
                while(graph->especes[cur].suc[f + 1] != -1){
                    graph->especes[cur].suc[f] = graph->especes[cur].suc[f + 1];
                    ++f;
                }
                graph->especes[cur].suc[f + 1] = -1;
                ark->arcsuivant = neuil->arcsuivant;
                free(neuil);
            }
        }
    }
    if(graph->especes[a].suc[0] != -1){
        for(int i = 0; graph->especes[a].suc[i] != -1; i++){
            cur = graph->especes[a].suc[i];
            f = 0;
            while(graph->especes[cur].pred[f] != a){
                ++f;
            }
            while(graph->especes[cur].pred[f + 1] != -1){
                graph->especes[cur].pred[f] = graph->especes[cur].pred[f + 1];
                ++f;
            }
            graph->especes[cur].pred[f + 1] = -1;
        }
    }
    while(graph->especes[a].arc != NULL){
        Arc* ark = graph->especes[a].arc;
        graph->especes[a].arc = ark->arcsuivant;
        free(ark);
    }
    graph->especes[a].supp = true;
    strncpy(graph->especes[a].nom, "Supprime", longueur_Max - 1);
    graph->especes[a].nom[longueur_Max - 1] = '\0';
    return graph;
}

Graph* preset(Graph* graph){

    switch(graph->nbEspeces){

        case 9:  // Cours d'eau

            // Producteurs primaires - populations plus élevées car base de la chaîne
            graph->especes[1].population = 1000;     // Débris végétaux
            graph->especes[1].taux_accroissement = 1.2f;

            graph->especes[2].population = 800;      // Microalgue
            graph->especes[2].taux_accroissement = 1.3f;

            graph->especes[3].population = 200;      // Cadavre
            graph->especes[3].taux_accroissement = 0.8f;

            // Décomposeur
            graph->especes[4].population = 400;      // Ver de vase
            graph->especes[4].taux_accroissement = 1.1f;

            // Producteurs secondaires
            graph->especes[5].population = 300;      // Micro-organisme
            graph->especes[5].taux_accroissement = 1.2f;

            graph->especes[6].population = 250;      // Limnée
            graph->especes[6].taux_accroissement = 1.1f;

            graph->especes[7].population = 200;      // Gammare
            graph->especes[7].taux_accroissement = 1.1f;

            graph->especes[8].population = 100;      // Carpe
            graph->especes[8].taux_accroissement = 1.0f;

            // Prédateur au sommet
            graph->especes[9].population = 50;       // Brochet
            graph->especes[9].taux_accroissement = 0.9f;

            printf("\nLe preset Cours d'eau a ete ajoute\n");
            break;

        case 13:  // Savane

            // Producteurs primaires
            graph->especes[1].population = 1000;     // Herbe à éléphant
            graph->especes[1].taux_accroissement = 1.3f;

            graph->especes[2].population = 800;      // Feuille
            graph->especes[2].taux_accroissement = 1.2f;

            graph->especes[3].population = 200;      // Carcasse
            graph->especes[3].taux_accroissement = 0.8f;

            // Décomposeur
            graph->especes[4].population = 500;      // Vers/Insecte
            graph->especes[4].taux_accroissement = 1.2f;

            // Producteurs secondaires
            graph->especes[5].population = 200;      // Marabout
            graph->especes[5].taux_accroissement = 1.0f;

            graph->especes[6].population = 300;      // Musaraigne
            graph->especes[6].taux_accroissement = 1.1f;

            graph->especes[7].population = 400;      // Antilope
            graph->especes[7].taux_accroissement = 1.0f;

            graph->especes[8].population = 300;      // Buffle
            graph->especes[8].taux_accroissement = 0.9f;

            graph->especes[9].population = 250;      // Girafe
            graph->especes[9].taux_accroissement = 0.9f;

            // Prédateurs
            graph->especes[10].population = 100;     // Hyène
            graph->especes[10].taux_accroissement = 0.9f;

            graph->especes[11].population = 80;      // Guépard
            graph->especes[11].taux_accroissement = 0.8f;

            graph->especes[12].population = 60;      // Lion
            graph->especes[12].taux_accroissement = 0.8f;

            graph->especes[13].population = 150;     // Vautour
            graph->especes[13].taux_accroissement = 0.9f;

            printf("\nLe preset Savane a ete ajoute\n");
            break;

        case 14:  // Forêt européenne

            // Producteurs primaires
            graph->especes[1].population = 1000;     // Graines/Glands
            graph->especes[1].taux_accroissement = 1.3f;

            graph->especes[2].population = 1200;     // Herbes/Feuilles
            graph->especes[2].taux_accroissement = 1.4f;

            graph->especes[3].population = 800;      // Sève
            graph->especes[3].taux_accroissement = 1.2f;

            graph->especes[4].population = 200;      // Charogne
            graph->especes[4].taux_accroissement = 0.8f;

            // Décomposeur
            graph->especes[5].population = 600;      // Larves/Insectes
            graph->especes[5].taux_accroissement = 1.2f;

            // Producteurs secondaires
            graph->especes[6].population = 400;      // Lapin
            graph->especes[6].taux_accroissement = 1.2f;

            graph->especes[7].population = 300;      // Ecureuil
            graph->especes[7].taux_accroissement = 1.1f;

            graph->especes[8].population = 350;      // Mulot
            graph->especes[8].taux_accroissement = 1.2f;

            graph->especes[9].population = 200;      // Pic-vert
            graph->especes[9].taux_accroissement = 1.0f;

            graph->especes[10].population = 150;     // Cerf
            graph->especes[10].taux_accroissement = 0.9f;

            graph->especes[11].population = 180;     // Sanglier
            graph->especes[11].taux_accroissement = 0.9f;

            // Prédateurs
            graph->especes[12].population = 80;      // Chouette
            graph->especes[12].taux_accroissement = 0.9f;

            graph->especes[13].population = 60;      // Buse
            graph->especes[13].taux_accroissement = 0.8f;

            graph->especes[14].population = 40;      // Loup
            graph->especes[14].taux_accroissement = 0.8f;

            printf("\nLe preset Foret a ete ajoute\n");
            break;

        default:
            printf("\nLe preset n'a pas marche\n");
            break;
    }
    return graph;
}

// Fonction principale pour l'algorithme A*
void A_star(Graph* graph) {
    if (!graph || !graph->especes) {
        printf("Erreur : Graphe invalide ou non initialise.\n");
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
        printf("Erreur : numero source invalide (%d).\n", sourceId);
        return;
    }

    printf("Entrez le numero de l'espece destination : ");
    scanf("%d", &destId);

    if (destId < 1 || destId > graph->nbEspeces) {
        printf("Erreur : numero destination invalide (%d).\n", destId);
        return;
    }

    // Initialisation des structures de données pour A*
    double* dist = malloc((graph->nbEspeces + 1) * sizeof(double)); // Distance minimale
    int* prev = malloc((graph->nbEspeces + 1) * sizeof(int)); // Prédécesseur
    bool* visited = malloc((graph->nbEspeces + 1) * sizeof(bool)); // Visité ou non

    if (!dist || !prev || !visited) {
        printf("Erreur : Allocation de memoire echouee.\n");
        free(dist);
        free(prev);
        free(visited);
        return;
    }

    for (int i = 1; i <= graph->nbEspeces; i++) {
        dist[i] = (i == sourceId) ? 0 : INT_MAX;
        prev[i] = -1;
        visited[i] = false;
    }

    // A* algorithm
    for (int i = 1; i <= graph->nbEspeces; i++) {
        int u = -1;
        double minDist = INT_MAX;

        // Trouver le sommet avec la distance minimale
        for (int j = 1; j <= graph->nbEspeces; j++) {
            if (!visited[j] && dist[j] < minDist) {
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
            printf("%s%s", graph->especes[path[i]].nom, (i > 0) ? " -> " : "\n");
        }
    }

    // Libération des ressources
    free(dist);
    free(prev);
    free(visited);
}



// Fonction principale pour calculer la connexité, k-arête-connexité et k-sommet-connexité
void k_connexite(Graph* graph) {
    if (!graph) {
        printf("Erreur : Graphe invalide.\n");
        return;
    }

    int nombreAnimaux = graph->nbEspeces;

    // Représentation du graphe sous forme de matrice d'adjacence
    int graphe[MAX_ANIMAUX][MAX_ANIMAUX] = {0};
    int kConnexite = 0;
    int connexiteTotale = 0;

    // Construire la matrice d'adjacence à partir des arcs du graphe
    for (int i = 1; i <= nombreAnimaux; i++) {
        Arc* arc_courant = graph->especes[i].arc;
        while (arc_courant) {
            graphe[arc_courant->IDb - 1][arc_courant->IDs - 1] = 1;
            graphe[arc_courant->IDs - 1][arc_courant->IDb - 1] = 1; // Graphe non orienté

            // Calculer la connexité de l'animal choisi
            if (arc_courant->IDb == i || arc_courant->IDs == i) {
                kConnexite++;
            }

            // Connexité totale du graphe
            connexiteTotale++;
            arc_courant = arc_courant->arcsuivant;
        }
    }

    // Demander à l'utilisateur de choisir un animal
    printf("\nListe des animaux :\n");
    for (int i = 1; i <= nombreAnimaux; i++) {
        printf("%d - %s\n", i, graph->especes[i].nom);
    }

    printf("\nEntrez le numero de l'animal que vous choisissez : ");
    scanf("%d", &choix);

    if (choix < 1 || choix > nombreAnimaux) {
        printf("Erreur : choix invalide.\n");
        return;
    }

    // Calcul de la k-arête-connexité et k-sommet-connexité
    int kAreteConnexite = 0;
    int kSommetConnexite = 0;

    for (int i = 0; i < nombreAnimaux; i++) {
        int visite[MAX_ANIMAUX] = {0};
        int pile[MAX_ANIMAUX];
        int top = -1;

        // Débuter à partir du premier sommet non exclu
        for (int j = 0; j < nombreAnimaux; j++) {
            if (j != i) {
                pile[++top] = j;
                visite[j] = 1;
                break;
            }
        }

        // DFS pour marquer les sommets atteignables
        while (top >= 0) {
            int v = pile[top--];
            for (int j = 0; j < nombreAnimaux; j++) {
                if (!visite[j] && graphe[v][j] == 1 && j != i) {
                    visite[j] = 1;
                    pile[++top] = j;
                }
            }
        }

        // Vérifier si tous les sommets non exclus sont visités
        int connexe = 1;
        for (int j = 0; j < nombreAnimaux; j++) {
            if (j != i && !visite[j]) {
                connexe = 0;
                break;
            }
        }

        if (connexe) {
            kAreteConnexite++;
            kSommetConnexite++;
        }
    }

    // Affichage des résultats
    printf("\nLa connexite de l'animal '%s' est : %d\n", graph->especes[choix].nom, kConnexite);
    printf("La connexite totale du graphe est : %d\n", connexiteTotale);
    printf("La k-arete-connexite du graphe est : %d\n", kAreteConnexite);
    printf("La k-sommet-connexite du graphe est : %d\n", kSommetConnexite);
}



void afficherChaine(Graph* graph, int* chaine, int taille) {
    // Affiche une chaîne alimentaire
    for(int i = 0; i < taille; i++) {
        printf("%s", graph->especes[chaine[i]].nom);
        if(i < taille - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

void trouverChaines(Graph* graph, int espece, int* chaine, int taille, bool* visite) {
    // Ajoute l'espèce courante à la chaîne
    chaine[taille] = espece;
    taille++;

    // Marque l'espèce comme visitée
    visite[espece] = true;

    // Si l'espèce n'a pas de prédécesseurs, affiche la chaîne
    if(graph->especes[espece].pred[0] == -1) {
        afficherChaine(graph, chaine, taille);
    } else {
        // Pour chaque prédécesseur
        for(int i = 0; graph->especes[espece].pred[i] != -1 && i < MAX_connexion; i++) {
            int pred = graph->especes[espece].pred[i];
            // Si le prédécesseur n'a pas déjà été visité (évite les cycles)
            if(!visite[pred]) {
                trouverChaines(graph, pred, chaine, taille, visite);
            }
        }
    }
    visite[espece] = false;
}

void chainesEspece(Graph* graph) {
    char nom[longueur_Max];
    int espece_id = -1;

    // Demande le nom de l'espèce
    printf("Entrez le nom de l'espece : ");
    scanf("%s", nom);

    // Trouve l'ID de l'espèce
    for(int i = 1; i <= graph->nbEspeces; i++) {
        if(strcmp(graph->especes[i].nom, nom) == 0) {
            espece_id = i;
            break;
        }
    }

    if(espece_id == -1) {
        printf("Espece non trouvee.\n");
        return;
    }

    // Initialise les structures pour la recherche
    int* chaine = (int*)malloc(graph->nbEspeces * sizeof(int));
    bool* visite = (bool*)malloc((graph->nbEspeces + 1) * sizeof(bool));

    if(chaine == NULL || visite == NULL) {
        printf("Erreur d'allocation memoire.\n");
        free(chaine);
        free(visite);
        return;
    }

    // Initialise le tableau des visites
    for(int i = 0; i <= graph->nbEspeces; i++) {
        visite[i] = false;
    }

    printf("Chaines alimentaires menant a %s :\n", nom);
    trouverChaines(graph, espece_id, chaine, 0, visite);

    // Libère la mémoire
    free(chaine);
    free(visite);
}
