#include "Trophiques.h"


void calculNiveauTrophique(Graph* graph) {
    int niveauxPossibles[MAX_ESPECES][MAX_niveaux];
    int nbNiveaux[MAX_ESPECES];

    // Initialisation
    for (int i = 1; i <= graph->nbEspeces; i++) {
        nbNiveaux[i] = 0;

        if (graph->especes[i].pred[0] == -1) {
            graph->especes[i].niveauTrophique = 1.0;
            niveauxPossibles[i][0] = 1;
            nbNiveaux[i] = 1;
        }
    }

    // Calcul des niveaux
    int modifie;
    do {
        modifie = 0;
        for (int i = 1; i <= graph->nbEspeces; i++) {
            if (graph->especes[i].pred[0] != -1) {
                for (int j = 0; graph->especes[i].pred[j] != -1 && j < MAX_connexion; j++) {
                    int predId = graph->especes[i].pred[j];
                    int nouveauNiveau = 1 + graph->especes[predId].niveauTrophique;

                    int niveauExiste = 0;
                    for (int k = 0; k < nbNiveaux[i]; k++) {
                        if (niveauxPossibles[i][k] == nouveauNiveau) {
                            niveauExiste = 1;
                            break;
                        }
                    }

                    if (!niveauExiste && nbNiveaux[i] < MAX_niveaux) {
                        niveauxPossibles[i][nbNiveaux[i]] = nouveauNiveau;
                        nbNiveaux[i]++;
                        modifie = 1;
                    }
                }

                if (nbNiveaux[i] > 0) {
                    int niveauMax = 0;
                    for (int k = 0; k < nbNiveaux[i]; k++) {
                        if (niveauxPossibles[i][k] > niveauMax) {
                            niveauMax = niveauxPossibles[i][k];
                        }
                    }
                    graph->especes[i].niveauTrophique = niveauMax;
                }
            }
        }
    } while (modifie);
}