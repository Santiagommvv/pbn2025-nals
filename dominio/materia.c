#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "materia.h"

Materia crearMateria(const char* nombre){
    Materia materia = {0}; // Inicializar todos los campos a 0
    materia.id = -1;
    
    // Verificar si el nombre es NULL o vacio
    if (!nombre || nombre[0] == '\0') {
        printf("Error: El nombre de la materia no puede ser NULL o vacio\n");
        return materia;
    }
    
    strncpy(materia.nombre, nombre, sizeof(materia.nombre)-1);
    materia.nombre[sizeof(materia.nombre)-1]='\0';

    materia.cantidadAlumnos = 0;
    for(int i = 0; i<MAX_ALUMNOS_POR_MATERIA; i++){
        materia.alumnosInscriptos[i] = -1;
    }

    materia.cantidadCorrelativas = 0;
    for(int i = 0; i<MAX_CORRELATIVAS;i++){
        materia.correlativas[i] = -1;
    }
    return materia;
}

int agregarCorrelativa(Materia* materia, int IDcorrelativa){
    // Verificar si materia es NULL
    if (!materia) {
        printf("Error: Puntero a materia es NULL\n");
        return 0;
    }
    
    // Verificar que el ID de correlativa sea valido
    if (IDcorrelativa <= 0) {
        printf("Error: ID de correlativa invalido (%d)\n", IDcorrelativa);
        return 0;
    }
    
    if(materia->cantidadCorrelativas >= MAX_CORRELATIVAS){
        return 0;
    }
    materia->correlativas[materia->cantidadCorrelativas++] = IDcorrelativa;
    return 1;
}