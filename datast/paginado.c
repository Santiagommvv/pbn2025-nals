#include <stdio.h>
#include <stdlib.h>
#include "paginado.h"
#include "../dominio/avl.h"
#include "../include/config.h"

// Funcion principal

void listarAlumnosPaginadoAVL(NodoAVL* raiz) {
    if(!raiz) {
        printf("No hay alumnos para mostrar");
        return;
    }

    int total = contarAlumnos(raiz);

    // Reservar memoria para el array
    Alumno* alumnos = malloc(sizeof(Alumno) * total);
    if(!alumnos){
        printf("Error de memoria\n");
        return;
    }

    int index = 0;
    recorrerInOrden(raiz, alumnos, &index);

    int pagina = 0;
    char opcion;
    int paginasTotales = (total + PAGINADO_TAM - 1) / PAGINADO_TAM;

    do {
        printf("\n--- Alumnos (Pagina %d de %d) ---\n", pagina + 1, paginasTotales);

        int inicio = pagina * PAGINADO_TAM;
        int fin = inicio + PAGINADO_TAM;
        if(fin>total) fin = total;

        for(int i = inicio; i< fin; i++){
            Alumno a = alumnos[i];
            printf("ID: %d | Nombre: %s | Edad: %d | Materias inscriptas: %d\n",
                a.id, a.nombre, a.edad, a.cantidadDeMateriasInscripto);
        }
        printf("\n[N] Pagina siguiente | [P] Pagina anterior | [S] Salir\n");
        printf("Elija una opcion: ");
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
            printf("Opcion invalida.\n");
        }
    } while (opcion != 's' && opcion != 'S');

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

void listarMateriasPaginado(NodoMateria* cabeza) {
    if(!cabeza) {
        printf("No hay materias para mostrar.\n");
        return;
    }
    
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
        
        printf("\n[N] Pagina siguiente | [P] Pagina anterior | [S] Salir\n");
        printf("Elija una opcion: ");
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcion = buffer[0];
        } else {
            opcion = 's';
        }

        if((opcion == 'n' || opcion == 'N') && (pagina + 1)* PAGINADO_TAM < total){
            pagina++;
        } else if ((opcion == 'p' || opcion == 'P') && pagina > 0) {
            pagina--;
        } else if(opcion != 's' && opcion != 'S'){
            printf("Opcion invalida.\n");
        }
    } while(opcion != 's' && opcion != 'S');
}