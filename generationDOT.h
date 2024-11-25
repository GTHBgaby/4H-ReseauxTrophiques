#ifndef INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
#define INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H

typedef struct {
    char* nom;
    char* couleur;
    int niveau;
    int population;
    float taux_accroissement;
} Espece;

void generer_dot(Espece* especes, int nb_especes, int (*relations)[2], int nb_relations, const char* filename);
void modifier_espece(Espece* espece);

#endif //INC_4E_RESEAUXTROPHIQUES_GENERATIONDOT_H
