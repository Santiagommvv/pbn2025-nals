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
#include "datast/utils.h"


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
    char nombre[100];
    int edad;

    do{
        mostrarMenu();
        opcion = pedirInt("Ingrese una opcion: ");

        switch(opcion) {
            // Agregar alumno
            case 1: {
                char nombre[100];
                int edad;

                pedirString("Ingrese nombre del alumno: ", nombre, MAX_NOMBRE);
                edad = pedirInt("Ingrese edad: ");

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
                printf("Alumno no encontrado\n");
                }
                break;
            }

            // Buscar alumno por rango de edad
            case 4: {
                int edadMin, edadMax;
                
                edadMin = pedirInt("Ingrese edad minima: ");
                edadMax = pedirInt("Ingrese edad maxima: ");

                if (edadMax < edadMin) {
                    printf("La edad maxima no puede ser menor que la minima.\n");
                    break;
                }

                int encontrados = 0;
                buscarAlumnosPorEdadAVL(alumnos, edadMin, edadMax, &encontrados);

                if(encontrados == 0){
                    printf("No se encontraron alumnos entre %d y %d años.\n", edadMin,edadMax);
                }

                break;
            }

            //Modificar alumno
            case 5: {
                int IDAlumno = pedirInt("Ingrese ID del alumno a modificar: ");
                modificarAlumnoAVL(alumnos, IDAlumno);
                break;
            }

            //Eliminar alumno
            case 6: {
                int IDAlumno = pedirInt("Ingrese ID del alumno a eliminar: ");
                alumnos = eliminarAlumnoAVL(alumnos, IDAlumno);
                break;
            }

            // Agregar materia
            case 7: {
                char nombre[MAX_NOMBRE];
                pedirString("Ingrese nombre de la materia: ", nombre, MAX_NOMBRE);
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
                int IDMateria = pedirInt("Ingrese ID de la materia a modificar: ");
                if(!modificarMateria(listaMaterias, IDMateria)) {
                    printf("No se pudo modificar la materia.\n");
                }
                break;
            }

            // Eliminar materia
            case 10: {
                int IDMateria = pedirInt("Ingrese el ID de la materia a eliminar: ");
                if (!eliminarMateria(&listaMaterias, IDMateria)) {
                    printf("No se pudo eliminar la materia.\n");
                }
                break;
            }

            // Inscribir alumno en materia
            case 11: {
                listarAlumnosPaginadoAVL(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno a inscribir: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno) {
                    printf("Alumno no encontrado.\n");
                    break;
                }

                listarMateriasPaginado(listaMaterias);
                int IDMateria = pedirInt("Ingrese ID de la materia: ");

                NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
                if(!materia) {
                    printf("Materia no encontrada.\n");
                    break;
                }
                if(!inscribirAlumnoEnMateria(&alumno->alumno, &materia->datos)) {
                    printf("No se pudo inscribir al alumno.\n");
                }
                break;
            }

            // Rendir materia
            case 12: {
                listarAlumnosPaginadoAVL(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado\n");
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

                int IDMateria = pedirInt("Ingrese ID de la materia a rendir: ");
                float nota = pedirFloat("Ingrese nota obtenida: ");

                rendirMateria(&alumno->alumno, IDMateria, nota);
                break;
            }

            // Listar materias rendidas por un alumno
            case 13: {
                listarAlumnosPaginadoAVL(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado\n");
                    break;
                }

                listarMateriasRendidas(&alumno->alumno, listaMaterias);
                break;
            }
            
            case 14: {
                int n = pedirInt("Cuantos alumnos desea generar? ");
                generarAlumnosAleatorios(&alumnos, n);
                break;
            }
            case 15: {
                int n = pedirInt("Cuantas materias desea generar? ");
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
    liberarAVL(alumnos);
    liberarListaMaterias(listaMaterias);
    return 0;
}