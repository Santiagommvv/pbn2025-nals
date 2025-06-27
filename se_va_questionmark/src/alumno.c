/*
#include <stdlib.h>
#include <string.h>
#include "../include/alumno.h"

Alumno crearAlumno(int id, const char *nombre, int edad) {
    printf("creando alumno");
    Alumno* nuevo = malloc(sizeof(Alumno));
    if (!nuevo) return NULL;
    nuevo->id = id;
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->nombre[sizeof(nuevo->nombre) - 1] = '\0'; // asegurá terminación
    nuevo->edad = edad;
    return nuevo;
}

// Función para liberar un Alumno si fuera necesario
void liberarAlumno(Alumno* alumno) {
    free(alumno);
}
*/