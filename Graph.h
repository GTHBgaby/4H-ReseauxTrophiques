#ifndef INC_4E_RESEAUXTROPHIQUES_GRAPH_H
#define INC_4E_RESEAUXTROPHIQUES_GRAPH_H


#define MAX_especes 100
#define longueur_Max 50
#define MAX_connexion 10

typedef struct a{
    int IDb;             // ID de l'espèce source
    int IDs;             // ID de l'espèce destination
    struct a* arcsuivant;    // permet de passer à l'arc suivant
    double infl;       // Coefficient d'influence
}Arc;

typedef struct {
    int id;
    int pred[MAX_connexion];
    int suc[MAX_connexion];
    char nom[longueur_Max];
    Arc* arc;
    int population;     // Taille de la population
    float taux_accroissement;    // Taux de croissance
    double capacite;       // Capacité maximale de la pop
    int niveauTrophique;     // Niveau trophique
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




#endif //INC_4E_RESEAUXTROPHIQUES_GRAPH_H
