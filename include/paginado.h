#ifndef PAGINADO_H
#define PAGINADO_H

#include "lista_alumno.h"
#include "lista_materia.h"
#include "config.h"


// Paginado para alumnos
void listarAlumnosPaginado(NodoAlumno* cabeza);

// Paginado para materias
void listarMateriasPaginado(NodoMateria* cabeza);

#endif // PAGINADO_H