#include "Graph.h"
#include "evolution.h"
#include "generationDOT.h"
#include <stdlib.h>
#include "stdio.h"
#include <time.h>

void Menu(){

    int choix = 0;
    char input;
    int a;


    Graph* ecosysteme = NULL;
    int tempsSimulation;

    ecosysteme = choisirGraph();

    do {
        system("cls");
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Changer de graphe\n");
        printf("2. Afficher les especes \n");
        printf("3. Question specifique\n");
        printf("4. Simulation \n");
        printf("5. Jsp\n");
        printf("6. Quitter\n");
        printf("\nVotre choix (1-6): ");

        scanf(" %c", &input);

        switch(input) {
            case '1':
                libererGraph(ecosysteme);
                ecosysteme = choisirGraph();
                break;
            case '2':
                printEcosysteme(ecosysteme);
                break;
            case '3':
                printf("Vous avez choisi l'option 3\n");
                break;

            case '4':

            printf("Entrez le temps de simulation (jours): ");
            scanf("%d", &tempsSimulation);
            getchar(); // Vide le buffer

            for(int t = 1; t <= tempsSimulation;) {
                system("cls");
                printf("=== Jour %d/%d ===\n", t, tempsSimulation);
                printf("Appuyez sur Entree pour avancer, 'q' pour quitter\n\n");

                evoluerPopulations(ecosysteme);
                for(int i = 1; i <= ecosysteme->nbEspeces; i++) {
                    printf("%s: Population = %.2f\n",ecosysteme->especes[i].nom,ecosysteme->especes[i].population);
                    printf("Capacite = %.2f\n",ecosysteme->especes[i].capacite);
                }

                if(getchar() == 'q') break;
                t++;
            }
            break;

            case '5':
                printf("Vous avez choisi l'option 5\n");
                break;
            case '6':
                libererGraph(ecosysteme);
                return;
            default:
                printf("Choix invalide! Veuillez choisir entre 1 et 6.\n");
        }
    } while(1);
}

int main(){
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