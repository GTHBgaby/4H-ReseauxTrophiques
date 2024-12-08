#ifndef INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
#define INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H

#include "Graph.h"

#define MAX_ESPECES 100
#define MAX_LIGNE 256
#define MKDIR(dir) _mkdir(dir)
#define DOT_PATH "C:\\Program Files\\Graphviz\\bin\\dot.exe"

void openImage(const char* file);
void genererFichierDot(Graph* graph, const char* file);
void DelTempFile(const char* file);

#endif //INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
