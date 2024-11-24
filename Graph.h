#ifndef INC_4E_RESEAUXTROPHIQUES_GRAPH_H
#define INC_4E_RESEAUXTROPHIQUES_GRAPH_H


#define MAX_especes 100
#define longueur_Max 50


typedef struct {
    int id;
    char nom[longueur_Max];
    double population;     // Taille de la population
    double tauxDeCroissance;    // Taux de croissance
    double capacite;       // Capacité de charge
    int niveauTrophique;     // Niveau trophique
} Especes;


typedef struct {
    int from;             // ID de l'espèce source
    int to;               // ID de l'espèce destination
    double weight;        // Coefficient d'influence
} Arc;


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
