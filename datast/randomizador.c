#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "randomizador.h"

// Nombres y apellidos base
const char* nombres[] = {"Juan", "Ana", "Carlos", "Laura", "Mauro", "Lucia", "Pedro", "Martina"};
const char* apellidos[] = {"Perez", "Gonzalez", "Martinez", "Gomez", "Rodriguez", "Fernandez", "Sanchez", "Rosamilia"};

const char* materias[] = {"Programacion de bajo nivel", "AyP1", "AyP2", "Algebra", "Analisis I", "Analisis II"};

Alumno crearAlumnoAleatorio() {
    Alumno a = crearAlumno("", 18+rand() % 15); // Edad entre 18 y 32
        snprintf(a.nombre, sizeof(a.nombre), "%s %s",
        nombres[rand() % (sizeof(nombres)/sizeof(char*))], 
        apellidos[rand() % (sizeof(apellidos)/sizeof(char*))]);
    return a;
}

Materia crearMateriaAleatoria(){
    Materia m;
    memset(&m, 0, sizeof(Materia));
    strncpy(m.nombre, materias[rand() % (sizeof(materias)/sizeof(char*))],
        sizeof(m.nombre) - 1);
    return m;
}

void generarAlumnosAleatorios(NodoAVL** raiz, int cantidad){
    if (!raiz) {
        printf("Error: Puntero a raíz es NULL\n");
        return;
    }
    
    srand(time(NULL));
    for(int i=0; i<cantidad;i++){
        Alumno a = crearAlumnoAleatorio();
        *raiz = insertarAVL(*raiz, a);
    }
    printf("%d alumnos aleatorios generados.\n", cantidad);
}

void generarMateriasAleatorias(NodoMateria** lista, int cantidad) {
    if (!lista) {
        printf("Error: Puntero a lista es NULL\n");
        return;
    }
    
    srand(time(NULL));
    for(int i=0;i<cantidad;i++){
        Materia m= crearMateriaAleatoria();
        NodoMateria* resultado = agregarMateria(lista, m.nombre);
        if (!resultado) {
            printf("Error: No se pudo agregar la materia %s\n", m.nombre);
        }
    }
    printf("%d materias aleatorias generadas.\n", cantidad);
}