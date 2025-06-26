#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dominio/alumno.h"
#include "dominio/avl.h"
#include "dominio/materia.h"
#include "dominio/lista_materia.h"
#include "datast/persistencia.h"
#include "datast/paginado.h"
#include "datast/randomizador.h"

void mostrarMenu(){
    printf("\n--- MENU ---\n");
    printf("1. Agregar alumno\n");
    printf("2. Listar alumnos\n");
    printf("3. Buscar alumno por nombre\n");
    printf("4. Buscar alumno por rango de edad\n");
    printf("5. Modificar alumno\n");
    printf("6. Eliminar alumno\n");
    printf("7. Agregar materia\n");
    printf("8. Listar materias\n");
    printf("9. Modificar materia\n");
    printf("10. Eliminar materia\n");
    printf("11. Incribir alumno en materia\n");
    printf("12. Rendir materia\n");
    printf("13. Listar materias rendidas por un alumno\n");
    printf("14. Generar alumnos aleatorios\n");
    printf("15. Generar materias aleatorias\n");
    printf("16. Salir\n");
    printf("Elija una opcion\n");
}

int main() {
    NodoAVL* alumnos = NULL;
    NodoMateria* listaMaterias = NULL;

    cargarDatos(&alumnos, &listaMaterias);

    int opcion;
    int IDAlumno, IDMateria;
    char nombre[100];
    int edad;

    do{
        mostrarMenu();
        scanf("%d", &opcion);
        getchar();

        switch(opcion) {
            // Agregar alumno
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
                alumnos = insertarAVL(alumnos, nuevo);
                printf("Alumno agregado\n");
                break;
            }

            // Listar alumnos
            case 2:
            listarAlumnosPaginadoAVL(alumnos);
            break;

            //Buscar alumno por nombre
            case 3: {
                char nombre[100];
                printf("Ingrese nombre para buscar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';

                NodoAVL* alumno = buscarAlumnoPorNombreAVL(alumnos, nombre);
                if(alumno) {
                    printf("Alumno encontrado: ID: %d | Nombre: %s | Edad: %d\n",
                            alumno->alumno.id, alumno->alumno.nombre, alumno->alumno.edad);
                } else {
                printf("Alumno no encontrado");
                }
                break;
            }

            // Buscar alumno por rango de edad
            case 4: {
                int edadMin, edadMax;
                printf("Ingrese edad minima: ");
                scanf("%d", &edadMin);
                getchar();
                printf("Ingrese edad maxima");
                scanf("%d", &edadMax);
                getchar();

                int encontrados = 0;
                buscarAlumnosPorEdadAVL(alumnos, edadMin, edadMax, &encontrados);

                if(encontrados == 0){
                    printf("No se encontraron alumnos entre %d y %d años.\n", edadMin,edadMax);
                }

                break;
            }

            //Modificar alumno
            case 5: {
                printf("Ingrese ID del alumno a modificar: ");
                scanf("%d", &IDAlumno);
                getchar();

                modificarAlumnoAVL(alumnos, IDAlumno);
                break;
            }

            //Eliminar alumno
            case 6: {
                printf("Ingrese ID del alumno a eliminar: ");
                scanf("%d", &IDAlumno);
                getchar();

                alumnos = eliminarAlumnoAVL(alumnos, IDAlumno);
                break;
            }

            // Agregar materia
            case 7: {
                char nombre[100];
                printf("Ingrese nombre de la materia: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';

                agregarMateria(&listaMaterias, nombre);
                printf("Materia agregada\n");
                break;
            }
            
            // Listar materia
            case 8:
            listarMateriasPaginado(listaMaterias);
            break;
            
            // Modificar materia
            case 9: {
            printf("Ingrese ID de la materia a modificar: ");
            scanf("%d", &IDMateria);
            getchar();

            modificarMateria(listaMaterias, IDMateria);
            break;
            }

            // Eliminar materia
            case 10: {
                printf("Ingrese el ID de la materia a eliminar: ");
                scanf("%d", &IDMateria);
                getchar();

                eliminarMateria(&listaMaterias, IDMateria);
                break;
            }

            // Inscribir alumno en materia
            case 11: {

                listarAlumnosPaginadoAVL(alumnos);
                printf("Ingrese ID del alumno a inscribir: ");
                scanf("%d", &IDAlumno);
                getchar();

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno) {
                    printf("Alumno no encontrado.\n");
                    break;
                }

                listarMateriasPaginado(listaMaterias);
                printf("Ingrese ID de la materia: ");
                scanf("%d", &IDMateria);
                getchar();

                NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
                if(!materia) {
                    printf("Materia no encontrada.\n");
                    break;
                }

                int yaInscripto = 0;
                for(int i = 0; i<alumno->alumno.cantidadDeMateriasInscripto; i++){
                    if(alumno->alumno.materiasInscripto[i] == IDMateria) {
                        yaInscripto = 1;
                        break;
                    }
                }

                if(yaInscripto) {
                    printf("El alumno ya esta inscripto en esta materia.\n");
                    break;
                }

                if(alumno->alumno.cantidadDeMateriasInscripto < MAX_MATERIAS_POR_ALUMNO && materia->datos.cantidadAlumnos < MAX_ALUMNOS_POR_MATERIA){
                    alumno->alumno.materiasInscripto[alumno->alumno.cantidadDeMateriasInscripto++] = IDMateria;
                    materia->datos.alumnosInscriptos[materia->datos.cantidadAlumnos++] = IDAlumno;
                    printf("Inscripcion realizada con exito.\n");
                } else{
                    printf("No se puede inscribir. Limite de materias alcanzado o cupo de materia lleno.\n");
                }
                break;
            }
            // Rendir materia
            case 12: {
                listarAlumnosPaginadoAVL(alumnos);
                printf("Ingrese ID del alumno: ");
                scanf("%d", &IDAlumno);
                getchar();

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado");
                    break;
                }

                printf("Materias inscriptas: \n");
                for(int i = 0; i < alumno->alumno.cantidadDeMateriasInscripto; i++){
                    int IDMat = alumno->alumno.materiasInscripto[i];
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

                rendirMateria(&alumno->alumno, IDMateria, nota);
                break;
            }

            // Listar materias rendidas por un alumno
            case 13: {
                listarAlumnosPaginadoAVL(alumnos);
                printf("Ingrese ID del alumno: ");
                scanf("%d", &IDAlumno);
                getchar();

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado\n");
                    break;
                }

                listarMateriasRendidas(&alumno->alumno, listaMaterias);
                break;
            }

            case 14: {
                int n;
                printf("Cuantos alumnos desea generar? ");
                scanf("%d", &n);
                getchar();
                generarAlumnosAleatorios(&alumnos, n);
                break;
            }

            case 15: {
                int n;
                printf("Cuantas materias desea generar? ");
                scanf("%d", &n);
                getchar();
                generarMateriasAleatorias(&listaMaterias, n);
                break;
            }

            case 16: {
                printf("Saliendo...\n");
                break;
            }


            default:
            printf("Opcion invalida\n");
        }
    } while(opcion!= 16);

    guardarDatos(alumnos,listaMaterias);
    return 0;
}