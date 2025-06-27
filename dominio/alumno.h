#ifndef ALUMNO_H
#define ALUMNO_H

#include "config.h"
#include "materia.h"

typedef struct MateriaRendida{
    int IDMateria;
    float nota;
    int aprobo;
} MateriaRendida;

typedef struct Alumno{
    int id;
    char nombre[100];
    int edad;

    int materiasInscripto[MAX_MATERIAS_POR_ALUMNO];
    int cantidadDeMateriasInscripto;

    MateriaRendida materiasRendidas[MAX_MATERIAS_POR_ALUMNO];
    int cantidadMateriasRendidas;
} Alumno;

Alumno crearAlumno(const char* nombre, int edad);
int rendirMateria(Alumno* alumno, int idMateria, float nota);
int inscribirAlumnoEnMateria(Alumno* alumno, Materia* materia);

#endif // ALUMNO_H