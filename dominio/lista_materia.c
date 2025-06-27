#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_materia.h"
#include "../datast/utils.h"

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

int eliminarMateria(NodoMateria** cabeza, int id){
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
            return 1;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Materia con ID %d no encontrada\n", id);
    return 0;
}

int modificarMateria(NodoMateria* cabeza, int id){
    while(cabeza) {
        if(cabeza->datos.id == id){
            pedirString("Ingrese nuevo nombre para la materia: ", cabeza->datos.nombre, MAX_NOMBRE);
            return 1;
        }
        cabeza = cabeza->siguiente;
    }
    printf("Materia con ID %d no encontrada\n", id);
    return 0;
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
        cabeza = cabeza->siguiente;
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

void liberarListaMaterias(NodoMateria* cabeza) {
    while (cabeza) {
        NodoMateria* tmp = cabeza;
        cabeza = cabeza->siguiente;
        free(tmp);
    }
}