#ifndef MATERIA_H
#define MATERIA_H

#include "config.h"

typedef struct Materia{
    int id;
    char nombre[100];

    int alumnosInscriptos[MAX_ALUMNOS_POR_MATERIA];
    
} Materia;
#endif // MATERIA_H

