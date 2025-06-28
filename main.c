#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "include/config.h"
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

void cargarIDDesdeArchivo() {
    FILE* f = fopen("./data/ultimo_id.txt", "r");
    if(f) {
        int id;
        if(fscanf(f, "%d", &id) == 1 && id > 0) {
            establecerUltimoID(id);
            printf("ID cargado: %d\n", id);
        } else {
            printf("Error al leer el ID del archivo o ID inválido. Iniciando con ID = 0.\n");
            establecerUltimoID(0);
        }
        fclose(f);
    } else {
        printf("No se pudo abrir el archivo de IDs. Iniciando con ID = 0.\n");
        establecerUltimoID(0);
        
        // Intentar crear el directorio si no existe
        #ifdef _WIN32
            mkdir("./data");
        #else
            mkdir("./data", 0777);
        #endif
    }
}

void guardarIDEnArchivo() {
    // Intentar crear el directorio si no existe
    #ifdef _WIN32
        mkdir("./data");
    #else
        mkdir("./data", 0777);
    #endif
    
    FILE* f = fopen("./data/ultimo_id.txt", "w");
    if(f) {
        int id = obtenerUltimoID();
        if (fprintf(f, "%d\n", id) < 0) {
            printf("Error al escribir el ID en el archivo\n");
        } else {
            printf("ID %d guardado correctamente\n", id);
        }
        fclose(f);
    } else {
        printf("Error: No se pudo abrir el archivo para guardar el ID\n");
    }
}

int main() {
    cargarIDDesdeArchivo();
    NodoAVL* alumnos = NULL;
    NodoMateria* listaMaterias = NULL;

    if (datosGuardadosDisponibles()) {
        printf("Cargando datos previos...\n");
        cargarDatos(&alumnos, &listaMaterias);
        if (!alumnos && !listaMaterias) {
            printf("Advertencia: No se pudieron cargar los datos correctamente.\n");
        }
    } else {
        printf("Iniciando sistema vacío.\n");
        alumnos = NULL;
        listaMaterias = NULL;
    }

    int opcion;
    char nombre[100];
    int edad;

    do{
        mostrarMenu();
        opcion = pedirInt("Ingrese una opcion: ");

        // Validar que la opción esté en el rango válido
        if (opcion < 1 || opcion > 16) {
            printf("Error: Opción inválida. Por favor ingrese un número entre 1 y 16.\n");
            continue;
        }

        switch(opcion) {
            // Agregar alumno
            case 1: {
                char nombre[100];
                int edad;

                pedirString("Ingrese nombre del alumno: ", nombre, MAX_NOMBRE);
                
                char mensajeEdad[100];
                sprintf(mensajeEdad, "Ingrese edad (%d-%d): ", EDAD_MINIMA, EDAD_MAXIMA);
                
                do {
                    edad = pedirInt(mensajeEdad);
                    if (!edadValida(edad)) {
                        printf("Error: La edad debe estar entre %d y %d años.\n", EDAD_MINIMA, EDAD_MAXIMA);
                    }
                } while (!edadValida(edad));

                Alumno nuevo = crearAlumno(nombre, edad);
                
                // Verificar si el ID es válido (mayor que 0)
                if (nuevo.id <= 0) {
                    printf("Error: No se pudo crear el alumno\n");
                    break;
                }
                
                NodoAVL* resultado = insertarAVL(alumnos, nuevo);
                if (resultado) {
                    alumnos = resultado;
                    printf("Alumno agregado correctamente\n");
                } else {
                    printf("Error: No se pudo agregar el alumno al árbol\n");
                }
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
                
                do {
                    edadMin = pedirInt("Ingrese edad minima (18-99): ");
                    if (!edadValida(edadMin)) {
                        printf("Error: La edad mínima debe estar entre %d y %d años.\n", EDAD_MINIMA, EDAD_MAXIMA);
                    }
                } while (!edadValida(edadMin));
                
                do {
                    edadMax = pedirInt("Ingrese edad maxima (18-99): ");
                    if (!edadValida(edadMax)) {
                        printf("Error: La edad máxima debe estar entre %d y %d años.\n", EDAD_MINIMA, EDAD_MAXIMA);
                    }
                    if (edadMax < edadMin) {
                        printf("Error: La edad máxima no puede ser menor que la mínima (%d).\n", edadMin);
                    }
                } while (!edadValida(edadMax) || edadMax < edadMin);

                int encontrados = 0;
                buscarAlumnosPorEdadAVL(alumnos, edadMin, edadMax, &encontrados);

                if(encontrados == 0){
                    printf("No se encontraron alumnos entre %d y %d años.\n", edadMin,edadMax);
                } else {
                    printf("Total de alumnos encontrados: %d\n", encontrados);
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
                
                // Verificar si el alumno existe
                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if (!alumno) {
                    printf("Error: No existe un alumno con ID %d\n", IDAlumno);
                    break;
                }
                
                // Pedir confirmación
                printf("¿Está seguro que desea eliminar al alumno %s (ID: %d)? (S/N): ", 
                       alumno->alumno.nombre, alumno->alumno.id);
                
                char confirmacion[10];
                fgets(confirmacion, sizeof(confirmacion), stdin);
                confirmacion[strcspn(confirmacion, "\n")] = '\0';
                
                if (confirmacion[0] == 'S' || confirmacion[0] == 's') {
                    alumnos = eliminarAlumnoAVL(alumnos, IDAlumno);
                    printf("Alumno eliminado correctamente.\n");
                } else {
                    printf("Operación cancelada.\n");
                }
                break;
            }

            // Agregar materia
            case 7: {
                char nombre[MAX_NOMBRE];
                pedirString("Ingrese nombre de la materia: ", nombre, MAX_NOMBRE);
                
                NodoMateria* resultado = agregarMateria(&listaMaterias, nombre);
                if (resultado) {
                    printf("Materia agregada correctamente\n");
                } else {
                    printf("Error: No se pudo agregar la materia\n");
                }
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
                
                // Verificar si la materia existe
                NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
                if (!materia) {
                    printf("Error: No existe una materia con ID %d\n", IDMateria);
                    break;
                }
                
                // Comprobar si hay alumnos inscriptos en la materia
                if (materia->datos.cantidadAlumnos > 0) {
                    printf("Advertencia: Hay %d alumno(s) inscripto(s) en esta materia.\n", 
                           materia->datos.cantidadAlumnos);
                }
                
                // Pedir confirmación
                printf("¿Está seguro que desea eliminar la materia %s (ID: %d)? (S/N): ", 
                       materia->datos.nombre, materia->datos.id);
                
                char confirmacion[10];
                fgets(confirmacion, sizeof(confirmacion), stdin);
                confirmacion[strcspn(confirmacion, "\n")] = '\0';
                
                if (confirmacion[0] == 'S' || confirmacion[0] == 's') {
                    if (!eliminarMateria(&listaMaterias, IDMateria)) {
                        printf("Error: No se pudo eliminar la materia.\n");
                    } else {
                        printf("Materia eliminada correctamente.\n");
                    }
                } else {
                    printf("Operación cancelada.\n");
                }
                break;
            }

            // Inscribir alumno en materia
            case 11: {
                listarAlumnosPaginadoAVL(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno a inscribir: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno) {
                    printf("Error: Alumno no encontrado.\n");
                    break;
                }
                
                // Verificar si el alumno ya alcanzó el máximo de materias
                if (alumno->alumno.cantidadDeMateriasInscripto >= MAX_MATERIAS_POR_ALUMNO) {
                    printf("Error: El alumno ya está inscripto en el máximo de materias permitidas (%d).\n", 
                           MAX_MATERIAS_POR_ALUMNO);
                    break;
                }

                listarMateriasPaginado(listaMaterias);
                int IDMateria = pedirInt("Ingrese ID de la materia: ");

                NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
                if(!materia) {
                    printf("Error: Materia no encontrada.\n");
                    break;
                }
                
                // Verificar si la materia ya alcanzó el máximo de alumnos
                if (materia->datos.cantidadAlumnos >= MAX_ALUMNOS_POR_MATERIA) {
                    printf("Error: La materia ya tiene el máximo de alumnos inscriptos (%d).\n", 
                           MAX_ALUMNOS_POR_MATERIA);
                    break;
                }
                
                // Verificar si el alumno ya está inscripto en esta materia
                int yaInscripto = 0;
                for (int i = 0; i < alumno->alumno.cantidadDeMateriasInscripto; i++) {
                    if (alumno->alumno.materiasInscripto[i] == IDMateria) {
                        yaInscripto = 1;
                        break;
                    }
                }
                
                if (yaInscripto) {
                    printf("Error: El alumno ya está inscripto en esta materia.\n");
                    break;
                }
                
                // Verificar correlativas
                if (materia->datos.cantidadCorrelativas > 0) {
                    printf("La materia tiene %d correlativa(s):\n", materia->datos.cantidadCorrelativas);
                    
                    int correlativasFaltantes = 0;
                    for (int i = 0; i < materia->datos.cantidadCorrelativas; i++) {
                        int idCorrelativa = materia->datos.correlativas[i];
                        NodoMateria* materiaCorrelativa = buscarMateriaPorID(listaMaterias, idCorrelativa);
                        
                        if (materiaCorrelativa) {
                            printf("- %s (ID: %d)\n", materiaCorrelativa->datos.nombre, idCorrelativa);
                            
                            // Verificar si el alumno aprobó esta correlativa
                            int aprobada = 0;
                            for (int j = 0; j < alumno->alumno.cantidadMateriasRendidas; j++) {
                                if (alumno->alumno.materiasRendidas[j].IDMateria == idCorrelativa && 
                                    alumno->alumno.materiasRendidas[j].aprobo) {
                                    aprobada = 1;
                                    break;
                                }
                            }
                            
                            if (!aprobada) {
                                correlativasFaltantes++;
                                printf("  > No aprobada\n");
                            } else {
                                printf("  > Aprobada\n");
                            }
                        }
                    }
                    
                    if (correlativasFaltantes > 0) {
                        printf("Error: El alumno debe aprobar todas las correlativas antes de inscribirse.\n");
                        break;
                    }
                }
                
                if(!inscribirAlumnoEnMateria(&alumno->alumno, &materia->datos)) {
                    printf("Error: No se pudo inscribir al alumno.\n");
                } else {
                    printf("Alumno inscripto correctamente en la materia.\n");
                }
                break;
            }

            // Rendir materia
            case 12: {
                listarAlumnosPaginadoAVL(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Error: Alumno no encontrado\n");
                    break;
                }
                
                // Verificar si el alumno está inscripto en alguna materia
                if (alumno->alumno.cantidadDeMateriasInscripto == 0) {
                    printf("Error: El alumno no está inscripto en ninguna materia.\n");
                    break;
                }

                printf("Materias inscriptas: \n");
                int materiasDisponibles = 0;
                for(int i = 0; i < alumno->alumno.cantidadDeMateriasInscripto; i++){
                    int IDMat = alumno->alumno.materiasInscripto[i];
                    
                    // Verificar si ya rindió esta materia
                    int yaRendida = 0;
                    for (int j = 0; j < alumno->alumno.cantidadMateriasRendidas; j++) {
                        if (alumno->alumno.materiasRendidas[j].IDMateria == IDMat && 
                            alumno->alumno.materiasRendidas[j].aprobo) {
                            yaRendida = 1;
                            break;
                        }
                    }
                    
                    if (!yaRendida) {
                        NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMat);
                        if(materia){
                            printf("ID: %d | Nombre: %s\n", materia->datos.id, materia->datos.nombre);
                            materiasDisponibles++;
                        }
                    }
                }
                
                if (materiasDisponibles == 0) {
                    printf("Error: El alumno no tiene materias pendientes para rendir.\n");
                    break;
                }

                int IDMateria = pedirInt("Ingrese ID de la materia a rendir: ");
                
                // Verificar si el alumno está inscripto en esta materia
                int estaInscripto = 0;
                for (int i = 0; i < alumno->alumno.cantidadDeMateriasInscripto; i++) {
                    if (alumno->alumno.materiasInscripto[i] == IDMateria) {
                        estaInscripto = 1;
                        break;
                    }
                }
                
                if (!estaInscripto) {
                    printf("Error: El alumno no está inscripto en esta materia.\n");
                    break;
                }
                
                // Verificar si ya aprobó esta materia
                for (int j = 0; j < alumno->alumno.cantidadMateriasRendidas; j++) {
                    if (alumno->alumno.materiasRendidas[j].IDMateria == IDMateria && 
                        alumno->alumno.materiasRendidas[j].aprobo) {
                        printf("Error: El alumno ya aprobó esta materia anteriormente.\n");
                        break;
                    }
                }
                
                float nota;
                do {
                    nota = pedirFloat("Ingrese nota obtenida (0-10): ");
                    if (nota < NOTA_MINIMA || nota > NOTA_MAXIMA) {
                        printf("Error: La nota debe estar entre %.1f y %.1f.\n", NOTA_MINIMA, NOTA_MAXIMA);
                    }
                } while (nota < NOTA_MINIMA || nota > NOTA_MAXIMA);

                if (rendirMateria(&alumno->alumno, IDMateria, nota)) {
                    NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
                    if (materia) {
                        printf("Materia %s rendida correctamente con nota %.2f (%s).\n", 
                               materia->datos.nombre, nota, (nota >= NOTA_APROBACION) ? "Aprobada" : "Desaprobada");
                    }
                } else {
                    printf("Error: No se pudo registrar la materia como rendida.\n");
                }
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
                int n;
                do {
                    n = pedirInt("¿Cuántos alumnos desea generar? (mínimo 1): ");
                    if (n <= 0) {
                        printf("Error: Debe generar al menos 1 alumno.\n");
                    }
                } while (n <= 0);
                
                printf("Intentando generar %d alumnos aleatorios...\n", n);
                generarAlumnosAleatorios(&alumnos, n);
                break;
            }
            case 15: {
                int n;
                do {
                    n = pedirInt("¿Cuántas materias desea generar? (1-50): ");
                    if (n <= 0 || n > 50) {
                        printf("Error: Ingrese un número entre 1 y 50.\n");
                    }
                } while (n <= 0 || n > 50);
                
                printf("Generando %d materias aleatorias...\n", n);
                generarMateriasAleatorias(&listaMaterias, n);
                printf("Materias aleatorias generadas correctamente.\n");
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
    guardarIDEnArchivo();
    return 0;
}