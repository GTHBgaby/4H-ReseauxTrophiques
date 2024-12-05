#include "Graph.h"
#include "evolution.h"
#include "generationDOT.h"
#include <stdlib.h>
#include "stdio.h"
#include "star.h"
#include "Trophiques.h"

void Menu(){

    int choix = 0;
    char input;
    int a;


    Graph* ecosysteme = NULL;
    int tempsSimulation;

    ecosysteme = choisirGraph();

    do {

        //system("cls"); cette ligne fait bug tous l'affichage supp si elle sert a r
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Changer de graphe\n");
        printf("2. Afficher les especes \n");
        printf("3. Niveau Trophique\n");
        printf("4. Simulation \n");
        printf("5. Modifier les valeurs\n");
        printf("6. Affichage des schemas (en .jpg)\n");
        printf("7. Chemin le plus rapide\n");
        printf("8. Quitter\n");
        printf("\nVotre choix (1-8): ");

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

                printf("Vous avez choisi l'option 3\n\n");
            calculNiveauTrophique(ecosysteme);

            for (int i = 1; i <= ecosysteme->nbEspeces; i++) {
                if (ecosysteme->especes[i].pred[0] == -1) {
                    printf("%s --> producteur primaire (niveau 1)\n", ecosysteme->especes[i].nom);
                }
                else {
                    printf("%s --> niveau trophique %d\n",
                        ecosysteme->especes[i].nom,
                        (int)ecosysteme->especes[i].niveauTrophique);
                }
            }
            printf("\nAppuyez sur Entree pour continuer...\n");
            while(getchar() != '\n');
            getchar();
            break;

            case '4': {
                printf("Entrez le temps de simulation (en mois):\n");
                scanf("%d", &tempsSimulation);
                while(getchar() != '\n'); // Vide le buffer correctement

                char input;
                int t = 1;

                while(t <= tempsSimulation) {
                    system("cls");
                    printf("\n=== Jour %d/%d ===\n\n", t, tempsSimulation);
                    printf("Appuyez sur Entree pour avancer, 'q' pour quitter\n\n");

                    evoluerPopulations(ecosysteme);

                    // Affichage formaté des populations
                    for(int i = 1; i <= ecosysteme->nbEspeces; i++) {
                        printf("%-15s: Population = %6.2f  |  Capacite = %8.2f\n",
                               ecosysteme->especes[i].nom,
                               ecosysteme->especes[i].population,
                               ecosysteme->especes[i].capacite);
                    }

                    printf("\n"); // Ligne vide pour meilleure lisibilité

                    input = getchar();
                    while(getchar() != '\n'); // Vide le buffer après la lecture

                    if(input == 'q') break;
                    t++;
                }
                break;
            }

            case '5':
                ecosysteme = modifierGraph(ecosysteme);
                break;
            case '6':
                //code Gab
                return;
            case '7':
                choix_a_star();  // Appel à la fonction qui gère le choix de l'environnement et lance A_star
                return;
            case '8':
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