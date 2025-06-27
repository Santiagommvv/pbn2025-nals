/*
#ifndef LISTA_ALUMNO_H
#define LISTA_ALUMNO_H

#include <string.h>
#include "alumno.h"
#include "lista_materia.h"

typedef struct NodoAlumno{
    Alumno datos;
    struct NodoAlumno* siguiente;
} NodoAlumno;

//Funciones
NodoAlumno* agregarAlumno(NodoAlumno** cabeza, Alumno nuevo);
NodoAlumno* buscarAlumnoPorNombre(NodoAlumno* cabeza, const char* nombre);
NodoAlumno* buscarAlumnoPorID(NodoAlumno* cabeza, int id);
void buscarAlumnosPorEdad(NodoAlumno* cabeza, int edadMin, int edadMax);
void eliminarAlumno(NodoAlumno** cabeza, int id);
void modificarAlumno(NodoAlumno* cabeza, int id);
void listarAlumnos(NodoAlumno* cabeza);
void listarMateriasAprobadas(Alumno* alumno, NodoMateria* listaMateria);
void listarMateriasRendidas(Alumno* alumno, NodoMateria* listaMaterias);

#endif // LISTA_ALUMNO_H
*/
