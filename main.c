#include "Graph.h"
#include "generationDOT.h"
#include <stdio.h>
#include <stdlib.h>


/*int main() {

    // Test de lecture du fichier
    Graph* ecosystem = read_ecosystem_file("../CoursDeau.txt");
    if (ecosystem) {
        print_ecosystem(ecosystem);
        destroy_graph(ecosystem);
    }

    return 0;
}*/
/*int main() {
    Espece especes[] = {
            {"phytoplancton", "lightgreen", 1, 100, 50.0},
            {"zooplancton", "lightyellow", 2, 100, 50.0}
    };

    int relations[][2] = {{0, 1}};
    char filename[100];
    int choix;

    printf("Nom du fichier de sortie (sans extension): ");
    scanf("%s", filename);

    do {
        printf("\n1. Modifier phytoplancton\n");
        printf("2. Modifier zooplancton\n");
        printf("3. Générer le graphe\n");
        printf("4. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                modifier_espece(&especes[0]);
                break;
            case 2:
                modifier_espece(&especes[1]);
                break;
            case 3:
                generer_dot(especes, 2, relations, 1, filename);
                printf("Graphe généré dans %s.dot\n", filename);
                break;
        }
    } while (choix != 4);

    return 0;
}*/

int main(){
    Arc* man = malloc(sizeof (Arc));
    char nomFichier[50];

    char nom_fichierDOT[100];
    char nom_espece[100];
    Espece especes[MAX_ESPECES];
    int nb_especes;
    char choix;
    int fichier_valide;

    /*Graph* graph = NULL;
    //printf("Donnez le nom du fichier a analyser :");
    //scanf("%s", nomFichier);
    graph = lireGraphFichier(nomFichier,man);
    printEcosysteme(graph);*/

    do {
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
}
