//
// Created by zbsam145 on 24/6/2025.
//

#ifndef ALUMNO_H
#define ALUMNO_H
typedef struct Alumno {
    int id;
    char nombre[100];
    int edad;
    //int materiasInscripto[1000];
    //int cantidadDeMaterias;
}Alumno;

Alumno* crearAlumno(int id, const char *nombre, int edad) {}
void liberarAlumno(Alumno* alumno) {}

#endif //ALUMNO_H
