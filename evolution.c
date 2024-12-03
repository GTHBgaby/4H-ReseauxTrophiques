#include "evolution.h"
#define POP_BASE 100.0
#define DELTA_T 0.1



void evoluerPopulations(Graph* g) {

    // Tableau temporaire pour stocker les nouvelles populations
    int* nouvelles_populations = malloc((g->nbEspeces + 1) * sizeof(int));

    // Calculer les nouvelles populations pour chaque espèce
    for (int i = 1; i <= g->nbEspeces; i++) {
        double K = g->especes[i].capacite;
        double r = g->especes[i].taux_accroissement / 100.0; // Convertir en décimal
        int N = g->especes[i].population;

        // Calculer la capacité modifiée par les ressources disponibles
        double K_modifie = K;
        for (int j = 0; j < MAX_connexion && g->especes[i].pred[j] != -1; j++) {
            int pred_id = g->especes[i].pred[j];
            Arc* arc_courant = g->especes[pred_id].arc;

            // Trouver l'arc correspondant
            while (arc_courant) {
                if (arc_courant->IDs == i) {
                    K_modifie += arc_courant->infl * g->especes[pred_id].population;
                    break;
                }
                arc_courant = arc_courant->arcsuivant;
            }
        }

        // Modèle logistique de croissance
        double croissance = N + r * N * (1 - N / K_modifie);

        // Soustraire l'effet des prédateurs
        for (int j = 0; j < MAX_connexion && g->especes[i].suc[j] != -1; j++) {
            int suc_id = g->especes[i].suc[j];
            Arc* arc_courant = g->especes[i].arc;

            while (arc_courant) {
                if (arc_courant->IDs == suc_id) {
                    croissance -= arc_courant->infl * g->especes[suc_id].population;
                    break;
                }
                arc_courant = arc_courant->arcsuivant;
            }
        }

        // Assurer que la population ne devient pas négative
        nouvelles_populations[i] = (croissance > 0) ? (int)croissance : 0;
        g->especes[i].capacite = K_modifie ;
    }

    // Mettre à jour les populations
    for (int i = 1; i <= g->nbEspeces; i++) {
        g->especes[i].population = nouvelles_populations[i];
    }

    free(nouvelles_populations);
}