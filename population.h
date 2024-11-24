#ifndef POPULATION_H
#define POPULATION_H

typedef struct {
    double prey;
    double predator;
    double prey_growth_rate;
    double predation_rate;
    double predator_death_rate;
    double reproduction_rate;
} Ecosystem;

Ecosystem initialize_ecosystem(double prey, double predator, double prey_growth_rate,
                               double predation_rate, double predator_death_rate,
                               double reproduction_rate);

void simulate_predator_prey(Ecosystem* ecosystem, int steps);

void simulate_logistic_growth(double r, double K, double initial_population, int steps);

void generate_dot_file(const char* filename, Ecosystem* ecosystem, int steps);

#endif
