#include <stdio.h>
#include <stdlib.h>
#include "include\nodo.h"

// Crea un nuevo nodo
Nodo* crearNodo(int dato) {
    //Reserva la memoria para el nodo
    Nodo * nuevoNodo = malloc(sizeof(Nodo));
    if (nuevoNodo == NULL) {
        return NULL;
    }
    //Reserva la memoria del dato a ingresar y la libera si el dato es nulo
    nuevoNodo->dato = malloc(sizeof(int));
    if(nuevoNodo->dato == NULL) {
        free(nuevoNodo);
        return NULL;
    }

    *(nuevoNodo->dato) = dato;
    nuevoNodo->siguienteNodo = NULL;

    return nuevoNodo;
}