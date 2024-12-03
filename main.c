#include "Graph.h"
#include "generationDOT.h"
#include<stdlib.h>
#include<stdio.h>
void Menu(){
    int choix = 0;
    char input;
    Graph* ecosysteme = NULL;

    ecosysteme = choisirGraph();

    do {
        system("cls");
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Changer de graphe\n");
        printf("2. Afficher les especes \n");
        printf("3. Question specifique\n");
        printf("4. Simulation \n");
        printf("5. Modifier les valeurs\n");
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
                printf("Vous avez choisi l'option 4\n");
                break;
            case '5':
                ecosysteme = modifierGraph(ecosysteme);
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
