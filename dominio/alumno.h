#ifndef ALUMNO_H
#define ALUMNO_H

#include "../include/config.h"
#include "materia.h"

typedef struct MateriaRendida{
    int IDMateria;
    float nota;
    int aprobo;
} MateriaRendida;

typedef struct Alumno{
    int id;
    char nombre[50];
    char apellido[50];
    int edad;

    int materiasInscripto[MAX_MATERIAS_POR_ALUMNO];
    int cantidadDeMateriasInscripto;

    MateriaRendida materiasRendidas[MAX_MATERIAS_POR_ALUMNO];
    int cantidadMateriasRendidas;
} Alumno;

int obtenerUltimoID(void);
void establecerUltimoID(int id);
Alumno crearAlumno(const char* nombre, const char* apellido, int edad);
int rendirMateria(Alumno* alumno, Materia* materia, float nota);
int inscribirAlumnoEnMateria(Alumno* alumno, Materia* materia);
int eliminarMateriaDeAlumno(Alumno* alumno, int idMateria);
// Función común para visualizar un alumno con formato
void visualizarAlumno(Alumno a, int formatoAvanzado);
// Funciones auxiliares para verificar correlatividades
int haAprobadoMateria(const Alumno* alumno, int idMateria);
int haAprobadoTodasHastaID(const Alumno* alumno, int idLimite);

#endif // ALUMNO_H