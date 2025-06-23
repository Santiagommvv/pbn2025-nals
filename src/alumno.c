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
    nuevoAlumno.cantidadDeMateriasInscripto = 0;
    
    return nuevoAlumno;
}

int rendirMateria(Alumno* alumno, int idMateria, float nota){
    if(!alumno) return 0;

    for(int i = 0; i<alumno->cantidadMateriasRendidas;i++){
        if(alumno->materiasRendidas[i].IDMateria == idMateria){
            printf("La materia ya fue rendida\n");
            return 0;
        }
    }

    if(alumno->cantidadMateriasRendidas >= MAX_MATERIAS_POR_ALUMNO){
        printf("No hay espacio para registrar más materias rendidas.\n");
        return 0;
    }

    MateriaRendida nueva;
    nueva.IDMateria = idMateria;
    nueva.nota = nota;
    nueva.aprobo = (nota >= 4.0);

    alumno->materiasRendidas[alumno->cantidadMateriasRendidas++] = nueva;

    printf("Materias ID %d rendida con nota %.2f (%s)\n", idMateria, nota, nueva.aprobo ? "Aprobado": "Desaprobado");

    return 1;
}