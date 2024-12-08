#include "Graph.h"
#include "evolution.h"
#include "generationDOT.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "Trophiques.h"

void Menu() {

    char input;
    Graph* ecosysteme = NULL;
    const char* current_graph = "ecosysteme";

    // Initialisation de l'écosystème
    ecosysteme = choisirGraph();
    if (!ecosysteme) return;

    do {
        system("cls");
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Changer de graphe\n");
        printf("2. Afficher les especes \n");
        printf("3. Questions\n");
        printf("4. Simulation \n");
        printf("5. Modifier les valeurs\n");
        printf("6. Affichage des schemas (en .jpg)\n");
        printf("7. Quitter\n");
        printf("\nVotre choix (1-7): ");

        // Lecture du choix avec un espace avant %c pour ignorer les caractères speciaux
        scanf(" %c", &input);
        getchar();
        system("cls");

        switch(input) {
            case '1': {
                if (ecosysteme != NULL) {
                    libererGraph(ecosysteme);
                }
                ecosysteme = choisirGraph();
                printf("Appuyez sur Entree pour continuer...");
                getchar();  // Attente de l'appui sur Entrée
                break;
            }
            case '2':
                printEcosysteme(ecosysteme);
                break;

            case '3':
                menuQuestion(ecosysteme);
                break;

            case '4': {
                int tempsSimulation;
                printf("\nEntrez le temps de simulation (en mois): ");
                if (scanf("%d", &tempsSimulation) != 1) {
                    printf("Erreur: veuillez entrer un nombre valide\n");
                    while (getchar() != '\n') {};
                    break;
                }
                while (getchar() != '\n') {};

                int t = 1;
                char simInput;

                do {
                    printf("\n=== Mois %d/%d ===\n\n", t, tempsSimulation);
                    evoluerPopulations(ecosysteme);

                    printf("%-20s | %-15s\n", "Espece", "Population");
                    printf("----------------------------------------\n");

                        for(int i = 1; i <= ecosysteme->nbEspeces; i++) {
                            if (ecosysteme->especes[i].supp==false) {
                            printf("%-20s | %11.2f\n",
                                ecosysteme->especes[i].nom,
                                ecosysteme->especes[i].population);
                        }
                    }
                    if (t < tempsSimulation) {
                        printf("\nAppuyez sur Entree pour continuer ou 'q' pour quitter... ");
                        simInput = getchar();
                        if (simInput != '\n') {
                            while (getchar() != '\n');
                        }
                        if (simInput == 'q' || simInput == 'Q') break;
                    }
                    t++;
                } while (t <= tempsSimulation);
                break;
            }

            case '5':
                ecosysteme = modifierGraph(ecosysteme);
                break;

            case '6':
                genererFichierDot(ecosysteme, current_graph);
                printf("Appuyez sur entree");
                getchar();
                break;
        }
    } while(input != '7');

    DelTempFile(current_graph);

    if(ecosysteme) {
        libererGraph(ecosysteme);
    }
}

int main() {
    Menu();
    return 0;
}