#ifndef ALUMNO_H
#define ALUMNO_H

#include "../config/config.h"
#include "materia.h"

typedef struct NodoMateria NodoMateria;

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
void visualizarAlumno(Alumno a, int formatoAvanzado);
int haAprobadoMateria(const Alumno* alumno, int idMateria);
int haAprobadoTodasHastaID(const Alumno* alumno, int idLimite);
void mostrarMateriasAprobadas(const Alumno* alumno, NodoMateria* listaMaterias);

#endif // ALUMNO_H