#ifndef LISTAES_H
#define LISTAES_H

#include "nodo.h"

typedef struct{
    Nodo * cabeza;
    Nodo * cola;
    int tamanio;
} Lista;

Lista miLista;

//Nodo* crearNodo(int dato); //no estoy seguro si sea una función que use lista
void insertarAlInicio(Nodo** cabeza, int dato);
void insertarAlFinal(Nodo** cola, int dato);  //chequear que acá está con doble puntero y en la implementación no.
void eliminarNodo(Nodo** cabeza, int dato);
Nodo* buscarNodo(Nodo* cabeza, int dato);
void imprimirLista(Nodo* cabeza);
void liberarLista(Nodo** cabeza);

#endif //LISTAES_H

