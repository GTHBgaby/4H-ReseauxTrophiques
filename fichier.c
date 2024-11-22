//
// Created by louis on 22/11/2024.
//

#include "fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*Graph* load_network(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier %s: %s\n", filename, strerror(errno));
        return NULL;
    }

    char buffer[256];
    Graph* g = NULL;

    // Lecture du nom de l'écosystème
    if (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;  // Supprime le retour à la ligne
    }

    // Lecture du nombre d'espèces
    int nb_species;
    fscanf(file, "%d\n", &nb_species);

    g = create_graph(nb_species);
    if (!g) {
        fclose(file);
        return NULL;
    }

    strncpy(g->name, buffer, MAX_NAME_LENGTH - 1);

    // Lecture des espèces
    for (int i = 0; i < nb_species; i++) {
        Species* sp = &g->species[i];
        if (fgets(buffer, sizeof(buffer), file)) {
            char name[MAX_NAME_LENGTH];
            sscanf(buffer, "%d;%[^;];%lf;%lf;%lf;%d",
                   &sp->id, name, &sp->population,
                   &sp->growth_rate, &sp->capacity,
                   &sp->trophic_level);
            strncpy(sp->name, name, MAX_NAME_LENGTH - 1);
        }
    }

    // Lecture du nombre de relations
    int nb_relations;
    fscanf(file, "%d\n", &nb_relations);

    // Lecture des relations
    for (int i = 0; i < nb_relations; i++) {
        int from, to;
        double weight;
        if (fgets(buffer, sizeof(buffer), file)) {
            sscanf(buffer, "%d;%d;%lf", &from, &to, &weight);
            add_relation(g, from, to, weight);
        }
    }

    fclose(file);
    return g;
}

int save_network(const Graph* g, const char* filename) {
    if (!g) return 0;

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur lors de la création du fichier %s: %s\n", filename, strerror(errno));
        return 0;
    }

    // Écriture du nom de l'écosystème
    fprintf(file, "%s\n", g->name);

    // Écriture du nombre d'espèces
    fprintf(file, "%d\n", g->nb_species);

    // Écriture des espèces
    for (int i = 0; i < g->nb_species; i++) {
        Species sp = g->species[i];
        fprintf(file, "%d;%s;%f;%f;%f;%d\n",
                sp.id, sp.name, sp.population,
                sp.growth_rate, sp.capacity,
                sp.trophic_level);
    }

    // Compte et écriture du nombre de relations
    int nb_relations = 0;
    for (int i = 0; i < g->nb_species; i++) {
        for (int j = 0; j < g->nb_species; j++) {
            if (g->adjacency[i][j]) nb_relations++;
        }
    }
    fprintf(file, "%d\n", nb_relations);

    // Écriture des relations
    for (int i = 0; i < g->nb_species; i++) {
        for (int j = 0; j < g->nb_species; j++) {
            if (g->adjacency[i][j]) {
                fprintf(file, "%d;%d;%f\n", i, j, g->weights[i][j]);
            }
        }
    }

    fclose(file);
    return 1;
}

Graph* load_forest_network() {
    // Création d'un réseau forestier simplifié
    Graph* g = create_graph(6);
    if (!g) return NULL;

    strcpy(g->name, "Ecosysteme Forestier");

    // Ajout des espèces
    add_species(g, "Arbres", 100.0, 0.1);           // id 0
    add_species(g, "Herbivores", 50.0, 0.2);        // id 1
    add_species(g, "Petits rongeurs", 80.0, 0.3);   // id 2
    add_species(g, "Oiseaux", 40.0, 0.2);           // id 3
    add_species(g, "Renards", 10.0, 0.1);           // id 4
    add_species(g, "Loups", 5.0, 0.05);             // id 5

    // Ajout des relations trophiques
    add_relation(g, 0, 1, 0.3);  // Arbres -> Herbivores
    add_relation(g, 0, 2, 0.2);  // Arbres -> Rongeurs
    add_relation(g, 2, 3, 0.4);  // Rongeurs -> Oiseaux
    add_relation(g, 2, 4, 0.5);  // Rongeurs -> Renards
    add_relation(g, 1, 5, 0.6);  // Herbivores -> Loups
    add_relation(g, 4, 5, 0.3);  // Renards -> Loups (compétition/prédation)

    return g;
}

Graph* load_marine_network() {
    // Création d'un réseau marin simplifié
    Graph* g = create_graph(5);
    if (!g) return NULL;

    strcpy(g->name, "Ecosysteme Marin");

    // Ajout des espèces
    add_species(g, "Phytoplancton", 1000.0, 0.5);   // id 0
    add_species(g, "Zooplancton", 500.0, 0.3);      // id 1
    add_species(g, "Petits poissons", 200.0, 0.2);  // id 2
    add_species(g, "Thons", 50.0, 0.1);             // id 3
    add_species(g, "Requins", 10.0, 0.05);          // id 4

    // Ajout des relations trophiques
    add_relation(g, 0, 1, 0.4);  // Phytoplancton -> Zooplancton
    add_relation(g, 1, 2, 0.3);  // Zooplancton -> Petits poissons
    add_relation(g, 2, 3, 0.5);  // Petits poissons -> Thons
    add_relation(g, 2, 4, 0.4);  // Petits poissons -> Requins
    add_relation(g, 3, 4, 0.3);  // Thons -> Requins

    return g;
}

Graph* load_prairie_network() {
    // Création d'un réseau de prairie simplifié
    Graph* g = create_graph(7);
    if (!g) return NULL;

    strcpy(g->name, "Ecosysteme Prairie");

    // Ajout des espèces
    add_species(g, "Herbe", 1000.0, 0.6);           // id 0
    add_species(g, "Fleurs", 500.0, 0.4);           // id 1
    add_species(g, "Insectes", 300.0, 0.3);         // id 2
    add_species(g, "Sauterelles", 200.0, 0.25);     // id 3
    add_species(g, "Oiseaux", 50.0, 0.2);           // id 4
    add_species(g, "Serpents", 20.0, 0.1);          // id 5
    add_species(g, "Rapaces", 10.0, 0.05);          // id 6

    // Ajout des relations trophiques
    add_relation(g, 0, 2, 0.3);  // Herbe -> Insectes
    add_relation(g, 0, 3, 0.4);  // Herbe -> Sauterelles
    add_relation(g, 1, 2, 0.5);  // Fleurs -> Insectes
    add_relation(g, 2, 4, 0.4);  // Insectes -> Oiseaux
    add_relation(g, 3, 4, 0.3);  // Sauterelles -> Oiseaux
    add_relation(g, 3, 5, 0.4);  // Sauterelles -> Serpents
    add_relation(g, 4, 6, 0.5);  // Oiseaux -> Rapaces
    add_relation(g, 5, 6, 0.3);  // Serpents -> Rapaces

    return g;
}

int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}*/
