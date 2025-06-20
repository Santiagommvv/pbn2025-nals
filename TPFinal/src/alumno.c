#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/alumno.h"

static int ultimoID = 0;

Alumno* crearAlumno(const char* nombre, int edad){

    Alumno* nuevoAlumno = malloc(sizeof(Alumno));
    if(nuevoAlumno != NULL){
        nuevoAlumno->id = ++ultimoID;
        strncpy(nuevoAlumno->nombre, nombre, MAX_NOMBRE-1);
        nuevoAlumno->nombre[MAX_NOMBRE-1] = '\0';
        nuevoAlumno->edad = edad;
        nuevoAlumno->cantidadDeMaterias = 0;
        nuevoAlumno->siguiente = NULL;
        return nuevoAlumno;
    } else {
        printf("No hay suficiente espacio para crear un alumno\n");
        return NULL;
    }
}

Alumno* buscarAlumnoPorNombre(Alumno* cabeza, char* nombre){

    while(cabeza != NULL){
        if(strcmp(cabeza->nombre, nombre) == 0){
            return cabeza;
        }
        cabeza = cabeza->siguiente;
    }
    return NULL;
}

Alumno* buscarAlumnoPorId(Alumno* cabeza, int id){

    while(cabeza != NULL){
        if(cabeza->id == id){
            return cabeza;
        }
        cabeza = cabeza->siguiente;
    }
    return NULL;
}

/*
void eliminarAlumno(Alumno** cabeza, int id);
void modificarAlumno(Alumno* cabeza, int id);
*/