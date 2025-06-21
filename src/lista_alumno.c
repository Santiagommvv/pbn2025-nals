#include <stdio.h>
#include <stdlib.h>
#include "../include/alumno.h"
#include "../include/lista_alumno.h"

static int ultimoID = 0;

// Agrega un alumno a la lista
NodoAlumno* agregarAlumno(NodoAlumno** cabeza, Alumno nuevo){
    
    NodoAlumno* nodo = malloc(sizeof(NodoAlumno));

    if (!nodo) return NULL;
    
    nuevo.id = ++ultimoID;
    nodo->datos = nuevo;
    nodo->siguiente = *cabeza;
    *cabeza = nodo;
    return nodo;
}

NodoAlumno* buscarAlumnoPorNombre(NodoAlumno* cabeza, const char* nombre){
    while(cabeza) {
        if(strcmp(cabeza->datos.nombre,nombre) == 0){
            return cabeza;
        }
        cabeza = cabeza->siguiente;
    }
    return NULL;
}

NodoAlumno* buscarAlumnoPorID(NodoAlumno* cabeza, int id){
    while(cabeza) {
        if(cabeza->datos.id == id){
            return cabeza;
        }
        cabeza = cabeza->siguiente;
    }
    return NULL;
}

void eliminarAlumno(NodoAlumno** cabeza, int id) {
    
    NodoAlumno* actual = *cabeza;
    NodoAlumno* anterior = NULL;

    while (actual) {
        if(actual->datos.id == id){
            if(anterior){
                anterior->siguiente = actual->siguiente;
            }
            else{
                *cabeza = actual->siguiente;
            }
            free(actual);
            printf("Alumno con id %d eliminado\n", id);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Alumno con ID %d no encontrado\n", id);
}

void modificarAlumno(NodoAlumno* cabeza, int id){
    NodoAlumno* alumno = buscarAlumnoPorID(cabeza, id);
    if(!alumno) {
        printf("Alumno con ID %d no encontrado\n", id);
        return;
    }

    printf("Modificando alumno con ID %d\n", id);
    printf("Ingrese nuevo nombre: ");
    scanf(" %[^\n]", alumno->datos.nombre);
    printf("Ingrese nueva edad: ");
    scanf("%d", &alumno->datos.edad);
}

void listarAlumnos(NodoAlumno* cabeza) {
    if (!cabeza) {
        printf("No hay alumnos para mostrar.\n");
        return;
    }

    printf("Listado de alumnos:\n");
    while (cabeza) {
        Alumno a = cabeza->datos;
        printf("ID: %d | Nombre: %s | Edad: %d | Materias inscriptas: %d\n",a.id, a.nombre, a.edad, a.cantidadDeMaterias);
        cabeza = cabeza->siguiente;
    }
}