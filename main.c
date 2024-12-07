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

    // Initialisation de l'écosystème
    ecosysteme = choisirGraph();
    if (!ecosysteme) return;

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Changer de graphe\n");
        printf("2. Afficher les especes \n");
        printf("3. Niveau Trophique\n");
        printf("4. Simulation \n");
        printf("5. Modifier les valeurs\n");
        printf("6. Affichage des schemas (en .jpg)\n");
        printf("7. Chemin le plus rapide\n");
        printf("8. Connexite\n");
        printf("8. Quitter\n");
        printf("\nVotre choix (1-9): ");

        // Lecture du choix avec un espace avant %c pour ignorer les caractères speciaux
        scanf(" %c", &input);
        getchar();

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

            case '3': {
                printf("\nCalcul des niveaux trophiques :\n\n");
                calculNiveauTrophique(ecosysteme);

                for (int i = 1; i <= ecosysteme->nbEspeces; i++) {
                    if (ecosysteme->especes[i].pred[0] == -1) {
                        printf("%-20s --> producteur primaire (niveau 1)\n", ecosysteme->especes[i].nom);
                    } else {
                        printf("%-20s --> niveau trophique %d\n",
                               ecosysteme->especes[i].nom,
                               (int) ecosysteme->especes[i].niveauTrophique);
                    }
                }
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            }

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
                        printf("%-20s | %11.2f\n",
                            ecosysteme->especes[i].nom,
                            ecosysteme->especes[i].population);
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

                // Ajoutez les autres cas ici
            case '5':
                ecosysteme = modifierGraph(ecosysteme);
                break;

            case '6':
                break;
            case '7':
                A_star();
                break;

            case '8':
                k_connexite();
                break;

        }
    } while(input != '9');

    // Libération de la mémoire avant de quitter
    if(ecosysteme) {
        libererGraph(ecosysteme);
    }
}

int main() {
    Menu();
    return 0;
}


/*int main(){
    Arc* man = malloc(sizeof (Arc));
    char nomFichier[50];

    char nom_fichierDOT[100];
    char nom_espece[100];
    Especes especes[MAX_ESPECES];
    int nb_especes;
    char choix;
    int fichier_valide;

    /*Graph* graph = NULL;
    //printf("Donnez le nom du fichier a analyser :");
    //scanf("%s", nomFichier);
    graph = lireGraphFichier(nomFichier,man);
    printEcosysteme(graph);*/

    /*do {
        do {
            nb_especes = 0;
            printf("Entrez le nom du fichier a modifier (sans extension):\n");
            scanf("%s", nom_fichierDOT);

            lire_fichier_dot(nom_fichierDOT, especes, &nb_especes);

            if (nb_especes == 0) {
                printf("Fichier invalide ou vide. Veuillez reessayer.\n\n");
                fichier_valide = 0;
            } else {
                fichier_valide = 1;
            }
        } while (!fichier_valide);

        printf("Entrez le nom de l'espece a modifier :\n");
        scanf("%s", nom_espece);

        if (modifier_espece(especes, nb_especes, nom_espece)) {
            mettre_a_jour_fichier_dot(nom_fichierDOT, especes, nb_especes);
            printf("Fichier mis a jour avec succes.\n");
        }

        printf("Voulez-vous modifier une autre espece ? (y/n):\n");
        scanf(" %c", &choix);
    } while (choix == 'y' || choix == 'Y');
    return 0;
}*/