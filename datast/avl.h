#include "../dominio/alumno.h"

#ifndef AVL_H
#define AVL_H
typedef struct NodoAVL {
    Alumno alumno;
    int altura;
    struct NodoAVL* izq;
    struct NodoAVL* der;
} NodoAVL;

int max(int a, int b){}
int altura(NodoAVL* n) {}
int balance(NodoAVL* n) {}

void imprimirInOrden(NodoAVL* raiz) {}

NodoAVL* rotarDerecha(NodoAVL* y) {}
NodoAVL* rotarIzquierda(NodoAVL* x) {}
NodoAVL* insertarAVL(NodoAVL* nodo, Alumno alumno) {}


#endif //AVL_H
