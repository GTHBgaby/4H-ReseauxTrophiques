#ifndef LECTEURFICHIERS_H
#define LECTEURFICHIERS_H
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"



Graph* read_ecosystem_file(const char* filename);
void print_ecosystem(Graph* g);
void destroy_graph(Graph* g);




#endif //LECTEURFICHIERS_H
