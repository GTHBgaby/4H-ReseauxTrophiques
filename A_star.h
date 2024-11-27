#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX 100

struct Arete {
    int destination;
    int poids;
    struct Arete* suivante;
};

struct Graphe {
    int nbSommets;
    struct Arete* listesAdj[MAX];
};

struct Arete* creerArete(int destination, int poids) {
    struct Arete* nouvelleArete = (struct Arete*) malloc(sizeof(struct Arete));
    nouvelleArete->destination = destination;
    nouvelleArete->poids = poids;
    nouvelleArete->suivante = NULL;
    return nouvelleArete;
}

struct Graphe* creerGraphe(int sommets) {
    struct Graphe* graphe = (struct Graphe*) malloc(sizeof(struct Graphe));
    graphe->nbSommets = sommets;

    for (int i = 0; i < sommets; i++) {
        graphe->listesAdj[i] = NULL;
    }

    return graphe;
}

void ajouterArete(struct Graphe* graphe, int source, int destination, int poids) {
    struct Arete* nouvelleArete = creerArete(destination, poids);
    nouvelleArete->suivante = graphe->listesAdj[source];
    graphe->listesAdj[source] = nouvelleArete;

    nouvelleArete = creerArete(source, poids);
    nouvelleArete->suivante = graphe->listesAdj[destination];
    graphe->listesAdj[destination] = nouvelleArete;
}

int heuristique(int source, int cible) {
    return abs(source - cible);
}

void a_star(const char* nomFichier) {
    printf("Début de la fonction A* pour le fichier %s\n", nomFichier);

    FILE* fichier = fopen(nomFichier, "r");
    if (!fichier) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }

    int nbSommets, nbAretes;
    if (fscanf(fichier, "%d %d\n", &nbSommets, &nbAretes) != 2) {
        printf("Erreur : format incorrect dans le fichier %s\n", nomFichier);
        fclose(fichier);
        return;
    }
    printf("Nombre de sommets : %d, Nombre d'arêtes : %d\n", nbSommets, nbAretes);


    char buffer[256];
    for (int i = 0; i < nbSommets; i++) {
        if (fgets(buffer, sizeof(buffer), fichier) == NULL) {
            printf("Erreur : données manquantes dans les noms des sommets.\n");
            fclose(fichier);
            return;
        }
    }

    struct Graphe* graphe = creerGraphe(nbSommets);
    if (!graphe) {
        printf("Erreur : allocation mémoire pour le graphe échouée.\n");
        fclose(fichier);
        return;
    }

    for (int i = 0; i < nbAretes; i++) {
        int source, destination;
        if (fscanf(fichier, "%d %d\n", &source, &destination) != 2) {
            printf("Erreur : données incorrectes ou ligne vide à l'arête %d.\n", i + 1);
            fclose(fichier);
            free(graphe);
            return;
        }
        ajouterArete(graphe, source - 1, destination - 1, 1);
    }

    fclose(fichier);
    printf("Graphe chargé avec succès depuis %s\n", nomFichier);


    int source = 0;
    int cible = nbSommets - 1;
    int g_score[nbSommets];
    int f_score[nbSommets];
    int ouvert[MAX];
    int precedent[MAX];

    for (int i = 0; i < nbSommets; i++) {
        g_score[i] = INT_MAX;
        f_score[i] = INT_MAX;
        ouvert[i] = 1;
        precedent[i] = -1;
    }

    g_score[source] = 0;
    f_score[source] = heuristique(source, cible);

    while (1) {
        int u = -1;
        int minF = INT_MAX;

        for (int i = 0; i < nbSommets; i++) {
            if (ouvert[i] && f_score[i] < minF) {
                minF = f_score[i];
                u = i;
            }
        }

        if (u == -1 || u == cible) break;
        ouvert[u] = 0;

        struct Arete* courant = graphe->listesAdj[u];
        while (courant) {
            int v = courant->destination;

            if (ouvert[v]) {
                int tentative_g_score = g_score[u] + courant->poids;
                if (tentative_g_score < g_score[v]) {
                    precedent[v] = u;
                    g_score[v] = tentative_g_score;
                    f_score[v] = g_score[v] + heuristique(v, cible);
                }
            }
            courant = courant->suivante;
        }
    }

    if (g_score[cible] == INT_MAX) {
        printf("Aucun chemin trouvé de %d à %d dans %s\n", source, cible, nomFichier);
    } else {
        printf("Chemin trouvé de %d à %d avec une distance de %d dans %s\n", source, cible, g_score[cible], nomFichier);
        printf("Chemin : ");

        int chemin[MAX];
        int tailleChemin = 0;
        for (int v = cible; v != -1; v = precedent[v]) {
            chemin[tailleChemin++] = v;
        }

        for (int i = tailleChemin - 1; i >= 0; i--) {
            printf("%d%s", chemin[i], (i > 0) ? " -> " : "\n");
        }
    }

    free(graphe);
}
