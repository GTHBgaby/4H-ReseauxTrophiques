#include "fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}*/