#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "alumno.h"

static int ultimoID = 0;    //acá hay una discusión filosófica sobre si va "= 0" o no

// Devuelve el último ID usado
int obtenerUltimoID() {
    return ultimoID;
}

// Establece el último ID usado al arrancar el programa
void establecerUltimoID(int id) {
    //para no pasarle un dato inválido
    if(id > ultimoID) {
        ultimoID = id;
    }
}

Alumno crearAlumno(const char* nombre, int edad){
    Alumno nuevoAlumno = {0}; // Inicializar a cero todos los campos
    
    // Verificar si el nombre es NULL o vacío
    if (!nombre || nombre[0] == '\0') {
        printf("Error: Ingrese un nombre válido\n");
        return nuevoAlumno; // Retorna un alumno con ID=0 (inválido)
    }
    
    // Validar la edad
    if (edad < 18 || edad > 99) {
        printf("Error: La edad debe estar entre 18 y 99 años\n");
        return nuevoAlumno; // Retorna un alumno con ID=0 (inválido)
    }
    
    // Verificar que el nombre solo contiene caracteres válidos
    for (int i = 0; nombre[i] != '\0'; i++) {
        if (!isalpha((unsigned char)nombre[i]) && !isspace((unsigned char)nombre[i])) {
            printf("Error: El nombre debe contener solo letras y espacios\n");
            return nuevoAlumno; // Retorna un alumno con ID=0 (inválido)
        }
    }
    
    // Si pasó las validaciones, ahora sí incrementamos el ID
    nuevoAlumno.id = ++ultimoID;
    strncpy(nuevoAlumno.nombre, nombre, MAX_NOMBRE-1);
    nuevoAlumno.nombre[MAX_NOMBRE-1] = '\0';
    nuevoAlumno.edad = edad;
    nuevoAlumno.cantidadDeMateriasInscripto = 0;
    nuevoAlumno.cantidadMateriasRendidas = 0; 
    
    return nuevoAlumno;
}

int rendirMateria(Alumno* alumno, int idMateria, float nota){
    if(!alumno) {
        printf("Error: Alumno inválido\n");
        return 0;
    }
    
    // Validar ID de materia
    if(idMateria <= 0) {
        printf("Error: ID de materia inválido\n");
        return 0;
    }
    
    // Validar el rango de la nota
    if(nota < NOTA_MINIMA || nota > NOTA_MAXIMA) {
        printf("Error: La nota debe estar entre %.1f y %.1f\n", NOTA_MINIMA, NOTA_MAXIMA);
        return 0;
    }
    
    // Verificar si la materia ya fue rendida
    for(int i = 0; i < alumno->cantidadMateriasRendidas; i++){
        if(alumno->materiasRendidas[i].IDMateria == idMateria){
            printf("La materia ya fue rendida\n");
            return 0;
        }
    }
    
    // Verificar si el alumno está inscrito en la materia
    int estaInscrito = 0;
    for(int i = 0; i < alumno->cantidadDeMateriasInscripto; i++) {
        if(alumno->materiasInscripto[i] == idMateria) {
            estaInscrito = 1;
            break;
        }
    }
    
    if(!estaInscrito) {
        printf("Error: El alumno no está inscrito en esta materia\n");
        return 0;
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
    if (!alumno) {
        printf("Error: Alumno inválido\n");
        return 0;
    }
    
    if (!materia) {
        printf("Error: Materia inválida\n");
        return 0;
    }
    
    // Validar IDs
    if (alumno->id <= 0) {
        printf("Error: ID de alumno inválido\n");
        return 0;
    }
    
    if (materia->id <= 0) {
        printf("Error: ID de materia inválido\n");
        return 0;
    }

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
    
    // Verificar correlativas
    for (int i = 0; i < materia->cantidadCorrelativas; i++) {
        int idCorrelativa = materia->correlativas[i];
        int correlativaAprobada = 0;
        
        // Buscar si la correlativa está aprobada
        for (int j = 0; j < alumno->cantidadMateriasRendidas; j++) {
            if (alumno->materiasRendidas[j].IDMateria == idCorrelativa && 
                alumno->materiasRendidas[j].aprobo) {
                correlativaAprobada = 1;
                break;
            }
        }
        
        if (!correlativaAprobada) {
            printf("Error: El alumno no ha aprobado la correlativa (ID: %d) requerida\n", idCorrelativa);
            return 0;
        }
    }

    alumno->materiasInscripto[alumno->cantidadDeMateriasInscripto++] = materia->id;
    materia->alumnosInscriptos[materia->cantidadAlumnos++] = alumno->id;
    printf("Inscripción realizada con éxito.\n");
    return 1;
}