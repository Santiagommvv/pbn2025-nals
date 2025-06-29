// 1: INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "materia.h"

// 2: FUNCIONES BASICAS
// crea una materia
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
// agregar materia correlativa
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

int eliminarAlumnoDeMateria(Materia* materia, int idAlumno) {
    // Verificar si materia es NULL
    if (!materia) {
        printf("Error: Puntero a materia es NULL\n");
        return 0;
    }
    
    // Verificar que el ID de alumno sea valido
    if (idAlumno <= 0) {
        printf("Error: ID de alumno invalido (%d)\n", idAlumno);
        return 0;
    }
    
    // Buscar el alumno en la lista de inscritos
    int indice = -1;
    for (int i = 0; i < materia->cantidadAlumnos; i++) {
        if (materia->alumnosInscriptos[i] == idAlumno) {
            indice = i;
            break;
        }
    }
    
    // Si no se encontro el alumno
    if (indice == -1) {
        return 0;
    }
    
    // Eliminar al alumno moviendo todos los elementos posteriores una posicion
    for (int i = indice; i < materia->cantidadAlumnos - 1; i++) {
        materia->alumnosInscriptos[i] = materia->alumnosInscriptos[i + 1];
    }
    
    // Decrementar el contador de alumnos
    materia->cantidadAlumnos--;   
    return 1;
}