#include <stdio.h>
#include <stdlib.h>
#include "include/nodo.h"

// Crea un nuevo nodo
Nodo* crearNodo(int dato) {
    //Reserva la memoria para el nodo
    Nodo * nuevoNodo = malloc(sizeof(Nodo));
    if (nuevoNodo == NULL) {
        return NULL;
    }
    //Reserva la memoria del dato a ingresar y la libera si el dato es nulo
  //  nuevoNodo->dato = malloc(sizeof(int));
    if(nuevoNodo->dato == NULL) {
        free(nuevoNodo);
        return NULL;
    }

   // *(nuevoNodo->dato) = dato;
    nuevoNodo->siguienteNodo = NULL;

    return nuevoNodo;
}

//La funcion te permite saber que tiene almacenado el nodo actual.Devuelve su contenido.
int* getDato(Nodo* actual){
    if (actual != NULL && actual->dato != NULL){
        return actual->dato;
    }
    return NULL;
}

//La funcion te permite elegir a quien apunta el nodo actual
void setNext(Nodo* actual, Nodo* siguiente){
    if(actual != NULL){
        actual->siguienteNodo = siguiente;
    }
}

//La funcion devuelve el proximo nodo si es que lo hay
Nodo* next(Nodo* actual) {
    if (hasNext(actual)){
        return actual->siguienteNodo;
    }
  return NULL;
}

//La funcion te permite saber si hay un nodo siguiente 
int hasNext(Nodo* actual){
    if (actual!= NULL && actual->siguienteNodo != NULL){
        return 1;
    }
  return 0;
}

void eliminarNodo(Nodo* actual){

}