#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "alumno.h"

static int ultimoID = 0;    //aca hay una discusion filosofica sobre si va "= 0" o no

// Devuelve el ultimo ID usado
int obtenerUltimoID() {
    return ultimoID;
}

// Establece el ultimo ID usado al arrancar el programa
void establecerUltimoID(int id) {
    //para no pasarle un dato invalido
    if(id > ultimoID) {
        ultimoID = id;
    }
}

Alumno crearAlumno(const char* nombre, const char* apellido, int edad){
    Alumno nuevoAlumno = {0}; // Inicializar a cero todos los campos
    
    // Verificar si el nombre es NULL o vacio
    if (!nombre || nombre[0] == '\0' || !apellido || apellido[0] == '\0') {
        printf("Error: Ingrese un nombre y apellido validos\n");
        return nuevoAlumno; // Retorna un alumno con ID=0 (invalido)
    }
    
    // Validar la edad
    if (edad < 18 || edad > 99) {
        printf("Error: La edad debe estar entre 18 y 99 años\n");
        return nuevoAlumno; // Retorna un alumno con ID=0 (invalido)
    }
    
    // Verificar que el nombre y apellido solo contienen caracteres validos
    for (int i = 0; nombre[i] != '\0'; i++) {
        if (!isalpha((unsigned char)nombre[i]) && !isspace((unsigned char)nombre[i])) {
            printf("Error: El nombre debe contener solo letras y espacios\n");
            return nuevoAlumno; // Retorna un alumno con ID=0 (invalido)
        }
    }
    
    for (int i = 0; apellido[i] != '\0'; i++) {
        if (!isalpha((unsigned char)apellido[i]) && !isspace((unsigned char)apellido[i])) {
            printf("Error: El apellido debe contener solo letras y espacios\n");
            return nuevoAlumno; // Retorna un alumno con ID=0 (invalido)
        }
    }
    
    // Si paso las validaciones, ahora si incrementamos el ID
    nuevoAlumno.id = ++ultimoID;
    strncpy(nuevoAlumno.nombre, nombre, sizeof(nuevoAlumno.nombre)-1);
    nuevoAlumno.nombre[sizeof(nuevoAlumno.nombre)-1] = '\0';
    strncpy(nuevoAlumno.apellido, apellido, sizeof(nuevoAlumno.apellido)-1);
    nuevoAlumno.apellido[sizeof(nuevoAlumno.apellido)-1] = '\0';
    nuevoAlumno.edad = edad;
    nuevoAlumno.cantidadDeMateriasInscripto = 0;
    nuevoAlumno.cantidadMateriasRendidas = 0; 
    
    return nuevoAlumno;
}

int rendirMateria(Alumno* alumno, int idMateria, float nota){
    if(!alumno) {
        printf("Error: Alumno invalido\n");
        return 0;
    }
    
    // Validar ID de materia
    if(idMateria <= 0) {
        printf("Error: ID de materia invalido\n");
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
    
    // Verificar si el alumno esta inscrito en la materia
    int estaInscrito = 0;
    for(int i = 0; i < alumno->cantidadDeMateriasInscripto; i++) {
        if(alumno->materiasInscripto[i] == idMateria) {
            estaInscrito = 1;
            break;
        }
    }
    
    if(!estaInscrito) {
        printf("Error: El alumno no esta inscrito en esta materia\n");
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
        printf("Error: Alumno invalido\n");
        return 0;
    }
    
    if (!materia) {
        printf("Error: Materia invalida\n");
        return 0;
    }
    
    // Validar IDs
    if (alumno->id <= 0) {
        printf("Error: ID de alumno invalido\n");
        return 0;
    }
    
    if (materia->id <= 0) {
        printf("Error: ID de materia invalido\n");
        return 0;
    }

    // Verifica si ya esta inscripto
    for(int i = 0; i < alumno->cantidadDeMateriasInscripto; i++) {
        if(alumno->materiasInscripto[i] == materia->id) {
            printf("El alumno ya esta inscripto en esta materia.\n");
            return 0;
        }
    }

    if(alumno->cantidadDeMateriasInscripto >= MAX_MATERIAS_POR_ALUMNO) {
        printf("El alumno alcanzo el maximo de materias.\n");
        return 0;
    }
    if(materia->cantidadAlumnos >= MAX_ALUMNOS_POR_MATERIA) {
        printf("La materia alcanzo el cupo maximo de alumnos.\n");
        return 0;
    }
    
    // Verificar correlativas
    for (int i = 0; i < materia->cantidadCorrelativas; i++) {
        int idCorrelativa = materia->correlativas[i];
        int correlativaAprobada = 0;
        
        // Buscar si la correlativa esta aprobada
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
    printf("Inscripcion realizada con exito.\n");
    return 1;
}

// Funcion comun para visualizar un alumno con formato
void visualizarAlumno(Alumno a, int formatoAvanzado) {
    char nombreCompleto[101];
    snprintf(nombreCompleto, sizeof(nombreCompleto), "%s, %s", a.apellido, a.nombre);
    
    if (formatoAvanzado) {
        printf("%-5d | %-30s | %-5d | %-20d\n",
               a.id, nombreCompleto, a.edad, a.cantidadDeMateriasInscripto);
    } else {
        printf("ID: %d | Nombre: %s | Edad: %d | Materias inscriptas: %d\n",
               a.id, nombreCompleto, a.edad, a.cantidadDeMateriasInscripto);
    }
}