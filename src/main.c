#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/alumno.h"
#include "../include/lista_alumno.h"
#include "../include/materia.h"
#include "../include/lista_materia.h"

void mostrarMenu(){
    printf("\n--- MENU ---\n");
    printf("1. Agregar alumno\n");
    printf("2. Listar alumnos\n");
    printf("3. Agregar materia\n");
    printf("4. Listar materias\n");
    printf("5. Salir\n");
    printf("Elija una opcion");
}

int main() {
    NodoAlumno* listaAlumnos = NULL;
    NodoMateria* listaMaterias = NULL;

    int opcion;

    do{
        mostrarMenu();
        scanf("%d", &opcion);
        getchar();

        switch(opcion) {
            case 1: {
                char nombre[100];
                int edad;
                printf("Ingrese nombre del alumno: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';

                printf("Ingrese edad: ");
                scanf("%d", &edad);
                getchar();

                Alumno nuevo = crearAlumno(nombre, edad);
                agregarAlumno(&listaAlumnos, nuevo);
                printf("Alumno agregado\n");
                break;
            }

            case 2:
            listarAlumnos(listaAlumnos);
            break;

            case 3: {
                char nombre[100];
                printf("Ingrese nombre de la materia: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';

                agregarMateria(&listaMaterias, nombre);
                printf("Materia agregada\n");
                break;
            }

            case 4:
            listarMaterias(listaMaterias);
            break;

            default:
            printf("Opcion invalida");
        }
    } while(opcion!= 5);

    return 0;
}