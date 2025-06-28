#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lista_materia.h"
#include "../datast/utils.h"

static int ultimoIDMateria = 0;

// Variable global para almacenar la lista de materias (para referencias internas)
static NodoMateria* listaMateriaGlobal = NULL;

// Funcion para actualizar la referencia global
void actualizarReferenciaMaterias(NodoMateria* lista) {
    listaMateriaGlobal = lista;
}

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
    if (*cabeza == NULL) {
        // Lista vacia, el nuevo nodo es la cabeza
        *cabeza = nodo;
    } else {
        // Encontrar el ultimo nodo
        NodoMateria* ultimo = *cabeza;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        // Agregar el nuevo nodo al final
        ultimo->siguiente = nodo;
    }
    
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

void liberarListaMaterias(NodoMateria* cabeza) {
    while (cabeza) {
        NodoMateria* tmp = cabeza;
        cabeza = cabeza->siguiente;
        free(tmp);
    }
}

int contarNodosMaterias(NodoMateria* cabeza) {
    int count = 0;
    while (cabeza) {
        count++;
        cabeza = cabeza->siguiente;
    }
    return count;
}

// Funcion para capitalizar la primera letra de una cadena
static void capitalizarPrimeraLetra(char* texto) {
    if (texto && *texto) {
        *texto = toupper((unsigned char)*texto);
    }
}

// Funcion comun para visualizar una materia con formato
void visualizarMateria(Materia m, int formatoAvanzado) {
    // Crear copia del nombre para capitalizar la primera letra
    char nombreCapitalizado[100];
    strncpy(nombreCapitalizado, m.nombre, sizeof(nombreCapitalizado) - 1);
    nombreCapitalizado[sizeof(nombreCapitalizado) - 1] = '\0';
    capitalizarPrimeraLetra(nombreCapitalizado);
    
    if (formatoAvanzado) {
        // Formatear las correlativas como string
        char correlativas[200] = "";
        if (m.cantidadCorrelativas > 0) {
            int pos = 0;
            for (int i = 0; i < m.cantidadCorrelativas && pos < 199; i++) {
                int idCorrelativa = m.correlativas[i];
                // Buscar la materia correlativa para mostrar su nombre
                NodoMateria* materiaCorrelativa = buscarMateriaPorID(NULL, idCorrelativa);
                
                if (materiaCorrelativa) {
                    // Copiar y capitalizar el nombre de la correlativa
                    char nombreCorrelativaCapitalizado[100];
                    strncpy(nombreCorrelativaCapitalizado, materiaCorrelativa->datos.nombre, sizeof(nombreCorrelativaCapitalizado) - 1);
                    nombreCorrelativaCapitalizado[sizeof(nombreCorrelativaCapitalizado) - 1] = '\0';
                    capitalizarPrimeraLetra(nombreCorrelativaCapitalizado);
                    
                    pos += snprintf(correlativas + pos, 199 - pos, "%s %d", 
                                    nombreCorrelativaCapitalizado, idCorrelativa);
                } else {
                    // Si no se encuentra la materia, mostrar solo el ID
                    pos += snprintf(correlativas + pos, 199 - pos, "(%d)", idCorrelativa);
                }
                
                if (i < m.cantidadCorrelativas - 1 && pos < 198) {
                    pos += snprintf(correlativas + pos, 199 - pos, ", ");
                }
            }
        } else if (m.id >= ID_MATERIAS_AVANZADAS) {
            snprintf(correlativas, 199, "*Todas las materias anteriores*");
        } else {
            snprintf(correlativas, 199, "Ninguna");
        }
        
        printf("%-5d | %-30s | %-20d | %-40s\n",
               m.id, nombreCapitalizado, m.cantidadAlumnos, correlativas);
    } else {
        printf("ID: %d | Nombre: %s | Cantidad de Alumnos: %d", 
               m.id, nombreCapitalizado, m.cantidadAlumnos);
        
        // Mostrar correlativas si las tiene
        if (m.cantidadCorrelativas > 0) {
            printf(" | Correlativas: \n");
            for (int i = 0; i < m.cantidadCorrelativas; i++) {
                int idCorrelativa = m.correlativas[i];
                NodoMateria* materiaCorrelativa = buscarMateriaPorID(NULL, idCorrelativa);
                
                printf("    - ");
                if (materiaCorrelativa) {
                    // Copiar y capitalizar el nombre de la correlativa
                    char nombreCorrelativaCapitalizado[100];
                    strncpy(nombreCorrelativaCapitalizado, materiaCorrelativa->datos.nombre, sizeof(nombreCorrelativaCapitalizado) - 1);
                    nombreCorrelativaCapitalizado[sizeof(nombreCorrelativaCapitalizado) - 1] = '\0';
                    capitalizarPrimeraLetra(nombreCorrelativaCapitalizado);
                    
                    printf("%s %d", nombreCorrelativaCapitalizado, idCorrelativa);
                } else {
                    printf("%d", idCorrelativa);
                }
                
                printf("\n");
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
    
    // Mostrar informacion actual de la materia
    char nombreCapitalizado[100];
    strncpy(nombreCapitalizado, materia->datos.nombre, sizeof(nombreCapitalizado) - 1);
    nombreCapitalizado[sizeof(nombreCapitalizado) - 1] = '\0';
    capitalizarPrimeraLetra(nombreCapitalizado);
    
    printf("Materia: %s (ID: %d)\n", nombreCapitalizado, materia->datos.id);
    
    // Mostrar correlativas actuales
    printf("Correlativas actuales: \n");
    if (materia->datos.cantidadCorrelativas > 0) {
        for (int i = 0; i < materia->datos.cantidadCorrelativas; i++) {
            int idCorrelativa = materia->datos.correlativas[i];
            NodoMateria* correlativa = buscarMateriaPorID(cabeza, idCorrelativa);
            
            printf("  %d. ", i+1);
            if (correlativa) {
                char nombreCorrelativaCapitalizado[100];
                strncpy(nombreCorrelativaCapitalizado, correlativa->datos.nombre, sizeof(nombreCorrelativaCapitalizado) - 1);
                nombreCorrelativaCapitalizado[sizeof(nombreCorrelativaCapitalizado) - 1] = '\0';
                capitalizarPrimeraLetra(nombreCorrelativaCapitalizado);
                
                printf("%s %d\n", nombreCorrelativaCapitalizado, idCorrelativa);
            } else {
                printf("%d (Materia no encontrada)\n", idCorrelativa);
            }
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
        
        // Validar que no se este agregando a si misma como correlativa
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