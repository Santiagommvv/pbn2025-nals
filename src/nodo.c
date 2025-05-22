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



void setNext(Nodo* actual, Nodo* siguiente){
    if(actual!= NULL){
        actual->siguienteNodo = siguiente
    }
}

Nodo* next(Nodo* actual) {
    if (hasNext(actual)==1){
        return actual->siguienteNodo;
    }
  return NULL;
}

int hasNext(Nodo* actual){
    if (actual!= NULL && actual->siguienteNodo != NULL){
        return 1;
    }
  return 0;
}