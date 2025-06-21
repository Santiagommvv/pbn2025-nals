#ifndef ALUMNO_H
#define ALUMNO_H

#include "config.h"

typedef struct Alumno{
    int id;
    char nombre[100];
    int edad;

    int materiasInscripto[MAX_MATERIAS_POR_ALUMNO];
    int cantidadDeMaterias;
} Alumno;

Alumno crearAlumno(const char* nombre, int edad);


#endif // ALUMNO_H