#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "config/config.h"
#include "dominio/alumno.h"
#include "dominio/avl.h"
#include "dominio/materia.h"
#include "dominio/lista_materia.h"
#include "extensiones/persistencia.h"
#include "extensiones/paginado.h"
#include "extensiones/randomizador.h"
#include "util/utils.h"


void mostrarMenu(){
    limpiarPantalla();
    
    // Mostrar fecha y dia actual
    char fecha[30];
    char dia[20];
    mostrarFechaActual(dia, fecha);
    
    printf("%s, %s\n\n", dia, fecha);
    
    printf("--- MENU ---\n");
    printf("1. Agregar alumno\n");
    printf("2. Listar alumnos\n");
    printf("3. Buscar alumno por apellido\n");
    printf("4. Buscar alumno por rango de edad\n");
    printf("5. Modificar alumno\n");
    printf("6. Eliminar alumno\n");
    printf("7. Agregar materia\n");
    printf("8. Listar materias\n");
    printf("9. Modificar materia\n");
    printf("10. Eliminar materia\n");
    printf("11. Inscribir alumno en materia\n");
    printf("12. Rendir materia\n");
    printf("13. Listar materias rendidas por un alumno\n");
    printf("14. Listar materias aprobadas por un alumno\n");
    printf("15. Generar alumnos aleatorios\n");
    printf("16. Generar materias aleatorias\n");
    printf("17. Modificar correlatividades\n");
    printf("18. Cargar plan de estudios\n");
    printf("19. Salir\n");
    printf("Elija una opcion\n");
}

void cargarIDDesdeArchivo() {
    FILE* f = fopen("./data/ultimo_id.txt", "r");
    if(f) {
        int id;
        if(fscanf(f, "%d", &id) == 1 && id > 0) {
            establecerUltimoID(id);
            // Se elimino la impresion del ID cargado
        } else {
            printf("Error al leer el ID del archivo o ID invalido. Iniciando con ID = 0.\n");
            establecerUltimoID(0);
        }
        fclose(f);
    } else {
        printf("No se pudo abrir el archivo de IDs. Iniciando con ID = 0.\n");
        establecerUltimoID(0);
        
        // Intentar crear el directorio si no existe
        crearDirectorioData();
    }
}

void guardarIDEnArchivo() {
    // Intentar crear el directorio si no existe
    crearDirectorioData();
    
    FILE* f = fopen("./data/ultimo_id.txt", "w");
    if(f) {
        int id = obtenerUltimoID();
        if (fprintf(f, "%d\n", id) < 0) {
            printf("Error al escribir el ID en el archivo\n");
        }
        fclose(f);
    } else {
        printf("Error: No se pudo abrir el archivo para guardar el ID\n");
    }
}

int main() {
    // Limpiar la pantalla al inicio del programa
    limpiarPantalla();
    
    // Mostrar fecha y dia actual en la pantalla inicial
    char fecha[30];
    char dia[20];
    mostrarFechaActual(dia, fecha);
    printf("%s, %s\n\n", dia, fecha);
    
    cargarIDDesdeArchivo();
    NodoAVL* alumnos = NULL;
    NodoMateria* listaMaterias = NULL;

    if (datosGuardadosDisponibles()) {
        cargarDatos(&alumnos, &listaMaterias);
        if (!alumnos && !listaMaterias) {
            printf("Advertencia: No se pudieron cargar los datos correctamente.\n");
        }
    } else {
        printf("Iniciando sistema vacio.\n");
        alumnos = NULL;
        listaMaterias = NULL;
    }
    
    // Actualizar la referencia global a la lista de materias
    actualizarReferenciaMaterias(listaMaterias);

    int opcion;
    char nombre[100];
    int edad;

    do{
        mostrarMenu();
        opcion = pedirInt("Ingrese una opcion: ");

        // Validar que la opcion este en el rango valido
        if (opcion < 1 || opcion > 18) {
            printf("Error: Opcion invalida. Por favor ingrese un numero entre 1 y 18.\n");
            pausar();
            continue;
        }

        switch(opcion) {
            // Agregar alumno
            case 1: {
                char nombre[100];
                int edad;

                pedirString("Ingrese nombre del alumno: ", nombre, MAX_NOMBRE);
                
                // Solicitar apellido
                char apellido[50];
                printf("Ingrese apellido: ");
                fflush(stdin);
                fgets(apellido, sizeof(apellido), stdin);
                // Eliminar el salto de linea si existe
                char *nl = strchr(apellido, '\n');
                if (nl) *nl = '\0';
                
                char mensajeEdad[100];
                sprintf(mensajeEdad, "Ingrese edad (%d-%d): ", EDAD_MINIMA, EDAD_MAXIMA);
                
                do {
                    edad = pedirInt(mensajeEdad);
                    if (!edadValida(edad)) {
                        printf("Error: La edad debe estar entre %d y %d años.\n", EDAD_MINIMA, EDAD_MAXIMA);
                    }
                } while (!edadValida(edad));

                Alumno nuevo = crearAlumno(nombre, apellido, edad);
                
                // Verificar si el ID es valido (mayor que 0)
                if (nuevo.id <= 0) {
                    printf("Error: No se pudo crear el alumno\n");
                } else {
                    NodoAVL* resultado = insertarAVL(alumnos, nuevo);
                    if (resultado) {
                        alumnos = resultado;
                        printf("Alumno agregado correctamente\n");
                    } else {
                        printf("Error: No se pudo agregar el alumno al arbol\n");
                    }
                }
                
                pausar();
                break;
            }

            // Listar alumnos
            case 2:
            listarAlumnosPaginado(alumnos);
            break;

            //Buscar alumno por apellido
            case 3: {
                char apellido[100];
                printf("Ingrese apellido para buscar: ");
                fgets(apellido, sizeof(apellido), stdin);
                apellido[strcspn(apellido, "\n")] = '\0';

                NodoAVL* alumno = buscarAlumnoPorApellidoAVL(alumnos, apellido);
                if(alumno) {
                    printf("Alumno encontrado: ID: %d | Nombre: %s %s | Edad: %d\n",
                            alumno->alumno.id, alumno->alumno.nombre, alumno->alumno.apellido, alumno->alumno.edad);
                } else {
                printf("Alumno no encontrado\n");
                }
                pausar();
                break;
            }

            // Buscar alumno por rango de edad
            case 4: {
                int edadMin, edadMax;
                
                do {
                    edadMin = pedirInt("Ingrese edad minima (18-99): ");
                    if (!edadValida(edadMin)) {
                        printf("Error: La edad minima debe estar entre %d y %d años.\n", EDAD_MINIMA, EDAD_MAXIMA);
                    }
                } while (!edadValida(edadMin));
                
                do {
                    edadMax = pedirInt("Ingrese edad maxima (18-99): ");
                    if (!edadValida(edadMax)) {
                        printf("Error: La edad maxima debe estar entre %d y %d años.\n", EDAD_MINIMA, EDAD_MAXIMA);
                    }
                    if (edadMax < edadMin) {
                        printf("Error: La edad maxima no puede ser menor que la minima (%d).\n", edadMin);
                    }
                } while (!edadValida(edadMax) || edadMax < edadMin);

                int encontrados = 0;
                buscarAlumnosPorEdadAVL(alumnos, edadMin, edadMax, &encontrados);

                if(encontrados == 0){
                    printf("No se encontraron alumnos entre %d y %d años.\n", edadMin,edadMax);
                } else {
                    printf("Total de alumnos encontrados: %d\n", encontrados);
                }

                pausar();
                break;
            }

            //Modificar alumno
            case 5: {
                int IDAlumno = pedirInt("Ingrese ID del alumno a modificar: ");
                modificarAlumnoAVL(alumnos, IDAlumno);
                pausar();
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
                
                // Pedir confirmacion
                printf("¿Esta seguro que desea eliminar al alumno %s (ID: %d)? (S/N): ", 
                       alumno->alumno.nombre, alumno->alumno.id);
                
                char confirmacion[10];
                fgets(confirmacion, sizeof(confirmacion), stdin);
                confirmacion[strcspn(confirmacion, "\n")] = '\0';
                
                if (confirmacion[0] == 'S' || confirmacion[0] == 's') {
                    alumnos = eliminarAlumnoAVL(alumnos, IDAlumno);
                    printf("Alumno eliminado correctamente.\n");
                } else {
                    printf("Operacion cancelada.\n");
                }
                pausar();
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
                pausar();
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
                pausar();
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
                
                // Pedir confirmacion
                printf("¿Esta seguro que desea eliminar la materia %s (ID: %d)? (S/N): ", 
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
                    printf("Operacion cancelada.\n");
                }
                pausar();
                break;
            }

            // Inscribir alumno en materia
            case 11: {
                listarAlumnosPaginado(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno a inscribir: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno) {
                    printf("Error: Alumno no encontrado.\n");
                    break;
                }
                
                // Verificar si el alumno ya alcanzo el maximo de materias
                if (alumno->alumno.cantidadDeMateriasInscripto >= MAX_MATERIAS_POR_ALUMNO) {
                    printf("Error: El alumno ya esta inscripto en el maximo de materias permitidas (%d).\n", 
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
                
                // Verificar si la materia ya alcanzo el maximo de alumnos
                if (materia->datos.cantidadAlumnos >= MAX_ALUMNOS_POR_MATERIA) {
                    printf("Error: La materia ya tiene el maximo de alumnos inscriptos (%d).\n", 
                           MAX_ALUMNOS_POR_MATERIA);
                    break;
                }
                
                // Verificar si el alumno ya esta inscripto en esta materia
                int yaInscripto = 0;
                for (int i = 0; i < alumno->alumno.cantidadDeMateriasInscripto; i++) {
                    if (alumno->alumno.materiasInscripto[i] == IDMateria) {
                        yaInscripto = 1;
                        break;
                    }
                }
                
                if (yaInscripto) {
                    printf("Error: El alumno ya esta inscripto en esta materia.\n");
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
                            
                            // Verificar si el alumno aprobo esta correlativa
                            if (!haAprobadoMateria(&alumno->alumno, idCorrelativa)) {
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
                
                // Verificar regla especial para materias con ID >= ID_MATERIAS_AVANZADAS
                if (materia->datos.id >= ID_MATERIAS_AVANZADAS) {
                    printf("Verificando requisito especial: para materias avanzadas (ID >= %d) se requiere aprobar todas las materias anteriores.\n", ID_MATERIAS_AVANZADAS);
                    
                    if (!haAprobadoTodasHastaID(&alumno->alumno, ID_MATERIAS_AVANZADAS)) {
                        printf("Error: Para inscribirse en materias avanzadas (ID >= %d), debe aprobar todas las materias anteriores\n", ID_MATERIAS_AVANZADAS);
                        
                        // Mostrar algunas materias faltantes (limitar para no saturar la pantalla)
                        int materiasFaltantesMostradas = 0;
                        for (int id = 1; id < ID_MATERIAS_AVANZADAS && materiasFaltantesMostradas < 5; id++) {
                            if (!haAprobadoMateria(&alumno->alumno, id)) {
                                NodoMateria* materiaFaltante = buscarMateriaPorID(listaMaterias, id);
                                if (materiaFaltante) {
                                    printf("Falta aprobar: %s (ID: %d)\n", materiaFaltante->datos.nombre, id);
                                    materiasFaltantesMostradas++;
                                }
                            }
                        }
                        
                        if (materiasFaltantesMostradas == 5) {
                            printf("... y otras materias adicionales.\n");
                        }
                        
                        break;
                    }
                    
                    printf("Requisito especial verificado: todas las materias anteriores estan aprobadas.\n");
                }
                
                if(!inscribirAlumnoEnMateria(&alumno->alumno, &materia->datos)) {
                    printf("Error: No se pudo inscribir al alumno.\n");
                } else {
                    printf("Alumno inscripto correctamente en la materia.\n");
                }
                pausar();
                break;
            }

            // Rendir materia
            case 12: {
                listarAlumnosPaginado(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Error: Alumno no encontrado\n");
                    break;
                }
                
                // Verificar si el alumno esta inscripto en alguna materia
                if (alumno->alumno.cantidadDeMateriasInscripto == 0) {
                    printf("Error: El alumno no esta inscripto en ninguna materia.\n");
                    break;
                }

                printf("Materias inscriptas: \n");
                int materiasDisponibles = 0;
                for(int i = 0; i < alumno->alumno.cantidadDeMateriasInscripto; i++){
                    int IDMat = alumno->alumno.materiasInscripto[i];
                    
                    // Verificar si ya rindio esta materia
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
                
                // Verificar si el alumno esta inscripto en esta materia
                int estaInscripto = 0;
                for (int i = 0; i < alumno->alumno.cantidadDeMateriasInscripto; i++) {
                    if (alumno->alumno.materiasInscripto[i] == IDMateria) {
                        estaInscripto = 1;
                        break;
                    }
                }
                
                if (!estaInscripto) {
                    printf("Error: El alumno no esta inscripto en esta materia.\n");
                    break;
                }
                
                // Verificar si ya aprobo esta materia
                for (int j = 0; j < alumno->alumno.cantidadMateriasRendidas; j++) {
                    if (alumno->alumno.materiasRendidas[j].IDMateria == IDMateria && 
                        alumno->alumno.materiasRendidas[j].aprobo) {
                        printf("Error: El alumno ya aprobo esta materia anteriormente.\n");
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

                NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
                if (materia && rendirMateria(&alumno->alumno, &materia->datos, nota)) {
                    printf("Materia %s rendida correctamente con nota %.2f (%s).\n", 
                           materia->datos.nombre, nota, (nota >= NOTA_APROBACION) ? "Aprobada" : "Desaprobada");
                } else {
                    printf("Error: No se pudo registrar la materia como rendida.\n");
                }
                pausar();
                break;
            }

            // Listar materias rendidas por un alumno
            case 13: {
                listarAlumnosPaginado(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado\n");
                    break;
                }

                listarMateriasRendidas(&alumno->alumno, listaMaterias);
                pausar();
                break;
            }
            
            case 14: {
                listarAlumnosPaginado(alumnos);
                int IDAlumno = pedirInt("Ingrese ID del alumno: ");

                NodoAVL* alumno = buscarAlumnoPorIDAVL(alumnos, IDAlumno);
                if(!alumno){
                    printf("Alumno no encontrado\n");
                    break;
                }

                mostrarMateriasAprobadas(&alumno->alumno, listaMaterias);
                pausar();
                break;
            }
            
            case 15: {
                int n;
                do {
                    n = pedirInt("¿Cuantos alumnos desea generar? (minimo 1): ");
                    if (n <= 0) {
                        printf("Error: Debe generar al menos 1 alumno.\n");
                    }
                } while (n <= 0);
                
                printf("Intentando generar %d alumnos aleatorios...\n", n);
                generarAlumnosAleatorios(&alumnos, n);
                pausar();
                break;
            }
            case 16: {
                int n;
                do {
                    n = pedirInt("¿Cuantas materias desea generar? (1-50): ");
                    if (n <= 0 || n > 50) {
                        printf("Error: Ingrese un numero entre 1 y 50.\n");
                    }
                } while (n <= 0 || n > 50);
                
                printf("Generando %d materias aleatorias...\n", n);
                generarMateriasAleatorias(&listaMaterias, n);
                printf("Materias aleatorias generadas correctamente.\n");
                pausar();
                break;
            }
            case 17: {
                listarMateriasPaginado(listaMaterias);
                int IDMateria = pedirInt("Ingrese ID de la materia para modificar correlatividades: ");
                
                // Verificar si la materia existe
                NodoMateria* materia = buscarMateriaPorID(listaMaterias, IDMateria);
                if (!materia) {
                    printf("Error: No existe una materia con ID %d\n", IDMateria);
                    break;
                }
                
                // Llamar a la funcion para modificar correlatividades
                if (modificarCorrelativasMateria(listaMaterias, IDMateria)) {
                    printf("Correlatividades modificadas correctamente.\n");
                    
                    // Preguntar si quiere guardar los cambios
                    char opcionGuardar;
                    printf("¿Desea guardar los cambios en el plan de estudios? (S/N): ");
                    scanf(" %c", &opcionGuardar);
                    getchar(); // Limpiar el buffer de entrada
                    
                    if (opcionGuardar == 'S' || opcionGuardar == 's') {
                        // Guardar solo el plan de estudios
                        guardarPlanEstudiosCSV(listaMaterias);
                    } else {
                        printf("Cambios no guardados en el archivo.\n");
                    }
                } else {
                    printf("No se realizaron cambios en las correlatividades.\n");
                }
                
                pausar();
                break;
            }
            
            case 18: {
                printf("Cargando plan de estudios...\n");
                cargarPlanEstudiosCSV(&listaMaterias);
                
                // Actualizar la referencia global a la lista de materias
                actualizarReferenciaMaterias(listaMaterias);
                
                printf("Plan de estudios cargado correctamente.\n");
                pausar();
                break;
            }
            
            case 19: {
                char opcionGuardar;
                printf("¿Desea guardar los datos antes de salir? (S/N): ");
                scanf(" %c", &opcionGuardar);
                getchar(); // Limpiar el buffer de entrada
                
                if (opcionGuardar == 'S' || opcionGuardar == 's') {
                    guardarDatos(alumnos, listaMaterias);
                    guardarIDEnArchivo();
                    printf("Datos guardados correctamente.\n");
                } else {
                    printf("Datos no guardados.\n");
                }
                
                printf("Saliendo...\n");
                // No pausamos en el caso de salir
                break;
            }

            default:
            printf("Opcion invalida\n");
            pausar();
        }
    } while(opcion!= 19);

    liberarAVL(alumnos);
    liberarListaMaterias(listaMaterias);
    return 0;
}