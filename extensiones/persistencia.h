#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "../dominio/avl.h"
#include "../dominio/lista_materia.h"

//me fijo si hay datos persistidos de una sesion anterior
int datosGuardadosDisponibles();

// Carga listas al iniciar
void cargarDatos(NodoAVL** alumnos, NodoMateria** materias);

// Guarda listas antes de salir
void guardarDatos(NodoAVL* alumnos, NodoMateria* materias);

// Guarda solo el plan de estudios
void guardarPlanEstudiosCSV(NodoMateria* materias);

// Carga solo el plan de estudios
void cargarPlanEstudiosCSV(NodoMateria** lista);

#endif