#ifndef POPULATION_H
#define POPULATION_H

typedef struct {
    double prey;      // Population des proies
    double predator;  // Population des prédateurs
    double prey_growth_rate;   // Taux de croissance des proies
    double predation_rate;     // Taux de prédation
    double predator_death_rate; // Taux de mortalité des prédateurs
    double reproduction_rate;  // Taux de reproduction des prédateurs
} Ecosystem;

// Initialisation d'un écosystème
Ecosystem initialize_ecosystem(double prey, double predator, double prey_growth_rate,
                               double predation_rate, double predator_death_rate,
                               double reproduction_rate);

// Simulation de la dynamique prédateur-proie
void simulate_predator_prey(Ecosystem* ecosystem, int steps);

// Simulation de la croissance logistique
void simulate_logistic_growth(double r, double K, double initial_population, int steps);

#endif // POPULATION_H
