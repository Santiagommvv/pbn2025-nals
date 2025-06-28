#ifndef PAGINADO_H
#define PAGINADO_H

#include "../dominio/avl.h"
#include "../dominio/lista_materia.h"
#include "../include/config.h"


// Paginado para alumnos
void listarAlumnosPaginadoAVL(NodoAVL* raiz);

// Paginado para materias
void listarMateriasPaginado(NodoMateria* cabeza);

// Paginado con funcionalidades adicionales (buscar, ordenar, etc.)
void listarAlumnosAvanzado(NodoAVL* raiz);
void listarMateriasAvanzado(NodoMateria* cabeza);

#endif // PAGINADO_H