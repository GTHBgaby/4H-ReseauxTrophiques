#ifndef INC_4E_RESEAUXTROPHIQUES_GRAPH_H
#define INC_4E_RESEAUXTROPHIQUES_GRAPH_H

#include <stdbool.h>


#define MAX_especes 100
#define longueur_Max 50
#define MAX_connexion 10
#define MAX_niveaux 4

typedef struct a{
    int IDb;             // ID de l'espèce source
    int IDs;             // ID de l'espèce destination
    struct a* arcsuivant;    // permet de passer à l'arc suivant
    double infl;       // Coefficient d'influence
}Arc;

typedef struct {

    int id;
    bool supp;
    int pred[MAX_connexion];
    int suc[MAX_connexion];
    char nom[longueur_Max];
    Arc* arc;
    float population;     // Taille de la population
    float taux_accroissement;    // Taux de croissance
    double capacite;       // Capacité maximale de la pop
    double niveauTrophique;     // Niveau trophique
} Especes;


typedef struct {
    Especes* especes;     // Tableau des espèces
    int nbEspeces;       // Nombre d'espèces
    char nom[longueur_Max]; // Nom de l'écosystème
} Graph;

void Menu();
// Fonctions de base pour la manipulation des graphes
Graph* lireGraphFichier(const char* nomFichier);
Graph* choisirGraph();
void libererGraph(Graph* graph);
void printEcosysteme(Graph* g);

//Fonctions de modification
Graph* modifierGraph(Graph* graph);
Graph* modifierEspece(Graph* graph);
Graph* modifierCoeff(Graph* graph);
Graph* preset(Graph* graph);
Graph* supprEspece(Graph* graph);

void A_star();
void k_connexite();

//Fonctions de chaines d'especes
void chainesEspece(Graph* graph);
void trouverChaines(Graph* graph, int espece, int* chaine, int taille, bool* visite);
void afficherChaine(Graph* graph, int* chaine, int taille);



#endif //INC_4E_RESEAUXTROPHIQUES_GRAPH_H