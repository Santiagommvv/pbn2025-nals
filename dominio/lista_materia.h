#ifndef LISTA_MATERIA_H
#define LISTA_MATERIA_H

#include "materia.h"

typedef struct NodoMateria{
    Materia datos;
    struct NodoMateria* siguiente;
} NodoMateria;

// Funciones
NodoMateria* agregarMateria(NodoMateria** cabeza, const char* nombre);
int eliminarMateria(NodoMateria** cabeza, int id);
int modificarMateria(NodoMateria* cabeza, int id);
int modificarCorrelativasMateria(NodoMateria* cabeza, int id);
void listarMaterias(NodoMateria* cabeza);
int contarNodosMaterias(NodoMateria* cabeza);
NodoMateria* buscarMateriaPorID(NodoMateria* cabeza, int id);
NodoMateria* buscarMateriaPorNombre(NodoMateria* cabeza, const char* nombre);
void liberarListaMaterias(NodoMateria* lista);
// Funcion para actualizar la referencia global a la lista de materias
void actualizarReferenciaMaterias(NodoMateria* lista);
// Funcion comun para visualizar una materia con formato
void visualizarMateria(Materia m, int formatoAvanzado);


#endif // LISTA_MATERIA_H
