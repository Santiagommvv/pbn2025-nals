#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "alumno.h"
#include "../datast/utils.h"

// Declaracion externa de la funcion eliminaAlumnoDeMateria de materia.c
extern int eliminarAlumnoDeMateria(Materia* materia, int idAlumno);

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
    
    // Validar la edad usando la función edadValida
    if (!edadValida(edad)) {
        printf("Error: La edad debe estar entre %d y %d años\n", EDAD_MINIMA, EDAD_MAXIMA);
        return nuevoAlumno; // Retorna un alumno con ID=0 (invalido)
    }
    
    // Verificar que el nombre y apellido solo contienen caracteres validos
    if (!esSoloLetrasYEspacios(nombre)) {
        printf("Error: El nombre debe contener solo letras y espacios\n");
        return nuevoAlumno; // Retorna un alumno con ID=0 (invalido)
    }
    
    if (!esSoloLetrasYEspacios(apellido)) {
        printf("Error: El apellido debe contener solo letras y espacios\n");
        return nuevoAlumno; // Retorna un alumno con ID=0 (invalido)
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

int rendirMateria(Alumno* alumno, Materia* materia, float nota){
    if(!alumno) {
        printf("Error: Alumno invalido\n");
        return 0;
    }
    
    if(!materia) {
        printf("Error: Materia invalida\n");
        return 0;
    }
    
    // Validar ID de materia
    if(materia->id <= 0) {
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
        if(alumno->materiasRendidas[i].IDMateria == materia->id){
            printf("La materia ya fue rendida\n");
            return 0;
        }
    }
    
    // Verificar si el alumno esta inscrito en la materia
    int estaInscrito = 0;
    for(int i = 0; i < alumno->cantidadDeMateriasInscripto; i++) {
        if(alumno->materiasInscripto[i] == materia->id) {
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
    nueva.IDMateria = materia->id;
    nueva.nota = nota;
    nueva.aprobo = (nota >= 4.0);

    alumno->materiasRendidas[alumno->cantidadMateriasRendidas++] = nueva;
    
    // Eliminar la inscripcion de la materia que acaba de rendir
    eliminarMateriaDeAlumno(alumno, materia->id);
    
    // Eliminar al alumno de la lista de alumnos de la materia
    eliminarAlumnoDeMateria(materia, alumno->id);

    printf("Materias ID %d rendida con nota %.2f (%s)\n", materia->id, nota, nueva.aprobo ? "Aprobado": "Desaprobado");

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
    
    // Verificar si la materia ya ha sido rendida
    for (int i = 0; i < alumno->cantidadMateriasRendidas; i++) {
        if (alumno->materiasRendidas[i].IDMateria == materia->id) {
            // Si la aprobo, no puede volver a inscribirse
            if (alumno->materiasRendidas[i].aprobo) {
                printf("Error: El alumno ya aprobo esta materia\n");
                return 0;
            }
            // Si la desaprobo, puede volver a inscribirse
            break;
        }
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
        
        if (!haAprobadoMateria(alumno, idCorrelativa)) {
            printf("Error: El alumno no ha aprobado la correlativa (ID: %d) requerida\n", idCorrelativa);
            return 0;
        }
    }
    
    // Verificar regla especial para materias con ID >= ID_MATERIAS_AVANZADAS
    if (materia->id >= ID_MATERIAS_AVANZADAS) {
        printf("Verificando requisito especial: para materias avanzadas (ID >= %d) se requiere aprobar todas las materias anteriores.\n", ID_MATERIAS_AVANZADAS);
        
        if (!haAprobadoTodasHastaID(alumno, ID_MATERIAS_AVANZADAS)) {
            printf("Error: Para inscribirse en materias avanzadas (ID >= %d), debe aprobar todas las materias anteriores\n", ID_MATERIAS_AVANZADAS);
            return 0;
        }
        
        printf("Requisito especial verificado: todas las materias anteriores estan aprobadas.\n");
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

int eliminarMateriaDeAlumno(Alumno* alumno, int idMateria) {
    // Verificar si alumno es NULL
    if (!alumno) {
        printf("Error: Puntero a alumno es NULL\n");
        return 0;
    }
    
    // Verificar que el ID de materia sea valido
    if (idMateria <= 0) {
        printf("Error: ID de materia invalido (%d)\n", idMateria);
        return 0;
    }
    
    // Buscar la materia en la lista de inscriptos
    int indice = -1;
    for (int i = 0; i < alumno->cantidadDeMateriasInscripto; i++) {
        if (alumno->materiasInscripto[i] == idMateria) {
            indice = i;
            break;
        }
    }
    
    // Si no se encontro la materia
    if (indice == -1) {
        return 0;
    }
    
    // Eliminar la materia moviendo todos los elementos posteriores una posicion
    for (int i = indice; i < alumno->cantidadDeMateriasInscripto - 1; i++) {
        alumno->materiasInscripto[i] = alumno->materiasInscripto[i + 1];
    }
    
    // Decrementar el contador de materias
    alumno->cantidadDeMateriasInscripto--;
    
    return 1;
}

// Funcion auxiliar para verificar si un alumno ha aprobado una materia especifica
int haAprobadoMateria(const Alumno* alumno, int idMateria) {
    if (!alumno) return 0;
    
    for (int i = 0; i < alumno->cantidadMateriasRendidas; i++) {
        if (alumno->materiasRendidas[i].IDMateria == idMateria && 
            alumno->materiasRendidas[i].aprobo) {
            return 1; // Materia aprobada
        }
    }
    
    return 0; // No aprobada
}

// Funcion para verificar si un alumno ha aprobado todas las materias hasta un ID especifico
int haAprobadoTodasHastaID(const Alumno* alumno, int idLimite) {
    if (!alumno) return 0;
    
    for (int id = 1; id < idLimite; id++) {
        if (!haAprobadoMateria(alumno, id)) {
            return 0; // Encontro una materia no aprobada
        }
    }
    
    return 1; // Todas las materias aprobadas
}