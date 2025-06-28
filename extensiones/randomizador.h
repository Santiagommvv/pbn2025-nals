#ifndef RANDOMIZADOR_H
#define RANDOMIZADOR_H

#include "../dominio/avl.h"
#include "../dominio/lista_materia.h"

void generarAlumnosAleatorios(NodoAVL** raiz, int cantidad);
void generarMateriasAleatorias(NodoMateria** lista, int cantidad);

#endif // RANDOMIZADOR_H