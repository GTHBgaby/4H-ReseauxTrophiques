#include "evolution.h"
#define POP_BASE 100.0
#define DELTA_T 0.1
#define MAX_K_MODIFIER 2.0



Graph * evoluerPopulations(Graph* g) {
    int* nouvelles_populations = malloc((g->nbEspeces + 1) * sizeof(int));

    // Calculer les nouvelles populations pour chaque espèce
    for (int i = 1; i <= g->nbEspeces; i++) {

            double K = g->especes[i].capacite; //capacite max
            double r = g->especes[i].taux_accroissement / 100.0; //conversion decimal
            int N = g->especes[i].population; //pop

            // Calculer la capacité modifiée par les ressources disponibles
            double K_modifie = K;
            double total_modifier = 0.0;

            for (int j = 0; j < MAX_connexion && g->especes[i].pred[j] != -1; j++) {
                int pred_id = g->especes[i].pred[j];
                Arc* arc_courant = g->especes[i].arc;

                while (arc_courant) {
                    if (arc_courant->IDs == i) {
                        double modifier = arc_courant->infl * g->especes[pred_id].population / POP_BASE;
                        total_modifier += modifier;
                        break;
                    }
                    arc_courant = arc_courant->arcsuivant;
                }
            }

            // Limiter l'augmentation de la capacité
            double K_factor = 1.0 + (total_modifier > MAX_K_MODIFIER ? MAX_K_MODIFIER : total_modifier);
            K_modifie *= K_factor;

            // Modèle logistique avec pas de temps
            double croissance = N + DELTA_T * r * N * (1.0 - N / K_modifie);

            // Impact des prédateurs avec pas de temps
            for (int j = 0; j < MAX_connexion && g->especes[i].suc[j] != -1; j++) {
                int suc_id = g->especes[i].suc[j];
                Arc* arc_courant = g->especes[i].arc;

                while (arc_courant) {
                    if (arc_courant->IDs == suc_id) {
                        croissance -= DELTA_T * arc_courant->infl *
                                     (g->especes[suc_id].population / POP_BASE) * N;
                        break;
                    }
                    arc_courant = arc_courant->arcsuivant;
                }
            }



            // Assurer que la population reste dans des limites raisonnables
            if (croissance < 0) {
                nouvelles_populations[i] = 0;
            } else if (croissance > 2 * K_modifie) {
                nouvelles_populations[i] = (int)(2 * K_modifie);
            } else {
                nouvelles_populations[i] = (int)croissance;
            }

            g->especes[i].capacite = K_modifie;
        }

        // Mettre à jour les populations
        for (int i = 1; i <= g->nbEspeces; i++) {
            g->especes[i].population = nouvelles_populations[i];
        }

        free(nouvelles_populations);
        return g;

}