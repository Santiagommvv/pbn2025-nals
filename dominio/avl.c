#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "../datast/utils.h"


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

void imprimirInOrden(NodoAVL* raiz) {
    if (!raiz) return;
    imprimirInOrden(raiz->izq);
    printf("<<%i# %s %i>>\n", raiz->alumno.id, raiz->alumno.nombre, raiz->alumno.edad);
    imprimirInOrden(raiz->der);
}

// Contar cantidad de nodos
int contarAlumnos(NodoAVL* nodo){
    if(!nodo) return 0;
    return 1 + contarAlumnos(nodo->izq)+contarAlumnos(nodo->der);
}

// Llenar el array
void recorrerInOrden(NodoAVL* nodo, Alumno* alumnos, int* index){
    if(!nodo) return;

    recorrerInOrden(nodo->izq, alumnos, index);
    alumnos[*index] = nodo->alumno;
    (*index)++;
    recorrerInOrden(nodo->der,alumnos,index);
}

NodoAVL* buscarAlumnoPorNombreAVL(NodoAVL* raiz, const char* nombre){
    if(!raiz) return NULL;

    // Busca en el subarbol izquierdo
    NodoAVL* izq = buscarAlumnoPorNombreAVL(raiz->izq, nombre);
    if(izq) return izq;

    // Comparar nombre actual
    if(strcmp(raiz->alumno.nombre, nombre)==0){
        return raiz;
    }

    // Buscar en el subarbol derecho
    return buscarAlumnoPorNombreAVL(raiz->der, nombre);
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

NodoAVL* minimoNodoAVL(NodoAVL* nodo){
    NodoAVL* actual = nodo;
    while(actual && actual->izq)
        actual = actual->izq;
    return actual;
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

void buscarAlumnosPorEdadAVL(NodoAVL* raiz, int edadMin, int edadMax, int* encontrados){
    if (!raiz) return;

    // subarbol izquierdo
    buscarAlumnosPorEdadAVL(raiz->izq, edadMin, edadMax, encontrados);
    
    // verificar alumno actual
    int edad = raiz->alumno.edad;
    if(edad >= edadMin && edad <= edadMax) {
        Alumno a = raiz->alumno;
        printf("ID: %d | Nombre: %s | Edad: %d | Materias inscriptas: %d\n", a.id, a.nombre, a.edad, a.cantidadDeMateriasInscripto);
        (*encontrados)++;
    }

    // subarbol derecho
    buscarAlumnosPorEdadAVL(raiz->der, edadMin, edadMax, encontrados);
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

void listarMateriasAprobadas(Alumno* alumno, NodoMateria* listaMaterias){
    if(!alumno) {
        printf("Alumno no encontrado.\n");
        return;
    }

    int encontradas = 0;

    printf("Materias aprobadas de %s (ID %d):\n", alumno->nombre, alumno->id);

    for(int i= 0;i< alumno->cantidadMateriasRendidas; i++){
        MateriaRendida materia = alumno->materiasRendidas[i];
        if(materia.aprobo){
            NodoMateria* nodoMateria = buscarMateriaPorID(listaMaterias, materia.IDMateria);
            printf("- %s (ID %d), nota: %.2f\n",
                    nodoMateria ? nodoMateria->datos.nombre : "Desconocida",
                    materia.IDMateria,
                    materia.nota);
                encontradas++;            
        }
    }

    if(encontradas == 0){
        printf("No hay materias aprobadas registradas.\n");
    }
}

void listarMateriasRendidas(Alumno* alumno, NodoMateria* listaMaterias) {
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

void liberarAVL(NodoAVL* nodo) {
    if (!nodo) return;
    liberarAVL(nodo->izq);
    liberarAVL(nodo->der);
    free(nodo);
}