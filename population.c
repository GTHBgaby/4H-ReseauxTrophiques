#include "population.h"
#include <stdio.h>


Ecosystem initialize_ecosystem(double prey, double predator, double prey_growth_rate,
                               double predation_rate, double predator_death_rate,
                               double reproduction_rate) {
    Ecosystem eco;
    eco.prey = prey;
    eco.predator = predator;
    eco.prey_growth_rate = prey_growth_rate;
    eco.predation_rate = predation_rate;
    eco.predator_death_rate = predator_death_rate;
    eco.reproduction_rate = reproduction_rate;
    return eco;
}


void simulate_predator_prey(Ecosystem* ecosystem, int steps) {
    printf("\nSimulation Prédateur-Proie\n");
    printf("Étape\tProies\tPrédateurs\n");
    for (int t = 0; t <= steps; t++) {
        printf("%d\t%.2f\t%.2f\n", t, ecosystem->prey, ecosystem->predator);

        double new_prey = ecosystem->prey +
                          ecosystem->prey_growth_rate * ecosystem->prey -
                          ecosystem->predation_rate * ecosystem->prey * ecosystem->predator;

        double new_predator = ecosystem->predator +
                              ecosystem->reproduction_rate * ecosystem->prey * ecosystem->predator -
                              ecosystem->predator_death_rate * ecosystem->predator;

        ecosystem->prey = (new_prey > 0) ? new_prey : 0;
        ecosystem->predator = (new_predator > 0) ? new_predator : 0;
    }
}


void simulate_logistic_growth(double r, double K, double initial_population, int steps) {
    double population = initial_population;
    printf("\nSimulation Croissance Logistique\n");
    printf("Étape\tPopulation\n");

    for (int t = 0; t <= steps; t++) {
        printf("%d\t%.2f\n", t, population);
        population = population + r * population * (1 - population / K);
        if (population < 0) population = 0;
    }
}


void generate_dot_file(const char* filename, Ecosystem* ecosystem, int steps) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur : Impossible de créer le fichier DOT.\n");
        return;
    }

    fprintf(file, "digraph Ecosystem {\n");
    fprintf(file, "    node [shape=circle];\n");

    for (int t = 0; t <= steps; t++) {
        double new_prey = ecosystem->prey +
                          ecosystem->prey_growth_rate * ecosystem->prey -
                          ecosystem->predation_rate * ecosystem->prey * ecosystem->predator;

        double new_predator = ecosystem->predator +
                              ecosystem->reproduction_rate * ecosystem->prey * ecosystem->predator -
                              ecosystem->predator_death_rate * ecosystem->predator;

        fprintf(file, "    \"%d : Prey (%.2f)\" -> \"%d : Predator (%.2f)\";\n", t, ecosystem->prey, t + 1, ecosystem->predator);

        ecosystem->prey = (new_prey > 0) ? new_prey : 0;
        ecosystem->predator = (new_predator > 0) ? new_predator : 0;
    }

    fprintf(file, "}\n");
    fclose(file);

    printf("Fichier DOT généré : %s\n", filename);
}
