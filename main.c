#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "datast/avl.h"


// typedef struct Alumno {
//     int id;
//     char nombre[100];
//     int edad;
//     //int materiasInscripto[1000];
//     //int cantidadDeMaterias;
// }Alumno;

Alumno* crearAlumno(int id, char * nombre, int edad) {
    Alumno* al = malloc(sizeof(Alumno));
    al->id = id;
    strcpy(al->nombre, nombre);
    al->edad = edad;

    return al;
}

void eliminarAlumnoPorId(int id) {

}

const int capacidad_inicial = 100;


int main() {
    Alumno *a1, *a2, *a3, *a4;

    a1 = crearAlumno(1, "Jorge Fonte", 31);
    a2 = crearAlumno(2, "Gaston Ramsar", 25);
    a3 = crearAlumno(3, "Valeria Mongrel", 22);
    a4 = crearAlumno(4, "Simferopol Naval Base", 19);

    Alumno* alumnos[] = {a1, a2, a3, a4};

    for (int i = 0; i < 4; i++) {
        printf("<<%i# %s %i>>\n",alumnos[i]->id, alumnos[i]->nombre, alumnos[i]->edad);
    }
    //Alumno* lista = malloc(sizeof(Alumno) * capacidad_inicial);
    //printf(" %zu",sizeof(alu));
}
