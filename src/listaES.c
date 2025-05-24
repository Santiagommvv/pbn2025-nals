#include <stdio.h>
#include <stdlib.h>
#include "include/nodo.h"
#include "include/listaES.h"

//Inserta un nodo al inicio de la lista y ese nodo es la cabeza
void insertarAlInicio(Nodo** cabeza, int dato){
   Nodo* nuevaCabeza = crear(dato);
    if (*cabeza== NULL){
        *cabeza = nuevaCabeza;
    }else{
   //  nuevaCabeza -> &cabeza;
    }
}

void insertarAlFinal(Nodo* cabeza, int dato){

}

void eliminarNodo(Nodo** cabeza, int dato){

}

void eliminarUltimoNodo(Nodo** cabeza, int dato){

}

Nodo* buscarNodo(Nodo* cabeza, int dato){

}

void imprimirLista(Nodo* cabeza){

}

void liberarLista(Nodo** cabeza){

}