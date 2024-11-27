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
int main() {
    Arc* man = malloc(sizeof (Arc));
    char nomFichier[50];
    Graph* graph = NULL;
    //printf("Donnez le nom du fichier a analyser :");
    //scanf("%s", nomFichier);
    graph = lireGraphFichier(nomFichier,man);
    printEcosysteme(graph);
    return 0;
}

