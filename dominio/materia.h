#ifndef MATERIA_H
#define MATERIA_H

#include "../include/config.h"

typedef struct Materia{
    int id;
    char nombre[100];

    int alumnosInscriptos[MAX_ALUMNOS_POR_MATERIA];
    int cantidadAlumnos;
    
    int correlativas[MAX_CORRELATIVAS];
    int cantidadCorrelativas;
} Materia;


Materia crearMateria(const char* nombre);
int agregarCorrelativa(Materia* materia, int idCorrelativa);

#endif // MATERIA_H


