// 1: INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "../util/utils.h"

// 2: FUNCIONES UTILITARIAS PARA MANEJO DEL AVL

// basicas
int max(int a, int b) {
    return (a > b) ? a : b;
}

int altura(NodoAVL* n) {
    return n ? n->altura : 0;
}

int balance(NodoAVL* n) {
    return n ? altura(n->izq) - altura(n->der) : 0;
}

// rotaciones
NodoAVL* rotarDerecha(NodoAVL* y) {
    if (!y || !y->izq) return y; // Verificar si y o y->izq es NULL
    
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    return x;
}

NodoAVL* rotarIzquierda(NodoAVL* x) {
    if (!x || !x->der) return x; // Verificar si x o x->der es NULL
    
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    return y;
}

// 3: FUNCIONES DE BÚSQUEDA EN EL AVL

NodoAVL* buscarAlumnoPorApellidoAVL(NodoAVL* raiz, const char* apellido){
    if(!raiz) return NULL;

    // Busca en el subarbol izquierdo
    NodoAVL* izq = buscarAlumnoPorApellidoAVL(raiz->izq, apellido);
    if(izq) return izq;

    // Comparar apellido actual
    if(strcasecmp(raiz->alumno.apellido, apellido)==0){
        return raiz;
    }

    // Buscar en el subarbol derecho
    return buscarAlumnoPorApellidoAVL(raiz->der, apellido);
}

NodoAVL* buscarAlumnoPorIDAVL(NodoAVL* raiz, int id) {
    if(!raiz) return NULL;

    if(id == raiz->alumno.id) {
        return raiz;
    } else if(id < raiz->alumno.id){
        return buscarAlumnoPorIDAVL(raiz->izq,id);
    } else {
        return buscarAlumnoPorIDAVL(raiz->der,id);
    }
}

void buscarAlumnosPorEdadAVL(NodoAVL* raiz, int edadMin, int edadMax, int* encontrados){
    if (!raiz) return;

    // subarbol izquierdo
    buscarAlumnosPorEdadAVL(raiz->izq, edadMin, edadMax, encontrados);
    
    // verificar alumno actual
    int edad = raiz->alumno.edad;
    if(edad >= edadMin && edad <= edadMax) {
        visualizarAlumno(raiz->alumno, 0); // Usar la funcion comun
        (*encontrados)++;
    }

    // subarbol derecho
    buscarAlumnosPorEdadAVL(raiz->der, edadMin, edadMax, encontrados);
}

NodoAVL* minimoNodoAVL(NodoAVL* nodo){
    NodoAVL* actual = nodo;
    while(actual && actual->izq)
        actual = actual->izq;
    return actual;
}

// 4: FUNCIONES DE MANIPULACIÓN DEL AVL

NodoAVL* insertarAVL(NodoAVL* nodo, Alumno alumno) {
    if (nodo == NULL) {
        NodoAVL* nuevo = malloc(sizeof(NodoAVL));
        nuevo->alumno = alumno;
        nuevo->izq = nuevo->der = NULL;
        nuevo->altura = 1;
        return nuevo;
    }

    if (alumno.id < nodo->alumno.id)
        nodo->izq = insertarAVL(nodo->izq, alumno);
    else if (alumno.id > nodo->alumno.id)
        nodo->der = insertarAVL(nodo->der, alumno);
    else
        return nodo; // IDs duplicados no se insertan

    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int balanceo = balance(nodo);

    // Rotaciones segun el caso
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

NodoAVL* eliminarAlumnoAVL(NodoAVL* raiz, int id){
    if (!raiz) {
        printf("Alumno con ID %d no encontrado\n", id);
        return NULL;
    }
    
    if(id<raiz->alumno.id){
        raiz->izq = eliminarAlumnoAVL(raiz->izq, id);
    } else if(id > raiz-> alumno.id){
        raiz->der = eliminarAlumnoAVL(raiz->der, id);
    } else {
        // Nodo encontrado: eliminacion

        // solo un hijo o sin hijos
        if(!raiz->izq || !raiz->der){
            NodoAVL* temp = raiz->izq ? raiz->izq : raiz->der;

            // sin hijos
            if(!temp) {
                temp = raiz;
                raiz = NULL;
            } else {
                //un hijo
                *raiz = *temp;
            }
            free(temp);
        } else {
            // dos hijos
            NodoAVL* temp = minimoNodoAVL(raiz->der);

            // copia datos del sucesor inorden
            raiz->alumno = temp->alumno;

            // elimina sucesor
            raiz->der = eliminarAlumnoAVL(raiz->der,temp->alumno.id);
        }
    }

    // si el arbol queda vacio
    if(!raiz)
    return NULL;

    //actualizar altura
    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));

    //calcular balance
    int bal = balance(raiz);

    // Balancear el arbol

    // izquierda izquierda
    if(bal >1 && balance(raiz->izq) >= 0)
    return rotarDerecha(raiz);

    // izquierda Derecha
    if (bal > 1 && balance(raiz->izq)< 0){
        raiz->izq = rotarIzquierda(raiz->izq);
        return rotarDerecha(raiz);
    }

    // derecha derecha
    if (bal < -1 && balance (raiz->der) <= 0)
    return rotarIzquierda(raiz);

    // derecha izquierda
    if(bal < -1 && balance(raiz->der) > 0) {
        raiz->der = rotarDerecha(raiz->der);
        return rotarIzquierda(raiz);
    }

    return raiz;
}

void modificarAlumnoAVL(NodoAVL* raiz, int id){
    if (!raiz) {
        printf("Alumno con ID %d no encontrado\n", id);
        return;
    }

    if(id< raiz->alumno.id) {
        modificarAlumnoAVL(raiz->izq, id);
    } else if(id>raiz->alumno.id){
        modificarAlumnoAVL(raiz->der, id);
    } else {
        // encontrado
        printf("Modificando alumno con ID %d\n", id);

        pedirString("Ingrese nuevo nombre: ", raiz->alumno.nombre, MAX_NOMBRE);
        raiz->alumno.edad = pedirInt("Ingrese edad: ");
    }
}

void listarMateriasRendidas(Alumno* alumno, NodoMateria* listaMaterias) {
    if (!alumno) {
        printf("Alumno no encontrado.\n");
        return;
    }
    
    if (!listaMaterias) {
        printf("Lista de materias no disponible.\n");
        return;
    }
    
    if(alumno->cantidadMateriasRendidas == 0){
        printf("El alumno no ha rendido ninguna materia.\n");
        return;
    }

    printf("Materias rendidas por %s\n", alumno->nombre);
    for(int i = 0; i<alumno->cantidadMateriasRendidas; i++){
        MateriaRendida rendida = alumno->materiasRendidas[i];
        NodoMateria* materia = buscarMateriaPorID(listaMaterias, rendida.IDMateria);
        if(materia) {
            printf("ID: %d | Nombre: %s | Nota: %.2f | Estado: %s\n", rendida.IDMateria, materia->datos.nombre, rendida.nota, rendida.aprobo ? "Aprobado" : "Desaprobado");
        }
    }
}

// Contar cantidad de nodos
int contarAlumnos(NodoAVL* nodo){
    if(!nodo) return 0;
    return 1 + contarAlumnos(nodo->izq)+contarAlumnos(nodo->der);
}

// recorrer el AVL en orden y guardar los alumnos en un arreglo
void recorrerInOrden(NodoAVL* nodo, Alumno* alumnos, int* index){
    if(!nodo) return;

    recorrerInOrden(nodo->izq, alumnos, index);
    alumnos[*index] = nodo->alumno;
    (*index)++;
    recorrerInOrden(nodo->der,alumnos,index);
}

// Liberar memoria del AVL
void liberarAVL(NodoAVL* nodo) {
    if (!nodo) return;
    liberarAVL(nodo->izq);
    liberarAVL(nodo->der);
    free(nodo);
}