#ifndef INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
#define INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H

#define MAX_ESPECES 100
#define MAX_LIGNE 256

#include "Graph.h"

void lire_fichier_dot(const char* nom_fichier, Especes* especes, int* nb_especes);
int modifier_espece(Especes* especes, int nb_especes, const char* nom_espece);
void mettre_a_jour_fichier_dot(const char* nom_fichier, Especes* especes, int nb_especes);

void generer_dot(Especes* especes, int nb_especes, int (*relations)[2], int nb_relations, const char* filename);

#endif //INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
