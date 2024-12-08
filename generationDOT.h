#ifndef INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
#define INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H

#define MAX_ESPECES 100
#define MAX_LIGNE 256

#include "Graph.h"

void openImage(const char* file);
void genererFichierDot(Graph* graph, const char* file);
void DelTempFile(const char* file);

#endif //INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
