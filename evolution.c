#include "evolution.h"
#include <math.h>
#include <stdlib.h>

#define POP_BASE 100.0
#define DELTA_T 0.1

void modifierPopulation(Graph* graph, int id, double nouvelle_population) {
    if (!graph || id < 1 || id > graph->nbEspeces) return;
    graph->especes[id].population = nouvelle_population;
}

void modifierInfluence(Graph* graph, int idEspece, double nouvelleInfluence) {
    if (!graph || idEspece < 1 || idEspece > graph->nbEspeces) return;

    Arc* arc = graph->especes[idEspece].arc;
    while (arc) {
        arc->infl = nouvelleInfluence;
        arc = arc->arcsuivant;
    }
}




void miseAJourPopulations(Graph* graph) {
    if (!graph || !graph->especes) return;


    modifierInfluence(graph, 4, 1.0);

    double* nouvellesPopulations = malloc((graph->nbEspeces + 1) * sizeof(double));
    if (!nouvellesPopulations) return;

    for(int i = 1; i <= graph->nbEspeces; i++) {
        double N = graph->especes[i].population;
        double r = graph->especes[i].taux_accroissement;
        double K = graph->especes[i].capacite;

        // Producteurs primaires (espèces 1, 2 et 3)
        if (i <= 3) {
            // Si population est 0, initialiser avec POP_BASE
            if (N < POP_BASE) N = POP_BASE;
            double croissance = N + DELTA_T * r * N * (1.0 - N/K);
            nouvellesPopulations[i] = fmax(POP_BASE, croissance);
            continue;
        }

        // Autres espèces
        double croissance = N;
        if (N < POP_BASE) N = POP_BASE;
        double predationTotale = 0.0;
        double ressourcesTotales = 0.0;

        // Calcul ressources
        for (int j = 0; j < MAX_connexion && graph->especes[i].pred[j] != -1; j++) {
            int idProie = graph->especes[i].pred[j];
            if (idProie > 0) {
                Arc* arc = graph->especes[i].arc;
                while (arc) {
                    if (arc->IDs == idProie) {
                        double proiePopulation = fmax(POP_BASE, graph->especes[idProie].population);
                        ressourcesTotales += arc->infl * DELTA_T * proiePopulation;
                        break;
                    }
                    arc = arc->arcsuivant;
                }
            }
        }

        if (ressourcesTotales > 0) {
            croissance += ressourcesTotales;
        }

        // Calcul prédation
        for (int j = 0; j < MAX_connexion && graph->especes[i].suc[j] != -1; j++) {
            int idPredateur = graph->especes[i].suc[j];
            if (idPredateur > 0) {
                Arc* arc = graph->especes[idPredateur].arc;
                while (arc) {
                    if (arc->IDs == i) {
                        double predPopulation = fmax(POP_BASE, graph->especes[idPredateur].population);
                        predationTotale += arc->infl * DELTA_T * predPopulation;
                        break;
                    }
                    arc = arc->arcsuivant;
                }
            }
        }

        nouvellesPopulations[i] = fmax(POP_BASE, croissance - predationTotale);
    }

    // Mise à jour synchronisée
    for(int i = 1; i <= graph->nbEspeces; i++) {
        graph->especes[i].population = nouvellesPopulations[i];
    }
    free(nouvellesPopulations);
}
