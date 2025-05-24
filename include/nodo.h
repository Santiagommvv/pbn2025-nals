#ifndef NODO_H
#define NODO_H
typedef struct Nodo{
    int dato;
    struct Nodo* siguienteNodo;
} Nodo;

Nodo* crearNodo(int dato);
void setNext(Nodo* actual, Nodo* siguiente);
Nodo* next(Nodo* actual);
int hasNext(Nodo* actual);

#endif //NODO_H