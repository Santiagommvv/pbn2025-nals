#include <stdio.h>
#include <stdlib.h>
#include "../include/nodo.h"
#include "../include/listaES.h"


//Inserta un nodo al inicio de la lista y ese nodo es la cabeza
void insertarAlInicio(Nodo** cabeza, int dato){
    Nodo* nuevaCabeza = crearNodo(dato);

    if (*cabeza == NULL){
        *cabeza = nuevaCabeza;
        miLista.cabeza = nuevaCabeza;
        miLista.cola = nuevaCabeza;
    }else{
        nuevaCabeza->siguienteNodo = *cabeza;
        *cabeza = nuevaCabeza;
    }

    miLista.tamanio++;
}

//Inserta un nodo al final de la lista y ese nodo es la cola
void insertarAlFinal(Nodo** cola, int dato){
    Nodo* nuevaCola = crearNodo(dato);


    if(*cola == NULL){
        *cola = nuevaCola;
        miLista.cabeza = nuevaCola;
        miLista.cola = nuevaCola;
    }else{
        (*cola)->siguienteNodo = nuevaCola;
        *cola = nuevaCola;
        miLista.cola = nuevaCola;
    }

    miLista.tamanio++;
}

void eliminarNodo(Nodo** cabeza, int dato) {
    Nodo* actual = *cabeza;
    Nodo* anterior = NULL;

    if (miLista.tamanio == 0 || actual == NULL){
        return;
    }

    //caso cabeza es el nodo a eliminar
    if (actual->dato == dato) {
        miLista.tamanio--;
        if (actual-> siguienteNodo != NULL){
            miLista.cabeza = actual->siguienteNodo;
        }
        free(actual);
        return;
    }

    //demás casos
    anterior = actual;
    while (actual != NULL) {
        //elemento a eliminar es cola
        if (actual == miLista.cola && actual->dato == dato ) {
            miLista.cola = anterior;
            anterior->siguienteNodo = NULL;
            miLista.tamanio--;
            free(actual);
            actual = NULL;
            return;
        }

        if (actual->dato == dato) {
            miLista.tamanio--;
            anterior->siguienteNodo = actual->siguienteNodo;
            free(actual);
            actual = NULL;
            return;
        }
        actual = actual->siguienteNodo;
    }

}

Nodo* buscarNodo(Nodo* cabeza, int dato){
    Nodo* nodoActual = cabeza;

    while (nodoActual != NULL) {
        if (nodoActual->dato == dato) {
            return nodoActual;
        }
        nodoActual = nodoActual->siguienteNodo;
    }

    return NULL;
}

void imprimirLista(Nodo* cabeza){
    if (miLista.tamanio == 0) {
        printf("[]\n");
        return;
    }

    printf("[ ");
    Nodo* actual = cabeza;

    while (actual != NULL) {
        printf("%d ", actual->dato);
        if (actual->siguienteNodo != NULL) {
            printf("-> ");
        }
        actual = actual->siguienteNodo;
    }

    printf("]\n");
}

void liberarLista(Nodo** cabeza){
    Nodo* actual = *cabeza;

    while (actual != NULL){
        Nodo* siguiente = actual->siguienteNodo;
        free(actual);
        actual = siguiente;
    }
    *cabeza = NULL;

    miLista.cabeza = NULL;
    miLista.cola = NULL;
    miLista.tamanio = 0;
}