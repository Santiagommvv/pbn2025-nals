#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/materia.h"

Materia crearMateria(const char* nombre){
    Materia materia;
    materia.id = -1;
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
    if(materia->cantidadCorrelativas >= MAX_CORRELATIVAS){
        return 0;
    }
    materia->correlativas[materia->cantidadCorrelativas++] = IDcorrelativa;
    return 1;
}