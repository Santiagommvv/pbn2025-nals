#include <stdio.h>
#include "../include/paginado.h"
#include "../include/config.h"

// Contar nodos alumno
static int contarAlumnos(NodoAlumno* cabeza){
    int count = 0;
    while(cabeza) {
        count++;
        cabeza = cabeza->siguiente;
    }
    return count;
}

// Contar nodos materia
static int contarMaterias(NodoMateria* cabeza){
    int count = 0;
    while(cabeza) {
        count++;
        cabeza = cabeza->siguiente;
    }
    return count;
}

void listarAlumnosPaginado(NodoAlumno* cabeza){
    int total = contarAlumnos(cabeza);
    if(total == 0){
        printf("No hay alumnos para mostrar.\n");
        return;
    }

    int pagina = 0;
    char opcion;

    do {
        printf("\n--- Alumnos (Pagina %d de %d) ----\n", pagina+1, (total = PAGINADO_TAM - 1) / PAGINADO_TAM);

        NodoAlumno* actual = cabeza;
        // Avanzo al nodo desde donde empezar la pagina
        int skip = pagina * PAGINADO_TAM;
        while(skip > 0 && actual) {
            actual = actual->siguiente;
            skip--;
        }

        // Imprime hasta PAGINADO_TAM elementos o hasta que se acabe la lista
        int count = 0;
        while(actual && count<PAGINADO_TAM){
            Alumno a = actual->datos;
            printf("ID: %d | Nombre: %s | Edad: %d | Materias inscriptas: %d\n",
            a.id, a.nombre, a.edad, a.cantidadDeMateriasInscripto);
            actual = actual->siguiente;
            count++;
        }

        printf("\n[N] Pagina siguiente | [P] Pagina anterior | [S] Salir\n");
        printf("Elija una opcion: ");
        opcion = getchar();
        while(getchar() != '\n'); // limpiar buffer

        if ((opcion == 'n' || opcion == 'N') && (pagina + 1)* PAGINADO_TAM <total) {
            pagina++;
        } else if ((opcion == 'p' || opcion == 'P') && pagina > 0){
            pagina--;
        } else if (opcion != 's' || opcion == 'S') {
            printf("Opcion invalida.\n");
        }
    } while(opcion != 's' && opcion != 'S');
}

void listarMateriasPaginado(NodoMateria* cabeza) {
    int total = contarMaterias(cabeza);
    if(total == 0) {
        printf("No hay materias para mostrar.\n");
        return;
    }

    int pagina  = 0;
    char opcion;

    do {
        printf("\n--- Materias (Pagina %d de %d) ---\n", pagina+1, (total + PAGINADO_TAM - 1)/ PAGINADO_TAM);

        NodoMateria* actual = cabeza;
        int skip = pagina * PAGINADO_TAM;
        while(skip>0 && actual) {
            actual = actual ->siguiente;
            skip--;
        }

        int count = 0;
        while(actual && count < PAGINADO_TAM) {
            printf("ID: %d | Nombre: %s | Cantidad de Alumnos: %d\n",
                actual->datos.id, actual->datos.nombre, actual->datos.cantidadAlumnos);
            actual = actual->siguiente;
            count++;
        }
        
        printf("\n[N] Página siguiente | [P] Página anterior | [S] Salir\n");
        printf("Elija una opcion: ");
        opcion = getchar();
        while(getchar() != 'n'); // limpiar buffer

        if((opcion == 'n' || opcion == 'N') && (pagina + 1)* PAGINADO_TAM < total){
            pagina++;
        } else if ((opcion == 'p' || opcion == 'P') && pagina > 0) {
            pagina--;
        } else if(opcion != 's' && opcion != 'S'){
            printf("Opcion invalida.\n");
        }
    } while(opcion != 's' && opcion != 'S');
}