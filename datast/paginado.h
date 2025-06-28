#ifndef PAGINADO_H
#define PAGINADO_H

#include "../dominio/avl.h"
#include "../dominio/lista_materia.h"
#include "../include/config.h"

// Paginado con funcionalidades adicionales (buscar, ordenar, etc.)
void listarAlumnosPaginado(NodoAVL* raiz);
void listarMateriasPaginado(NodoMateria* cabeza);

#endif // PAGINADO_H