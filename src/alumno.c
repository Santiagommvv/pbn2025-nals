#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/alumno.h"

Alumno crearAlumno(const char* nombre, int edad){

    Alumno nuevoAlumno;
    
    nuevoAlumno.id = -1;
    strncpy(nuevoAlumno.nombre, nombre, MAX_NOMBRE-1);
    nuevoAlumno.nombre[MAX_NOMBRE-1] = '\0';
    nuevoAlumno.edad = edad;
    nuevoAlumno.cantidadDeMaterias = 0;
    
    return nuevoAlumno;
}