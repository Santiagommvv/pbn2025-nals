#ifndef PAGINADO_H
#define PAGINADO_H

#include "../dominio/avl.h"
#include "../dominio/lista_materia.h"
#include "../config/config.h"

// Paginado con funcionalidades adicionales (buscar, ordenar, etc.)
void listarAlumnosPaginado(NodoAVL* raiz, int especial);
void listarMateriasPaginado(NodoMateria* cabeza, int especial);

#endif // PAGINADO_H