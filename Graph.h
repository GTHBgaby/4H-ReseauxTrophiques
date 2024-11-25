#ifndef INC_4E_RESEAUXTROPHIQUES_GRAPH_H
#define INC_4E_RESEAUXTROPHIQUES_GRAPH_H


#define MAX_especes 100
#define longueur_Max 50
#define MAX_connexion 10

typedef struct a{
    int IDb;             // ID de l'espèce source
    int IDs;             // ID de l'espèce destination
    a* arcsuivant;    // permet de passer à l'arc suivant
    double infl;       // Coefficient d'influence
}Arc;

typedef struct {
    int id;
    int pred[MAX_connexion];
    int suc[MAX_connexion];
    char nom[longueur_Max];
    Arc* arc;
    double population;     // Taille de la population
    double tauxDeCroissance;    // Taux de croissance
    double capacite;       // Capacité de charge
    int niveauTrophique;     // Niveau trophique
} Especes;


typedef struct {
    Especes* especes;     // Tableau des espèces
    int** adjacence;      // Matrice d'adjacence
    double** poids;     // Matrice des poids
    int nbEspeces;       // Nombre d'espèces
    char nom[longueur_Max]; // Nom de l'écosystème
} Graph;



Graph* create_graph(int nbEspeces);
void destroy_graph(Graph* g);




#endif //INC_4E_RESEAUXTROPHIQUES_GRAPH_H
