//
// Created by zbsam145 on 16/5/2025.
//

#ifndef NODO_H
#define NODO_H
typedef struct Nodo{
    void* data;   //todavía no sabemos el tipo
    struct Nodo* siguienteNodo;
} Nodo;
#endif //NODO_H
