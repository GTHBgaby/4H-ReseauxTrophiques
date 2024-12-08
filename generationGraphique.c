#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Graph.h"
#include "evolution.h"
#include "generationDOT.h"
#include "generationGraphique.h"
#include "direct.h"

void genererGraphiqueEvolution(const char* nom_espece, float* historique, int nb_mois) {
    MKDIR("temp");

    float min_pop = historique[0];
    float max_pop = historique[0];

    // Trouver min et max
    for(int i = 1; i < nb_mois; i++) {
        if(historique[i] < min_pop) min_pop = historique[i];
        if(historique[i] > max_pop) max_pop = historique[i];
    }

    // Ajouter une marge de 10%
    float marge = (max_pop - min_pop) * 0.1;
    min_pop = (min_pop > marge) ? min_pop - marge : 0;
    max_pop += marge;

    char filename[256];
    snprintf(filename, sizeof(filename), "temp/evolution_%s.dot", nom_espece);
    FILE* f = fopen(filename, "w");
    if(!f) {
        printf("Impossible de creer le fichier graphique\n");
        return;
    }

    fprintf(f, "digraph evolution {\n");
    fprintf(f, "    rankdir=TB;\n");
    fprintf(f, "    size=\"8,6\";\n");
    fprintf(f, "    node [shape=none fontname=\"Arial\"];\n");
    fprintf(f, "    edge [color=\"#4a90e2\"];\n");

    // Titre
    fprintf(f, "    graph [label=<<b>Evolution de la population de %s</b>>, labelloc=t, fontsize=16];\n\n", nom_espece);

    // Création du tableau principal
    fprintf(f, "    tableau [label=<\n");
    fprintf(f, "        <table border=\"0\" cellborder=\"0\" cellspacing=\"0\">\n");

    // Axe Y (populations)
    fprintf(f, "            <tr>\n");
    fprintf(f, "                <td width=\"50\" height=\"300\" valign=\"top\">\n");
    fprintf(f, "                    <table border=\"0\" cellborder=\"0\" cellspacing=\"0\">\n");
    for(int i = 10; i >= 0; i--) {
        float val = min_pop + (max_pop - min_pop) * i / 10.0;
        fprintf(f, "                        <tr><td height=\"27\" align=\"right\">%.1f</td></tr>\n", val);
    }
    fprintf(f, "                    </table>\n");
    fprintf(f, "                </td>\n");

    // Zone du graphique
    fprintf(f, "                <td width=\"500\" height=\"300\" border=\"1\">\n");
    fprintf(f, "                    <table border=\"0\" cellborder=\"0\" cellspacing=\"0\">\n");
    fprintf(f, "                        <tr><td width=\"500\" height=\"300\"></td></tr>\n");
    fprintf(f, "                    </table>\n");
    fprintf(f, "                </td>\n");
    fprintf(f, "            </tr>\n");

    // Axe X (mois)
    fprintf(f, "            <tr>\n");
    fprintf(f, "                <td></td>\n");
    fprintf(f, "                <td>\n");
    fprintf(f, "                    <table border=\"0\" cellborder=\"0\" cellspacing=\"0\" width=\"500\">\n");
    fprintf(f, "                        <tr>\n");
    for(int i = 0; i < nb_mois; i++) {
        fprintf(f, "                            <td align=\"center\" width=\"%d\">Mois %d</td>\n", 500/nb_mois, i);
    }
    fprintf(f, "                        </tr>\n");
    fprintf(f, "                    </table>\n");
    fprintf(f, "                </td>\n");
    fprintf(f, "            </tr>\n");
    fprintf(f, "        </table>\n");
    fprintf(f, "    >];\n\n");

    // Points de données et connexions
    for(int i = 0; i < nb_mois; i++) {
        float x = 50 + (500 * i) / (nb_mois - 1);
        float y = 300 - (270 * (historique[i] - min_pop) / (max_pop - min_pop));

        fprintf(f, "    p%d [pos=\"%f,%f!\", shape=circle, width=0.2, style=filled, fillcolor=\"#4a90e2\", color=\"#4a90e2\"];\n",
                i, x, y);

        if(i < nb_mois - 1) {
            fprintf(f, "    p%d -> p%d [penwidth=2];\n", i, i + 1);
        }
    }

    fprintf(f, "}\n");
    fclose(f);

    // Générer l'image
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "\"\"%s\" -Tpng \"%s\" -o \"temp/evolution_%s.png\"\"",
             DOT_PATH, filename, nom_espece);

    if(system(cmd) == 0) {
        char png_path[256];
        snprintf(png_path, sizeof(png_path), "temp/evolution_%s.png", nom_espece);
        openImage(png_path);
    } else {
        printf("Erreur lors de la generation du graphique\n");
    }
}