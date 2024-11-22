#ifndef INC_4E_RESEAUXTROPHIQUES_GRAPH_H
#define INC_4E_RESEAUXTROPHIQUES_GRAPH_H


#define MAX_especes 100
#define longueur_Max 50

// Structure pour représenter une espèce
typedef struct {
    int id;
    char nom[longueur_Max];
    double population;     // Taille de la population
    double tauxDeCroissance;    // Taux de croissance
    double capacite;       // Capacité de charge
    int niveauTrophique;     // Niveau trophique
} Especes;

// Structure pour représenter un arc (relation entre espèces)
typedef struct {
    int from;             // ID de l'espèce source
    int to;               // ID de l'espèce destination
    double weight;        // Coefficient d'influence
} Arc;

// Structure principale du graphe
typedef struct {
    Especes* especes;     // Tableau des espèces
    int** adjacence;      // Matrice d'adjacence
    double** poids;     // Matrice des poids
    int nb_especes;       // Nombre d'espèces
    char nom[longueur_Max]; // Nom de l'écosystème
} Graph;


#endif //INC_4E_RESEAUXTROPHIQUES_GRAPH_H
