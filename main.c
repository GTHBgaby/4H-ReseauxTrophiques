#include "graph.h"
#include "lecteurFichiers.h"

int main() {

    // Test de lecture du fichier
    Graph* ecosystem = read_ecosystem_file("../CoursDeau.txt");
    if (ecosystem) {
        print_ecosystem(ecosystem);
        destroy_graph(ecosystem);
    }

    return 0;
}
