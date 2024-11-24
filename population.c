#include "population.h"
#include <stdio.h>

// Initialiser un écosystème avec des paramètres donnés
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

// Simulation du modèle prédateur-proie
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

        ecosystem->prey = (new_prey > 0) ? new_prey : 0;           // Éviter des populations négatives
        ecosystem->predator = (new_predator > 0) ? new_predator : 0;
    }
}

// Simulation de la croissance logistique
void simulate_logistic_growth(double r, double K, double initial_population, int steps) {
    double population = initial_population;
    printf("\nSimulation Croissance Logistique\n");
    printf("Étape\tPopulation\n");

    for (int t = 0; t <= steps; t++) {
        printf("%d\t%.2f\n", t, population);
        population = population + r * population * (1 - population / K);
        if (population < 0) population = 0;  // Éviter des populations négatives
    }
}
