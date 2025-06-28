#include "../dominio/alumno.h"
#include "../dominio/lista_materia.h"

#ifndef AVL_H
#define AVL_H
typedef struct NodoAVL {
    Alumno alumno;
    int altura;
    struct NodoAVL* izq;
    struct NodoAVL* der;
} NodoAVL;

int max(int a, int b);
int altura(NodoAVL* n);
int balance(NodoAVL* n);
void imprimirInOrden(NodoAVL* raiz);
void recorrerInOrden(NodoAVL* nodo, Alumno* alumnos, int* index);
int contarAlumnos(NodoAVL* nodo);
NodoAVL* buscarAlumnoPorApellidoAVL(NodoAVL* raiz, const char* apellido);
NodoAVL* buscarAlumnoPorIDAVL(NodoAVL* raiz, int id);
void buscarAlumnosPorEdadAVL(NodoAVL* raiz, int edadMin, int edadMax, int* encontrados);
void modificarAlumnoAVL(NodoAVL* raiz, int id);
void listarMateriasAprobadas(Alumno* alumno, NodoMateria* listaMaterias);
void listarMateriasRendidas(Alumno* alumno, NodoMateria* listaMaterias);

NodoAVL* minimoNodoAVL(NodoAVL* nodo);
NodoAVL* eliminarAlumnoAVL(NodoAVL* raiz, int id);

NodoAVL* rotarDerecha(NodoAVL* y);
NodoAVL* rotarIzquierda(NodoAVL* x);
NodoAVL* insertarAVL(NodoAVL* nodo, Alumno alumno);
void liberarAVL(NodoAVL* raiz);

#endif //AVL_H
