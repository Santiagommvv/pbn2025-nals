#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alumno.h"

static int ultimoID = 0;

Alumno crearAlumno(const char* nombre, int edad){

    Alumno nuevoAlumno;
    
    nuevoAlumno.id = ++ultimoID;
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
        printf("No hay espacio para registrar mas materias rendidas.\n");
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

int inscribirAlumnoEnMateria(Alumno* alumno, Materia* materia) {
    if (!alumno || !materia) return 0;

    // Verifica si ya está inscripto
    for(int i = 0; i < alumno->cantidadDeMateriasInscripto; i++) {
        if(alumno->materiasInscripto[i] == materia->id) {
            printf("El alumno ya está inscripto en esta materia.\n");
            return 0;
        }
    }

    if(alumno->cantidadDeMateriasInscripto >= MAX_MATERIAS_POR_ALUMNO) {
        printf("El alumno alcanzó el máximo de materias.\n");
        return 0;
    }
    if(materia->cantidadAlumnos >= MAX_ALUMNOS_POR_MATERIA) {
        printf("La materia alcanzó el cupo máximo de alumnos.\n");
        return 0;
    }

    alumno->materiasInscripto[alumno->cantidadDeMateriasInscripto++] = materia->id;
    materia->alumnosInscriptos[materia->cantidadAlumnos++] = alumno->id;
    printf("Inscripción realizada con éxito.\n");
    return 1;
}