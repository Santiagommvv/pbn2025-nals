#include <stdio.h>
#include <stdlib.h>
#include "paginado.h"
#include "../dominio/avl.h"
#include "../include/config.h"

// Función genérica para manejar el paginado
static void mostrarPaginado(int total, int itemsPorPagina, 
                           void (*mostrarElementos)(void*, int, int),
                           void* datos) {
    if (total <= 0) {
        printf("No hay elementos para mostrar.\n");
        return;
    }
    
    int pagina = 0;
    char opcion;
    int paginasTotales = (total + itemsPorPagina - 1) / itemsPorPagina;
    
    do {
        printf("\n--- Elementos (Página %d de %d) ---\n", pagina + 1, paginasTotales);
        
        int inicio = pagina * itemsPorPagina;
        int fin = inicio + itemsPorPagina;
        if (fin > total) fin = total;
        
        mostrarElementos(datos, inicio, fin);
        
        printf("\n[N] Página siguiente | [P] Página anterior | [S] Salir\n");
        printf("Elija una opción: ");
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcion = buffer[0];
        } else {
            opcion = 's'; // Si hay error, salir
        }
        
        if ((opcion == 'n' || opcion == 'N') && pagina + 1 < paginasTotales) {
            pagina++;
        } else if ((opcion == 'p' || opcion == 'P') && pagina > 0) {
            pagina--;
        } else if (opcion != 's' && opcion != 'S') {
            printf("Opción inválida.\n");
        }
    } while (opcion != 's' && opcion != 'S');
}

// Función para mostrar alumnos en un rango específico
static void mostrarAlumnos(void* datos, int inicio, int fin) {
    Alumno* alumnos = (Alumno*)datos;
    for (int i = inicio; i < fin; i++) {
        Alumno a = alumnos[i];
        printf("ID: %d | Nombre: %s | Edad: %d | Materias inscriptas: %d\n",
               a.id, a.nombre, a.edad, a.cantidadDeMateriasInscripto);
    }
}

void listarAlumnosPaginadoAVL(NodoAVL* raiz) {
    if (!raiz) {
        printf("No hay alumnos para mostrar.\n");
        return;
    }
    
    int total = contarAlumnos(raiz);
    
    // Reservar memoria para el array
    Alumno* alumnos = malloc(sizeof(Alumno) * total);
    if (!alumnos) {
        printf("Error de memoria\n");
        return;
    }
    
    int index = 0;
    recorrerInOrden(raiz, alumnos, &index);
    
    // Usar la función genérica de paginado
    mostrarPaginado(total, PAGINADO_TAM, mostrarAlumnos, alumnos);
    
    free(alumnos);
}

// Función para mostrar materias en un rango específico
static void mostrarMaterias(void* datos, int inicio, int fin) {
    NodoMateria** lista = (NodoMateria**)datos;
    NodoMateria* actual = *lista;
    int count = fin - inicio;
    
    // Crear un array para almacenar las materias en este rango
    NodoMateria** materiasArray = (NodoMateria**)malloc(count * sizeof(NodoMateria*));
    if (!materiasArray) {
        printf("Error de memoria al mostrar materias\n");
        return;
    }
    
    // Avanzar hasta el inicio
    int i = 0;
    while (actual && i < inicio) {
        actual = actual->siguiente;
        i++;
    }
    
    // Almacenar punteros a las materias en el rango
    int arrayIndex = 0;
    while (actual && i < fin) {
        materiasArray[arrayIndex++] = actual;
        actual = actual->siguiente;
        i++;
    }
    
    // Mostrar las materias en el orden de la lista (que ahora es por ID ascendente)
    for (int j = 0; j < arrayIndex; j++) {
        NodoMateria* materia = materiasArray[j];
        printf("ID: %d | Nombre: %s | Cantidad de Alumnos: %d\n",
               materia->datos.id, materia->datos.nombre, materia->datos.cantidadAlumnos);
    }
    
    free(materiasArray);
}

// Contar nodos materia
static int contarMaterias(NodoMateria* cabeza) {
    int count = 0;
    while (cabeza) {
        count++;
        cabeza = cabeza->siguiente;
    }
    return count;
}

void listarMateriasPaginado(NodoMateria* cabeza) {
    if (!cabeza) {
        printf("No hay materias para mostrar.\n");
        return;
    }
    
    int total = contarMaterias(cabeza);
    
    // Usar la función genérica de paginado
    mostrarPaginado(total, PAGINADO_TAM, mostrarMaterias, &cabeza);
}