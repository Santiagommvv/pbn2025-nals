#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "../dominio/avl.h"
#include "lista_materia.h"

// Carga listas al iniciar
void cargarDatos(NodoAVL** alumnos, NodoMateria** materias);

// Guarda listas antes de salir
void guardarDatos(NodoAVL* alumnos, NodoMateria* materias);

#endif