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
    printf("5. Incribir alumno en materia\n");
    printf("6. Rendir materia\n");
    printf("7. Listar materias rendidas por un alumno\n");
    printf("8. Salir\n");
    printf("Elija una opcion\n");
}

int main() {
    NodoAlumno* listaAlumnos = NULL;
    NodoMateria* listaMaterias = NULL;

    int opcion;
    int IDAlumno, IDMateria;

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

            case 5:

            listarAlumnos(listaAlumnos);
            printf("Ingrese ID del alumno a inscribir: ");
            scanf("%d", &IDAlumno);
            getchar();

            NodoAlumno* alumno = buscarAlumnoPorID(listaAlumnos, IDAlumno);
            if(!alumno) {
                printf("Alumno no encontrado.\n");
                break;
            }

            listarMaterias(listaMaterias);
            printf("Ingrese ID de la materia: ");
            scanf("%d", &IDMateria);
            getchar();

            NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
            if(!materia) {
                printf("Materia no encontrada.\n");
                break;
            }

            int yaInscripto = 0;
            for(int i = 0; i<alumno->datos.cantidadDeMateriasInscripto; i++){
                if(alumno->datos.materiasInscripto[i] == IDMateria) {
                    yaInscripto = 1;
                    break;
                }
            }

            if(yaInscripto) {
                printf("El alumno ya esta inscripto en esta materia.\n");
                break;
            }

            if(alumno->datos.cantidadDeMateriasInscripto < MAX_MATERIAS_POR_ALUMNO && materia->datos.cantidadAlumnos < MAX_ALUMNOS_POR_MATERIA){
                alumno->datos.materiasInscripto[alumno->datos.cantidadDeMateriasInscripto++] = IDMateria;

                materia->datos.alumnosInscriptos[materia->datos.cantidadAlumnos++] = IDAlumno;

                printf("Inscripcion realizada con exito.\n");
            } else{
                printf("No se puede inscribir. Limite de materias alcanzado o cupo de materia lleno.\n");
            }
            break;

            case 6: {
                listarAlumnos(listaAlumnos);
                printf("Ingrese ID del alumno: ");
                scanf("%d", &IDAlumno);
                getchar();

                NodoAlumno* alumno = buscarAlumnoPorID(listaAlumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado");
                    break;
                }

                printf("Materias inscriptas: \n");
                for(int i = 0; i < alumno->datos.cantidadDeMateriasInscripto; i++){
                    int IDMat = alumno->datos.materiasInscripto[i];
                    NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMat);
                    if(materia){
                        printf("ID: %d | Nombre: %s\n", materia->datos.id, materia->datos.nombre);
                    }
                }

                printf("Ingrese ID de la materia a rendir: ");
                scanf("%d", &IDMateria);
                getchar();

                printf("Ingrese nota obtenida: ");
                float nota;
                scanf("%f", &nota);
                getchar();

                rendirMateria(&alumno->datos, IDMateria, nota);
                break;
            }

            case 7: {
                listarAlumnos(listaAlumnos);
                printf("Ingrese ID del alumno: ");
                scanf("%d", &IDAlumno);
                getchar();

                NodoAlumno* alumno = buscarAlumnoPorID(listaAlumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado\n");
                    break;
                }

                listarMateriasRendidas(&alumno->datos, listaMaterias);
                break;
            }


            default:
            printf("Opción inválida\n");
        }
    } while(opcion!= 8);

    return 0;
}