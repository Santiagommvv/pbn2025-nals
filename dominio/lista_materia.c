#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_materia.h"
#include "../datast/utils.h"

static int ultimoIDMateria = 0;

NodoMateria* agregarMateria(NodoMateria** cabeza, const char* nombre) {
    if (!cabeza) {
        printf("Error: Puntero a cabeza es NULL\n");
        return NULL;
    }
    
    if (!nombre || nombre[0] == '\0') {
        printf("Error: El nombre de la materia no puede ser NULL o vacio\n");
        return NULL;
    }

    NodoMateria* nodo = malloc(sizeof(NodoMateria));
    if(!nodo){
        return NULL;
    }

    nodo->datos.id = ++ultimoIDMateria;
    
    strncpy(nodo->datos.nombre,nombre,sizeof(nodo->datos.nombre));
    nodo->datos.nombre[sizeof(nodo->datos.nombre) -1] = '\0';

    nodo->datos.cantidadAlumnos = 0;
    nodo->datos.cantidadCorrelativas = 0;
    nodo->siguiente = NULL;
    
    // Agregar el nodo al final de la lista para mantener el orden de IDs ascendente
    if (*cabeza == NULL) {
        // Lista vacia, el nuevo nodo es la cabeza
        *cabeza = nodo;
    } else {
        // Encontrar el ultimo nodo
        NodoMateria* ultimo = *cabeza;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        // Agregar el nuevo nodo al final
        ultimo->siguiente = nodo;
    }

    return nodo;
}

int eliminarMateria(NodoMateria** cabeza, int id){
    if (!cabeza || !(*cabeza)) {
        printf("Error: No hay materias para eliminar\n");
        return 0;
    }
    
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
    if (!cabeza) {
        printf("Error: No hay materias para modificar\n");
        return 0;
    }
    
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
        visualizarMateria(cabeza->datos, 0); // Usar la funcion comun
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
    if (!nombre) {
        printf("Error: nombre no puede ser NULL\n");
        return NULL;
    }
    
    while(cabeza){
        if(strcasecmp(cabeza->datos.nombre, nombre) == 0){
            return cabeza;
        }
        cabeza = cabeza->siguiente;
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

int contarNodosMaterias(NodoMateria* cabeza) {
    int count = 0;
    while (cabeza) {
        count++;
        cabeza = cabeza->siguiente;
    }
    return count;
}

// Funcion comun para visualizar una materia con formato
void visualizarMateria(Materia m, int formatoAvanzado) {
    if (formatoAvanzado) {
        printf("%-5d | %-30s | %-20d\n",
               m.id, m.nombre, m.cantidadAlumnos);
    } else {
        printf("ID: %d | Nombre: %s | Cantidad de Alumnos: %d\n",
               m.id, m.nombre, m.cantidadAlumnos);
    }
}