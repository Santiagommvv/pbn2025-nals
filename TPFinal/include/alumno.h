#ifndef ALUMNO_H
#define ALUMNO_H

#include "config.h"

typedef struct Alumno{
    int id;
    char nombre[100];
    int edad;

    int materiasInscripto[MAX_MATERIAS_POR_ALUMNO];
    int cantidadDeMaterias;

    struct Alumno* siguiente;

} Alumno;

Alumno* crearAlumno(const char* nombre, int edad);
Alumno* buscarAlumnoPorNombre(Alumno* cabeza, char* nombre);
Alumno* buscarAlumnoPorId(Alumno* cabeza, int id);
void eliminarAlumno(Alumno** cabeza, int id);
void modificarAlumno(Alumno* cabeza, int id);


#endif // ALUMNO_H