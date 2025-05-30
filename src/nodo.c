#include <stdio.h>
#include <stdlib.h>
#include "../include/nodo.h"

// Crea un nuevo nodo
Nodo* crearNodo(int dato) {
    //Reserva la memoria para el nodo
    Nodo* nuevoNodo = malloc(sizeof(Nodo));
    if (nuevoNodo == NULL) {
        return NULL;
    }
    
    nuevoNodo->dato = dato;
    nuevoNodo->siguienteNodo = NULL;

    return nuevoNodo;
}