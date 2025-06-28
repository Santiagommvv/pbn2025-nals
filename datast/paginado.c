#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Añadido para strstr y otras funciones de string
#include "paginado.h"
#include "utils.h"
#include "../dominio/avl.h"
#include "../dominio/alumno.h"
#include "../dominio/lista_materia.h"
#include "../include/config.h"

// Prototipos de funciones internas
static void mostrarEncabezadoTablaAlumnos();
static void mostrarEncabezadoTablaMaterias();
static NodoMateria** obtenerMateriasEnRango(NodoMateria* cabeza, int inicio, int fin, int* cantidadObtenida);

// Funcion generica para manejar el paginado
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
        printf("\n--- Elementos (Pagina %d de %d) ---\n", pagina + 1, paginasTotales);
        
        int inicio = pagina * itemsPorPagina;
        int fin = inicio + itemsPorPagina;
        if (fin > total) fin = total;
        
        mostrarElementos(datos, inicio, fin);
        
        printf("\n[N] Pagina siguiente | [P] Pagina anterior | [M] Volver al menu principal\n");
        printf("Elija una opcion: ");
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcion = buffer[0];
        } else {
            opcion = 'm'; // Si hay error, volver al menu
        }
        
        if ((opcion == 'n' || opcion == 'N') && pagina + 1 < paginasTotales) {
            pagina++;
        } else if ((opcion == 'p' || opcion == 'P') && pagina > 0) {
            pagina--;
        } else if (opcion != 'm' && opcion != 'M') {
            printf("Opcion invalida.\n");
        }
    } while (opcion != 'm' && opcion != 'M');
}

// Funcion para mostrar alumnos en un rango especifico
static void mostrarAlumnos(void* datos, int inicio, int fin) {
    Alumno* alumnos = (Alumno*)datos;
    for (int i = inicio; i < fin; i++) {
        visualizarAlumno(alumnos[i], 0); // 0 = formato basico
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
    
    int idx = 0;  // Cambiado de index a idx para evitar posibles conflictos
    recorrerInOrden(raiz, alumnos, &idx);
    
    // Usar la funcion generica de paginado
    mostrarPaginado(total, PAGINADO_TAM, mostrarAlumnos, alumnos);
    
    free(alumnos);
}

// Funcion para mostrar materias en un rango especifico
static void mostrarMaterias(void* datos, int inicio, int fin) {
    NodoMateria** lista = (NodoMateria**)datos;
    NodoMateria* cabeza = *lista;
    
    int cantidadMaterias = 0;
    NodoMateria** materiasArray = obtenerMateriasEnRango(cabeza, inicio, fin, &cantidadMaterias);
    
    if (!materiasArray) {
        return;
    }
    
    // Mostrar las materias en el orden de la lista
    for (int j = 0; j < cantidadMaterias; j++) {
        visualizarMateria(materiasArray[j]->datos, 0); // 0 = formato basico
    }
    
    free(materiasArray);
}

void listarMateriasPaginado(NodoMateria* cabeza) {
    if (!cabeza) {
        printf("No hay materias para mostrar.\n");
        return;
    }
    
    int total = contarNodosMaterias(cabeza);
    
    // Usar la funcion generica de paginado
    mostrarPaginado(total, PAGINADO_TAM, mostrarMaterias, &cabeza);
}

// Funcion para mostrar alumnos con opciones adicionales
static void mostrarAlumnosAvanzado(void* datos, int inicio, int fin) {
    Alumno* alumnos = (Alumno*)datos;
    mostrarEncabezadoTablaAlumnos();
    for (int i = inicio; i < fin; i++) {
        visualizarAlumno(alumnos[i], 1); // 1 = formato avanzado
    }
}

// Funcion para paginado avanzado de alumnos
void listarAlumnosAvanzado(NodoAVL* raiz) {
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
    
    int idx = 0;  // Cambiado de index a idx para evitar posibles conflictos
    recorrerInOrden(raiz, alumnos, &idx);
    
    int pagina = 0;
    char opcion;
    int itemsPorPagina = PAGINADO_TAM;
    int paginasTotales = (total + itemsPorPagina - 1) / itemsPorPagina;
    
    do {
        printf("\n--- Listado de Alumnos (Pagina %d de %d) ---\n", pagina + 1, paginasTotales);
        
        int inicio = pagina * itemsPorPagina;
        int fin = inicio + itemsPorPagina;
        if (fin > total) fin = total;
        
        mostrarAlumnosAvanzado(alumnos, inicio, fin);
        
        printf("\nOpciones de navegacion:\n");
        printf("[N] Pagina siguiente | [P] Pagina anterior | [B] Buscar por nombre\n");
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
                if (pagina + 1 < paginasTotales) {
                    pagina++;
                } else {
                    printf("Ya esta en la ultima pagina.\n");
                }
                break;
                
            case 'p': case 'P':
                if (pagina > 0) {
                    pagina--;
                } else {
                    printf("Ya esta en la primera pagina.\n");
                }
                break;
                
            case 'b': case 'B': {
                char nombre[100];
                printf("Ingrese nombre para buscar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
                
                printf("\nResultados de busqueda para '%s':\n", nombre);
                
                int encontrados = 0;
                for (int i = 0; i < total; i++) {
                    if (strcasestr(alumnos[i].nombre, nombre) != NULL) {
                        visualizarAlumno(alumnos[i], 0); // Usar la funcion comun
                        encontrados++;
                    }
                }
                
                if (encontrados == 0) {
                    printf("No se encontraron alumnos con ese nombre.\n");
                } else {
                    printf("Total encontrados: %d\n", encontrados);
                }
                
                printf("Presione Enter para continuar...");
                fgets(buffer, sizeof(buffer), stdin);
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
                
                printf("Presione Enter para continuar...");
                fgets(buffer, sizeof(buffer), stdin);
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
                
                printf("Presione Enter para continuar...");
                fgets(buffer, sizeof(buffer), stdin);
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

// Funcion para mostrar materias con opciones adicionales
static void mostrarMateriasAvanzado(void* datos, int inicio, int fin) {
    NodoMateria** lista = (NodoMateria**)datos;
    NodoMateria* cabeza = *lista;
    
    int cantidadMaterias = 0;
    NodoMateria** materiasArray = obtenerMateriasEnRango(cabeza, inicio, fin, &cantidadMaterias);
    
    if (!materiasArray) {
        return;
    }
    
    // Mostrar encabezado de tabla
    mostrarEncabezadoTablaMaterias();
    
    // Mostrar las materias en el orden de la lista
    for (int j = 0; j < cantidadMaterias; j++) {
        visualizarMateria(materiasArray[j]->datos, 1); // 1 = formato avanzado
    }
    
    free(materiasArray);
}



// Funcion para paginado avanzado de materias
void listarMateriasAvanzado(NodoMateria* cabeza) {
    if (!cabeza) {
        printf("No hay materias para mostrar.\n");
        return;
    }
    
    int total = contarNodosMaterias(cabeza);
    int pagina = 0;
    char opcion;
    int itemsPorPagina = PAGINADO_TAM;
    int paginasTotales = (total + itemsPorPagina - 1) / itemsPorPagina;
    
    do {
        printf("\n--- Listado de Materias (Pagina %d de %d) ---\n", pagina + 1, paginasTotales);
        
        int inicio = pagina * itemsPorPagina;
        int fin = inicio + itemsPorPagina;
        if (fin > total) fin = total;
        
        mostrarMateriasAvanzado(&cabeza, inicio, fin);
        
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
                if (pagina + 1 < paginasTotales) {
                    pagina++;
                } else {
                    printf("Ya esta en la ultima pagina.\n");
                }
                break;
                
            case 'p': case 'P':
                if (pagina > 0) {
                    pagina--;
                } else {
                    printf("Ya esta en la primera pagina.\n");
                }
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
                
                printf("Presione Enter para continuar...");
                fgets(buffer, sizeof(buffer), stdin);
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
                    
                    // Mostrar correlativas si tiene
                    if (materiaEncontrada->datos.cantidadCorrelativas > 0) {
                        printf("Correlativas: ");
                        for (int i = 0; i < materiaEncontrada->datos.cantidadCorrelativas; i++) {
                            printf("%d ", materiaEncontrada->datos.correlativas[i]);
                        }
                        printf("\n");
                    } else {
                        printf("No tiene correlativas.\n");
                    }
                } else {
                    printf("No se encontro ninguna materia con ID %d.\n", id);
                }
                
                printf("Presione Enter para continuar...");
                fgets(buffer, sizeof(buffer), stdin);
                break;
            }
            
            case 'a': case 'A': {
                // Esta funcionalidad requeriria copiar todas las materias a un array,
                // ordenarlas y mostrarlas. Por ahora solo mostramos un mensaje.
                printf("Funcionalidad de ordenamiento por cantidad de alumnos pendiente.\n");
                printf("Presione Enter para continuar...");
                fgets(buffer, sizeof(buffer), stdin);
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

// Funcion auxiliar para obtener un array de materias en un rango
static NodoMateria** obtenerMateriasEnRango(NodoMateria* cabeza, int inicio, int fin, int* cantidadObtenida) {
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

// Funcion para mostrar encabezado de tabla de alumnos
static void mostrarEncabezadoTablaAlumnos() {
    printf("\n%-5s | %-30s | %-5s | %-20s\n", "ID", "Nombre", "Edad", "Materias inscriptas");
    printf("----------------------------------------------------------------------\n");
}

// Funcion para mostrar encabezado de tabla de materias
static void mostrarEncabezadoTablaMaterias() {
    printf("\n%-5s | %-30s | %-20s\n", "ID", "Nombre", "Cantidad de Alumnos");
    printf("--------------------------------------------------------------\n");
}

