#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAUX_BASE 30
#define POP_BASE 100.00
#define CAP_BASE 1200.00



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
                printf("Erreur d'allocation mémoire pour l'arc\n");
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
    printf("\nAppuyer sur n'importe quelle touche pour continuer\n");
    scanf("%d",&fini);
}

Graph* choisirGraph() {
    int choix;
    Graph* ecosysteme = NULL;

    do {
        printf("Choisissez votre graphe :\n");
        printf("1. Cours d'eau\n");
        printf("2. Foret Europeenne\n");
        printf("3. Savane\n");

        if (scanf("%d", &choix) != 1 || choix < 1 || choix > 3) {
            printf("Choix invalide\n");
            while(getchar() != '\n'); // Vide le buffer
            continue;
        }

        switch (choix) {
            case 1: return lireGraphFichier("../CoursDeau.txt");
            case 2: return lireGraphFichier("../ForetEuropeenne.txt");
            case 3: return lireGraphFichier("../Savane.txt");
            default: return NULL;
        }
    } while(1);
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
                    printf("Quel nouvelle valeur pour la population de %s (%f):\n",graph->especes[a].nom,graph->especes[a].population);
                    scanf("%f",&graph->especes[a].population);
                    break;
                case 2:
                    printf("Quel nouvelle valeur pour le taux d'accroissement de %s (%f):\n", graph->especes[a].nom, graph->especes[a].taux_accroissement);
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