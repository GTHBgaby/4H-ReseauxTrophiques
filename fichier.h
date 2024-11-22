//
// Created by louis on 22/11/2024.
//

#ifndef INC_4E_RESEAUXTROPHIQUES_FICHIER_H
#define INC_4E_RESEAUXTROPHIQUES_FICHIER_H
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "graph.h"

// Format de fichier proposé :
// Première ligne : nom de l'écosystème
// Deuxième ligne : nombre d'espèces
// Ensuite, pour chaque espèce :
// id;nom;population_initiale;taux_croissance;capacite;niveau_trophique
// Après les espèces, nombre de relations
// Puis pour chaque relation :
// espece_source;espece_destination;poids

// Fonctions de lecture/écriture
Graph* load_network(const char* filename);
int save_network(const Graph* g, const char* filename);

// Fonctions utilitaires pour charger les réseaux prédéfinis
Graph* load_forest_network();    // Réseau forestier
Graph* load_marine_network();    // Réseau marin
Graph* load_prairie_network();   // Réseau de prairie

// Fonction pour vérifier si un fichier existe
int file_exists(const char* filename);

#endif

#endif //INC_4E_RESEAUXTROPHIQUES_FICHIER_H
