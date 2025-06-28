// 1: INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paginado.h"
#include "utils.h"
#include "../dominio/avl.h"
#include "../dominio/alumno.h"
#include "../dominio/lista_materia.h"
#include "../include/config.h"

// 2: PROTOTIPOS INTERNOS
static void mostrarAlumnos(void* datos, int inicio, int fin);
static void mostrarMaterias(void* datos, int inicio, int fin);
static void mostrarEncabezadoAlumnos();
static void mostrarEncabezadoMaterias();
static NodoMateria** obtenerMaterias(NodoMateria* cabeza, int inicio, int fin, int* cantidadObtenida);
static void navegacionEntrePaginas(int* pagina, int paginasTotales, char direccion);
static void pausarYLimpiar();
static void iniciarPaginacion(int total, int itemsPorPagina, int* pagina, int* paginasTotales);
static void calcularRangoPagina(int pagina, int itemsPorPagina, int total, int* inicio, int* fin);

// 3: FUNCIONES AUXILIARES

// Función auxiliar para navegación entre páginas
static void navegacionEntrePaginas(int* pagina, int paginasTotales, char direccion) {
    if (direccion == 'n' || direccion == 'N') {
        if (*pagina + 1 < paginasTotales) {
            (*pagina)++;
            limpiarPantalla();
        } else {
            printf("Ya esta en la ultima pagina.\n");
        }
    } else if (direccion == 'p' || direccion == 'P') {
        if (*pagina > 0) {
            (*pagina)--;
            limpiarPantalla();
        } else {
            printf("Ya esta en la primera pagina.\n");
        }
    }
}

// Funcion para mostrar encabezado de tabla de alumnos
static void mostrarEncabezadoAlumnos() {
    printf("\n%-5s | %-30s | %-5s | %-20s\n", "ID", "Nombre", "Edad", "Materias inscriptas");
    printf("----------------------------------------------------------------------\n");
}

// Funcion para mostrar encabezado de tabla de materias
static void mostrarEncabezadoMaterias() {
    printf("\n%-5s | %-30s | %-20s | %-40s\n", "ID", "Nombre", "Cantidad de Alumnos", "Correlativas");
    printf("-----------------------------------------------------------------------------------------------------\n");
}

static NodoMateria** obtenerMaterias(NodoMateria* cabeza, int inicio, int fin, int* cantidadObtenida) {
    NodoMateria* actual = cabeza;
    int count = fin - inicio;
    
    // Crear un array para almacenar las materias en este rango
    NodoMateria** materiasArray = (NodoMateria**)malloc(count * sizeof(NodoMateria*));
    if (!materiasArray) {
        printf("Error de memoria al obtener materias\n");
        return NULL;
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
    
    *cantidadObtenida = arrayIndex;
    return materiasArray;
}

// Función para inicializar variables de paginación
static void iniciarPaginacion(int total, int itemsPorPagina, int* pagina, int* paginasTotales) {
    *pagina = 0;
    *paginasTotales = (total + itemsPorPagina - 1) / itemsPorPagina;
}

// Función para calcular el rango de elementos a mostrar en la página actual
static void calcularRangoPagina(int pagina, int itemsPorPagina, int total, int* inicio, int* fin) {
    *inicio = pagina * itemsPorPagina;
    *fin = *inicio + itemsPorPagina;
    if (*fin > total) *fin = total;
}

// 4: VISUALIZACION
// Funcion para mostrar alumnos con opciones adicionales
static void mostrarAlumnos(void* datos, int inicio, int fin) {
    Alumno* alumnos = (Alumno*)datos;
    mostrarEncabezadoAlumnos();
    for (int i = inicio; i < fin; i++) {
        visualizarAlumno(alumnos[i], 1); // 1 = formato avanzado
    }
}

// Funcion para mostrar materias con opciones adicionales
static void mostrarMaterias(void* datos, int inicio, int fin) {
    NodoMateria** lista = (NodoMateria**)datos;
    NodoMateria* cabeza = *lista;
    
    int cantidadMaterias = 0;
    NodoMateria** materiasArray = obtenerMaterias(cabeza, inicio, fin, &cantidadMaterias);
    
    if (!materiasArray) {
        return;
    }
    
    // Mostrar encabezado de tabla
    mostrarEncabezadoMaterias();
    
    // Mostrar las materias en el orden de la lista
    for (int j = 0; j < cantidadMaterias; j++) {
        visualizarMateria(materiasArray[j]->datos, 1); // 1 = formato avanzado
    }
    
    free(materiasArray);
}

// Función auxiliar para pausar y limpiar la pantalla
static void pausarYLimpiar() {
    char buffer[16];
    printf("Presione Enter para continuar...");
    fgets(buffer, sizeof(buffer), stdin);
    limpiarPantalla();
}

// 5: PAGINADO DE ALUMNOS
// Funcion para paginado avanzado de alumnos
void listarAlumnosPaginado(NodoAVL* raiz) {
    if (!raiz) {
        printf("No hay alumnos para mostrar.\n");
        pausar();  // Añadimos una pausa para que el usuario pueda leer el mensaje
        return;
    }
    
    limpiarPantalla(); // Limpiar la pantalla al inicio
    
    int total = contarAlumnos(raiz);
    
    // Reservar memoria para el array
    Alumno* alumnos = malloc(sizeof(Alumno) * total);
    if (!alumnos) {
        printf("Error de memoria\n");
        return;
    }
    
    int idx = 0;  // Cambiado de index a idx para evitar posibles conflictos
    recorrerInOrden(raiz, alumnos, &idx);
    
    int pagina;
    char opcion;
    int itemsPorPagina = PAGINADO_TAM;
    int paginasTotales;
    
    iniciarPaginacion(total, itemsPorPagina, &pagina, &paginasTotales);
    
    do {
        printf("\n--- Listado de Alumnos (Pagina %d de %d) ---\n", pagina + 1, paginasTotales);
        
        int inicio, fin;
        calcularRangoPagina(pagina, itemsPorPagina, total, &inicio, &fin);
        
        mostrarAlumnos(alumnos, inicio, fin);
        
        printf("\nOpciones de navegacion:\n");
        printf("[N] Pagina siguiente | [P] Pagina anterior | [B] Buscar por apellido\n");
        printf("[E] Buscar por edad  | [I] Buscar por ID   | [M] Volver al menu principal\n");
        printf("Elija una opcion: ");
        
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcion = buffer[0];
        } else {
            opcion = 'm'; // Si hay error, volver al menu
        }
        
        switch(opcion) {
            case 'n': case 'N':
            case 'p': case 'P':
                navegacionEntrePaginas(&pagina, paginasTotales, opcion);
                break;
                
            case 'b': case 'B': {
                char apellido[100];
                printf("Ingrese apellido para buscar: ");
                fgets(apellido, sizeof(apellido), stdin);
                apellido[strcspn(apellido, "\n")] = '\0';
                
                printf("\nResultados de busqueda para apellido '%s':\n", apellido);
                
                int encontrados = 0;
                for (int i = 0; i < total; i++) {
                    if (strcasestr(alumnos[i].apellido, apellido) != NULL) {
                        visualizarAlumno(alumnos[i], 0); // Usar la funcion comun
                        encontrados++;
                    }
                }
                
                if (encontrados == 0) {
                    printf("No se encontraron alumnos con ese apellido.\n");
                } else {
                    printf("Total encontrados: %d\n", encontrados);
                }
                
                pausarYLimpiar();
                break;
            }
            
            case 'e': case 'E': {
                int edadMin, edadMax;
                printf("Ingrese edad minima: ");
                scanf("%d", &edadMin);
                getchar(); // Consumir el salto de linea
                
                printf("Ingrese edad maxima: ");
                scanf("%d", &edadMax);
                getchar(); // Consumir el salto de linea
                
                if (edadMin > edadMax) {
                    int temp = edadMin;
                    edadMin = edadMax;
                    edadMax = temp;
                }
                
                printf("\nAlumnos con edad entre %d y %d:\n", edadMin, edadMax);
                
                int encontrados = 0;
                buscarAlumnosPorEdadAVL(raiz, edadMin, edadMax, &encontrados);
                
                if (encontrados == 0) {
                    printf("No se encontraron alumnos en ese rango de edad.\n");
                } else {
                    printf("Total encontrados: %d\n", encontrados);
                }
                
                pausarYLimpiar();
                break;
            }
            
            case 'i': case 'I': {
                int id;
                printf("Ingrese ID del alumno: ");
                scanf("%d", &id);
                getchar(); // Consumir el salto de linea
                
                // Usar la funcion existente para buscar por ID
                NodoAVL* nodoEncontrado = buscarAlumnoPorIDAVL(raiz, id);
                
                if (nodoEncontrado) {
                    Alumno alumnoEncontrado = nodoEncontrado->alumno;
                    printf("\nAlumno encontrado:\n");
                    visualizarAlumno(alumnoEncontrado, 0); // Usar la funcion comun
                } else {
                    printf("No se encontro ningun alumno con ID %d.\n", id);
                }
                
                pausarYLimpiar();
                break;
            }
            
            case 'm': case 'M':
                // Salir al menu principal
                break;
                
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opcion != 'm' && opcion != 'M');
    
    free(alumnos);
}

// 6: PAGINADO DE MATERIAS
// Funcion para paginado avanzado de materias
void listarMateriasPaginado(NodoMateria* cabeza) {
    if (!cabeza) {
        printf("No hay materias para mostrar.\n");
        pausar();  // Añadimos una pausa para que el usuario pueda leer el mensaje
        return;
    }
    
    limpiarPantalla(); // Limpiar la pantalla al inicio
    
    int total = contarNodosMaterias(cabeza);
    int pagina;
    char opcion;
    int itemsPorPagina = PAGINADO_TAM;
    int paginasTotales;
    
    iniciarPaginacion(total, itemsPorPagina, &pagina, &paginasTotales);
    
    do {
        printf("\n--- Listado de Materias (Pagina %d de %d) ---\n", pagina + 1, paginasTotales);
        
        int inicio, fin;
        calcularRangoPagina(pagina, itemsPorPagina, total, &inicio, &fin);
        
        mostrarMaterias(&cabeza, inicio, fin);
        
        printf("\nOpciones de navegacion:\n");
        printf("[N] Pagina siguiente | [P] Pagina anterior | [B] Buscar por nombre\n");
        printf("[A] Ordenar por alumnos | [I] Buscar por ID | [M] Volver al menu principal\n");
        printf("Elija una opcion: ");
        
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcion = buffer[0];
        } else {
            opcion = 'm'; // Si hay error, volver al menu
        }
        
        switch(opcion) {
            case 'n': case 'N':
            case 'p': case 'P':
                navegacionEntrePaginas(&pagina, paginasTotales, opcion);
                break;
                
            case 'b': case 'B': {
                char nombre[100];
                printf("Ingrese nombre para buscar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
                
                printf("\nResultados de busqueda para '%s':\n", nombre);
                
                int encontrados = 0;
                NodoMateria* actual = cabeza;
                
                // Recorremos toda la lista buscando coincidencias parciales
                while (actual) {
                    if (strcasestr(actual->datos.nombre, nombre) != NULL) {
                        visualizarMateria(actual->datos, 0); // Usar la funcion comun
                        encontrados++;
                    }
                    actual = actual->siguiente;
                }
                
                if (encontrados == 0) {
                    printf("No se encontraron materias con ese nombre.\n");
                } else {
                    printf("Total encontradas: %d\n", encontrados);
                }
                
                pausarYLimpiar();
                break;
            }
            
            case 'i': case 'I': {
                int id;
                printf("Ingrese ID de la materia: ");
                scanf("%d", &id);
                getchar(); // Consumir el salto de linea
                
                // Usar la funcion existente para buscar por ID
                NodoMateria* materiaEncontrada = buscarMateriaPorID(cabeza, id);
                
                if (materiaEncontrada) {
                    printf("\nMateria encontrada:\n");
                    visualizarMateria(materiaEncontrada->datos, 0); // Usar la funcion comun
                } else {
                    printf("No se encontro ninguna materia con ID %d.\n", id);
                }
                
                pausarYLimpiar();
                break;
            }
            
            case 'a': case 'A': {
                // Esta funcionalidad requeriria copiar todas las materias a un array,
                // ordenarlas y mostrarlas. Por ahora solo mostramos un mensaje.
                printf("Funcionalidad de ordenamiento por cantidad de alumnos pendiente.\n");
                pausarYLimpiar();
                break;
            }
            
            case 'm': case 'M':
                // Salir al menu principal
                break;
                
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opcion != 'm' && opcion != 'M');
}