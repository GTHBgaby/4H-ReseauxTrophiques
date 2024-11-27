#ifndef LECTEURFICHIERS_H
#define LECTEURFICHIERS_H
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"



// Format de fichier proposé :
// Première ligne : nom de l'écosystème
// Deuxième ligne : nombre d'espèces
// Ensuite, pour chaque espèce :
// id;nom;population_initiale;taux_croissance;capacite;niveau_trophique
// Après les espèces, nombre de relations
// Puis pour chaque relation :
// espece_source;espece_destination;poids




Graph* read_ecosystem_file(const char* filename);
void print_ecosystem(Graph* g);
void destroy_graph(Graph* g);




#endif //LECTEURFICHIERS_H
