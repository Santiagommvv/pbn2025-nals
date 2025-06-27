/*
#include "../include/avl.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

int altura(NodoAVL* n) {
    return n ? n->altura : 0;
}

int balance(NodoAVL* n) {
    return n ? altura(n->izq) - altura(n->der) : 0;
}

NodoAVL* rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    return x;
}

NodoAVL* rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    return y;
}

NodoAVL* insertarAVL(NodoAVL* nodo, Alumno alumno) {
    if (nodo == NULL) {
        printf("insertando en uno vacío");
        NodoAVL* nuevo = malloc(sizeof(NodoAVL));
        nuevo->alumno = alumno;
        nuevo->izq = nuevo->der = NULL;
        nuevo->altura = 1;
        return nuevo;
    }
    printf("insertando en uno poblado");
    if (alumno.id < nodo->alumno.id)
        nodo->izq = insertarAVL(nodo->izq, alumno);
    else if (alumno.id > nodo->alumno.id)
        nodo->der = insertarAVL(nodo->der, alumno);
    else
        return nodo; // IDs duplicados no se insertan

    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int balanceo = balance(nodo);

    // Rotaciones según el caso
    if (balanceo > 1 && alumno.id < nodo->izq->alumno.id)
        return rotarDerecha(nodo);

    if (balanceo < -1 && alumno.id > nodo->der->alumno.id)
        return rotarIzquierda(nodo);

    if (balanceo > 1 && alumno.id > nodo->izq->alumno.id) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (balanceo < -1 && alumno.id < nodo->der->alumno.id) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void imprimirInOrden(NodoAVL* raiz) {
    if (!raiz) return;
    imprimirInOrden(raiz->izq);
    printf("<<%i# %s %i>>\n", raiz->alumno.id, raiz->alumno.nombre, raiz->alumno.edad);
    imprimirInOrden(raiz->der);
}
*/