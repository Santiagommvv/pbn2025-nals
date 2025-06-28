// INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/config.h"
#include "lista_materia.h"
#include "../datast/utils.h"

// 2: PROTOTIPOS DE FUNCIONES
static void capitalizarPrimeraLetra(char* texto);
static void obtenerNombreCapitalizado(const char* nombre, char* buffer, size_t tamano);
static void formatearCorrelativasComoString(Materia m, char* buffer, size_t tamano);
static void mostrarCorrelativa(int idCorrelativa, NodoMateria* cabeza, int conIndice, int indice);
static void agregarNodoAlFinal(NodoMateria** cabeza, NodoMateria* nuevoNodo);

// 3: VARIABLES GLOBALES
static int ultimoIDMateria = 0;
// Variable global para almacenar la lista de materias (para referencias internas)
static NodoMateria* listaMateriaGlobal = NULL;

// FUNCIONES AUXILIARES

// Funcion para capitalizar la primera letra de una cadena
static void capitalizarPrimeraLetra(char* texto) {
    if (texto && *texto) {
        *texto = toupper((unsigned char)*texto);
    }
}

// Funcion auxiliar para obtener nombre capitalizado
static void obtenerNombreCapitalizado(const char* nombre, char* buffer, size_t tamano) {
    if (!nombre || !buffer || tamano <= 0) return;
    
    strncpy(buffer, nombre, tamano - 1);
    buffer[tamano - 1] = '\0';
    capitalizarPrimeraLetra(buffer);
}

// Función auxiliar para formatear correlativas como string
static void formatearCorrelativasComoString(Materia m, char* buffer, size_t tamano) {
    if (!buffer || tamano <= 0) return;
    
    buffer[0] = '\0'; // Inicializar la cadena vacía
    
    if (m.cantidadCorrelativas > 0) {
        int pos = 0;
        for (int i = 0; i < m.cantidadCorrelativas && pos < tamano - 1; i++) {
            int idCorrelativa = m.correlativas[i];
            NodoMateria* materiaCorrelativa = buscarMateriaPorID(NULL, idCorrelativa);
            
            if (materiaCorrelativa) {
                char nombreCorrelativaCapitalizado[100];
                obtenerNombreCapitalizado(materiaCorrelativa->datos.nombre, 
                                        nombreCorrelativaCapitalizado, 
                                        sizeof(nombreCorrelativaCapitalizado));
                
                pos += snprintf(buffer + pos, tamano - pos, "%s %d", 
                                nombreCorrelativaCapitalizado, idCorrelativa);
            } else {
                pos += snprintf(buffer + pos, tamano - pos, "(%d)", idCorrelativa);
            }
            
            if (i < m.cantidadCorrelativas - 1 && pos < tamano - 2) {
                pos += snprintf(buffer + pos, tamano - pos, ", ");
            }
        }
    } else if (m.id >= ID_MATERIAS_AVANZADAS) {
        snprintf(buffer, tamano, "*Todas las materias anteriores*");
    } else {
        snprintf(buffer, tamano, "Ninguna");
    }
}

// Función auxiliar para mostrar una correlativa
static void mostrarCorrelativa(int idCorrelativa, NodoMateria* cabeza, int conIndice, int indice) {
    NodoMateria* correlativa = buscarMateriaPorID(cabeza, idCorrelativa);
    
    if (conIndice) {
        printf("  %d. ", indice);
    } else {
        printf("    - ");
    }
    
    if (correlativa) {
        char nombreCorrelativaCapitalizado[100];
        obtenerNombreCapitalizado(correlativa->datos.nombre, 
                                nombreCorrelativaCapitalizado, 
                                sizeof(nombreCorrelativaCapitalizado));
        
        printf("%s %d", nombreCorrelativaCapitalizado, idCorrelativa);
    } else {
        printf("%d", idCorrelativa);
        if (conIndice) printf(" (Materia no encontrada)");
    }
    
    printf("\n");
}
// Función auxiliar para agregar un nodo al final de la lista
static void agregarNodoAlFinal(NodoMateria** cabeza, NodoMateria* nuevoNodo) {
    if (!cabeza || !nuevoNodo) return;
    
    if (*cabeza == NULL) {
        // Lista vacía, el nuevo nodo es la cabeza
        *cabeza = nuevoNodo;
    } else {
        // Encontrar el último nodo
        NodoMateria* ultimo = *cabeza;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        // Agregar el nuevo nodo al final
        ultimo->siguiente = nuevoNodo;
    }
}
// Funcion para actualizar la referencia global
void actualizarReferenciaMaterias(NodoMateria* lista) {
    listaMateriaGlobal = lista;
}

// 4: FUNCIONES BASICAS DE LISTA

NodoMateria* agregarMateria(NodoMateria** cabeza, const char* nombre) {
    if (!cabeza) {
        printf("Error: Puntero a cabeza es NULL\n");
        return NULL;
    }
    
    if (!nombre || nombre[0] == '\0') {
        printf("Error: El nombre de la materia no puede ser NULL o vacio\n");
        return NULL;
    }

    NodoMateria* nodo = malloc(sizeof(NodoMateria));
    if(!nodo){
        return NULL;
    }

    nodo->datos.id = ++ultimoIDMateria;
    
    strncpy(nodo->datos.nombre,nombre,sizeof(nodo->datos.nombre));
    nodo->datos.nombre[sizeof(nodo->datos.nombre) -1] = '\0';

    nodo->datos.cantidadAlumnos = 0;
    nodo->datos.cantidadCorrelativas = 0;
    nodo->siguiente = NULL;
    
    // Agregar el nodo al final de la lista para mantener el orden de IDs ascendente
    agregarNodoAlFinal(cabeza, nodo);
    
    // Actualizar la referencia global
    actualizarReferenciaMaterias(*cabeza);

    return nodo;
}

int eliminarMateria(NodoMateria** cabeza, int id){
    if (!cabeza || !(*cabeza)) {
        printf("Error: No hay materias para eliminar\n");
        return 0;
    }
    
    NodoMateria* actual = *cabeza;
    NodoMateria* anterior = NULL;

    while(actual) {
        if(actual->datos.id == id) {
            if(anterior){
                anterior->siguiente = actual->siguiente;
            } else{
                *cabeza = actual->siguiente;
            }
            free(actual);
            printf("Materia con ID %d eliminada.\n", id);
            
            // Actualizar la referencia global despues de eliminar
            actualizarReferenciaMaterias(*cabeza);
            
            return 1;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Materia con ID %d no encontrada\n", id);
    return 0;
}

// liberar la memoria ocupada por la lista
void liberarListaMaterias(NodoMateria* cabeza) {
    while (cabeza) {
        NodoMateria* tmp = cabeza;
        cabeza = cabeza->siguiente;
        free(tmp);
    }
}
// contar el numero de materias en la lista
int contarNodosMaterias(NodoMateria* cabeza) {
    int count = 0;
    while (cabeza) {
        count++;
        cabeza = cabeza->siguiente;
    }
    return count;
}

// 5: FUNCIONES DE BUSQUEDA

NodoMateria* buscarMateriaPorID(NodoMateria* cabeza, int id){
    // Si cabeza es NULL, usar la referencia global
    if (!cabeza) {
        cabeza = listaMateriaGlobal;
    }
    
    while(cabeza) {
        if(cabeza->datos.id == id){
            return cabeza;
        }
        cabeza = cabeza->siguiente;
    }
    return NULL;
}

NodoMateria* buscarMateriaPorNombre(NodoMateria* cabeza, const char* nombre){
    if (!nombre) {
        printf("Error: nombre no puede ser NULL\n");
        return NULL;
    }
    
    while(cabeza){
        if(strcasecmp(cabeza->datos.nombre, nombre) == 0){
            return cabeza;
        }
        cabeza = cabeza->siguiente;
    }
    return NULL;
}

// 6: FUNCIONES DE VISUALIZACION

void listarMaterias(NodoMateria* cabeza) {
    if(!cabeza) {
        printf("No hay materias para mostrar");
        return;
    }

    printf("Listado de materias:\n");
    while(cabeza){
        visualizarMateria(cabeza->datos, 0); // Usar la funcion comun
        cabeza = cabeza->siguiente;
    }
}

// Funcion comun para visualizar una materia con formato
void visualizarMateria(Materia m, int formatoAvanzado) {
    char nombreCapitalizado[100];
    obtenerNombreCapitalizado(m.nombre, nombreCapitalizado, sizeof(nombreCapitalizado));
    
    if (formatoAvanzado) {
        // Formatear las correlativas como string
        char correlativas[200];
        formatearCorrelativasComoString(m, correlativas, sizeof(correlativas));
        
        printf("%-5d | %-30s | %-20d | %-40s\n",
                m.id, nombreCapitalizado, m.cantidadAlumnos, correlativas);
    } else {
        printf("ID: %d | Nombre: %s | Cantidad de Alumnos: %d", 
                m.id, nombreCapitalizado, m.cantidadAlumnos);
        
        // Mostrar correlativas si las tiene
        if (m.cantidadCorrelativas > 0) {
            printf(" | Correlativas: \n");
            for (int i = 0; i < m.cantidadCorrelativas; i++) {
                mostrarCorrelativa(m.correlativas[i], NULL, 0, 0);
            }
        }
        
        // Mostrar regla especial para materias avanzadas
        if (m.id >= ID_MATERIAS_AVANZADAS) {
            printf(" | *Requiere todas las materias anteriores*\n");
        } else if (m.cantidadCorrelativas == 0) {
            printf("\n");
        }
    }
}

// 7: FUNCIONES DE MODIFICACION

int modificarMateria(NodoMateria* cabeza, int id){
    if (!cabeza) {
        printf("Error: No hay materias para modificar\n");
        return 0;
    }
    
    while(cabeza) {
        if(cabeza->datos.id == id){
            pedirString("Ingrese nuevo nombre para la materia: ", cabeza->datos.nombre, MAX_NOMBRE);
            return 1;
        }
        cabeza = cabeza->siguiente;
    }
    printf("Materia con ID %d no encontrada\n", id);
    return 0;
}

// Funcion para agregar o modificar correlatividades de una materia
int modificarCorrelativasMateria(NodoMateria* cabeza, int id) {
    if (!cabeza) {
        printf("Error: No hay materias para modificar\n");
        return 0;
    }
    
    NodoMateria* materia = buscarMateriaPorID(cabeza, id);
    if (!materia) {
        printf("Error: Materia con ID %d no encontrada\n", id);
        return 0;
    }
    
    // Mostrar información actual de la materia
    char nombreCapitalizado[100];
    obtenerNombreCapitalizado(materia->datos.nombre, nombreCapitalizado, sizeof(nombreCapitalizado));
    
    printf("Materia: %s (ID: %d)\n", nombreCapitalizado, materia->datos.id);
    
    // Mostrar correlativas actuales
    printf("Correlativas actuales: \n");
    if (materia->datos.cantidadCorrelativas > 0) {
        for (int i = 0; i < materia->datos.cantidadCorrelativas; i++) {
            mostrarCorrelativa(materia->datos.correlativas[i], cabeza, 1, i+1);
        }
    } else {
        printf("  Ninguna\n");
    }
    
    // Regla especial para materias avanzadas
    if (materia->datos.id >= ID_MATERIAS_AVANZADAS) {
        printf("NOTA: Esta materia (ID >= %d) requiere tener aprobadas todas las materias anteriores.\n", ID_MATERIAS_AVANZADAS);
    }
    
    // Preguntar si quiere modificar las correlativas
    char opcion[2];
    pedirString("¿Desea modificar las correlativas? (S/N): ", opcion, 2);
    if (opcion[0] != 'S' && opcion[0] != 's') {
        printf("Operacion cancelada.\n");
        return 0;
    }
    
    // Reiniciar correlativas
    materia->datos.cantidadCorrelativas = 0;
    
    // Solicitar nuevas correlativas
    printf("Ingrese las correlativas (IDs) separadas por espacios (0 para terminar):\n");
    int idCorrelativa;
    
    while (1) {
        printf("Correlativa %d: ", materia->datos.cantidadCorrelativas + 1);
        scanf("%d", &idCorrelativa);
        getchar(); // Consumir el salto de linea
        
        if (idCorrelativa == 0) {
            break; // Terminar ingreso de correlativas
        }
        
        // Validar que la correlativa existe
        NodoMateria* correlativa = buscarMateriaPorID(cabeza, idCorrelativa);
        if (!correlativa) {
            printf("Error: No existe una materia con ID %d. Intente de nuevo.\n", idCorrelativa);
            continue;
        }
        
        // Validar que no se esté agregando a sí misma como correlativa
        if (idCorrelativa == materia->datos.id) {
            printf("Error: Una materia no puede ser correlativa de si misma.\n");
            continue;
        }
        
        // Agregar la correlativa
        if (materia->datos.cantidadCorrelativas < MAX_CORRELATIVAS) {
            materia->datos.correlativas[materia->datos.cantidadCorrelativas++] = idCorrelativa;
            printf("Correlativa %s (ID: %d) agregada.\n", correlativa->datos.nombre, idCorrelativa);
        } else {
            printf("Error: Se alcanzo el maximo de correlativas permitidas (%d).\n", MAX_CORRELATIVAS);
            break;
        }
    }
    
    printf("Correlativas actualizadas correctamente.\n");
    return 1;
}