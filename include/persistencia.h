#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "lista_alumno.h"
#include "lista_materia.h"

// Carga listas al iniciar
void cargarDatos(NodoAlumno** listaAlumnos, NodoMateria** listarMaterias);

// Guarda listas antes de salir
void guardarDatos(NodoAlumno* listasAlumnos, NodoMateria* listasMateria);

#endif