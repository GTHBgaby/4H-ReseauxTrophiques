#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>
#include "generationDOT.h"
#include "Graph.h"

#define MKDIR(dir) _mkdir(dir)
#define DOT_PATH "C:\\Program Files\\Graphviz\\bin\\dot.exe"

void openImage(const char* file) {
    char commande[512];
    snprintf(commande, sizeof(commande), "start \"\" \"%s\"", file);
    system(commande);
}

void genererFichierDot(Graph* graph, const char* nom_fichier) {
    MKDIR("temp");

    char dot_path[256];
    char png_path[256];
    snprintf(dot_path, sizeof(dot_path), "../GrapheDOT/%s.dot", nom_fichier);
    snprintf(png_path, sizeof(png_path), "temp/%s.png", nom_fichier);

    FILE* f = fopen(dot_path, "w");
    if (!f) {
        printf("Impossible de creer le fichier %s\n", dot_path);
        return;
    }

    fprintf(f, "digraph ecosysteme {\n");
    fprintf(f, "    rankdir=TB;\n");
    fprintf(f, "    compound=true;\n");
    fprintf(f, "    newrank=true;\n\n");

    if (graph->nbEspeces == 9) {
        fprintf(f, "    label = \"Cours d'eau\";\n");
        fprintf(f, "    bgcolor = \"lightblue\";\n");
    } else if (graph->nbEspeces == 14) {
        fprintf(f, "    label = \"Forêt Européenne\";\n");
        fprintf(f, "    bgcolor = \"lightgreen\";\n");
    } else if (graph->nbEspeces == 13) {
        fprintf(f, "    label = \"Savane\";\n");
        fprintf(f, "    bgcolor = \"lightyellow\";\n");
    }

    fprintf(f, "    node [shape=ellipse, style=filled, fillcolor=white];\n\n");

    for (int i = 1; i <= graph->nbEspeces; i++) {
        if (!graph->especes[i].supp) {
            fprintf(f, "    \"%s\" [label=\"%s\\n[%.0f]\\n%.2f%%\"]\n",
                    graph->especes[i].nom,
                    graph->especes[i].nom,
                    graph->especes[i].population,
                    graph->especes[i].taux_accroissement);
        }
    }

    fprintf(f, "\n");

    for (int i = 1; i <= graph->nbEspeces; i++) {
        if (!graph->especes[i].supp) {
            Arc* arc = graph->especes[i].arc;
            while (arc) {
                fprintf(f, "    \"%s\" -> \"%s\" [label=\"%.1f\", weight=%.1f, penwidth=%.1f]\n",
                        graph->especes[i].nom,
                        graph->especes[arc->IDs].nom,
                        arc->infl,
                        arc->infl,
                        arc->infl * 3);
                arc = arc->arcsuivant;
            }
        }
    }

    for (int i = 1; i <= graph->nbEspeces; i++) {
        if (!graph->especes[i].supp) {
            fprintf(f, "    {rank=same; \"%s\"}\n", graph->especes[i].nom);
        }
    }

    fprintf(f, "}\n");
    fclose(f);

    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "\"\"%s\" -Tpng \"%s\" -o \"%s\"\"",
             DOT_PATH, dot_path, png_path);

    int result = system(cmd);
    if (result != 0) {
        printf("Erreur lors de la generation de l'image\n");
        return;
    }

    FILE* test = fopen(png_path, "r");
    if (!test) {
        printf("L'image n'a pas pu etre generee\n");
        return;
    }
    fclose(test);

    openImage(png_path);
}

void DelTempFile(const char* nom_fichier) {
    char dot_path[256];
    char png_path[256];
    char original_dot_path[256];

    snprintf(original_dot_path, sizeof(original_dot_path), "../GrapheDOT/%s.dot", nom_fichier);
    remove(original_dot_path);

    snprintf(dot_path, sizeof(dot_path), "temp/%s.dot", nom_fichier);
    remove(dot_path);

    snprintf(png_path, sizeof(png_path), "temp/%s.png", nom_fichier);
    remove(png_path);

    _rmdir("temp");
}