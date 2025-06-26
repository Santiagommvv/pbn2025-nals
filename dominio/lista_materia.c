#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_materia.h"

static int ultimoIDMateria = 0;

NodoMateria* agregarMateria(NodoMateria** cabeza, const char* nombre) {

    NodoMateria* nodo = malloc(sizeof(NodoMateria));
    if(!nodo){
        return NULL;
    }

    nodo->datos.id = ++ultimoIDMateria;
    
    strncpy(nodo->datos.nombre,nombre,sizeof(nodo->datos.nombre));
    nodo->datos.nombre[sizeof(nodo->datos.nombre) -1] = '\0';

    nodo->datos.cantidadAlumnos = 0;
    nodo->datos.cantidadCorrelativas = 0;
    nodo->siguiente = *cabeza;
    *cabeza = nodo;

    return nodo;
}

void eliminarMateria(NodoMateria** cabeza, int id){
    NodoMateria* actual = *cabeza;
    NodoMateria* anterior = NULL;

    while(actual) {
        if(actual->datos.id == id) {
            if(anterior){
                anterior->siguiente = actual->siguiente;
            } else{
                *cabeza = actual->siguiente;
            }
            free(actual);
            printf("Materia con ID %d eliminada.\n", id);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Materia con ID %d no encontrada\n", id);
}

void modificarMateria(NodoMateria* cabeza, int id){
    while(cabeza) {
        if(cabeza->datos.id == id){
            printf("Ingrese nuevo nombre para la materia: ");
            scanf(" %[^\n]", cabeza->datos.nombre);
            return;
        }
        cabeza = cabeza->siguiente;
    }
    printf("Materia con ID %d no encontrada\n", id);
}

void listarMaterias(NodoMateria* cabeza) {
    if(!cabeza) {
        printf("No hay materias para mostrar");
        return;
    }

    printf("Listado de materias:\n");
    while(cabeza){
        Materia m = cabeza->datos;
        printf("ID: %d | Nombre: %s | Inscriptos: %d\n", m.id, m.nombre,m.cantidadAlumnos);
        cabeza = cabeza->siguiente;
    }
}

NodoMateria* buscarMateriaPorID(NodoMateria* cabeza, int id){
    while(cabeza) {
        if(cabeza->datos.id == id){
            return cabeza;
        }
    }
    return NULL;
}

NodoMateria* buscarMateriaPorNombre(NodoMateria* cabeza, const char* nombre){
    while(cabeza){
        if(strcmp(cabeza->datos.nombre, nombre) == 0){
            return cabeza;
        }
    }
    return NULL;
}