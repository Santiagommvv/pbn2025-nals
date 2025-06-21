#ifndef LISTA_ALUMNO_H
#define LISTA_ALUMNO_H

#include "alumno.h"

typedef struct NodoAlumno{
    Alumno datos;
    struct NodoAlumno* siguiente;
} NodoAlumno;

//Funciones
NodoAlumno* agregarAlumno(NodoAlumno** cabeza, Alumno nuevo);
NodoAlumno* buscarAlumnoPorNombre(NodoAlumno* cabeza, const char* nombre);
NodoAlumno* buscarAlumnoPorId(NodoAlumno* cabeza, int id);
void eliminarAlumno(NodoAlumno** cabeza, int id);
void modificarAlumno(NodoAlumno* cabeza, int id);
void listarAlumnos(NodoAlumno* cabeza);

#endif // LISTA_ALUMNO_H

