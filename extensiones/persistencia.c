// 1: INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "../config/config.h"
#include <sys/stat.h>
#include "../dominio/lista_materia.h"

// 2: FUNCIONES AUXILIARES

// Verifica si existen archivos de datos guardados de una sesión anterior
int datosGuardadosDisponibles() {
    struct stat buffer1, buffer2;
    return stat(ALUMNOS_CSV, &buffer1) == 0 && stat(MATERIAS_CSV, &buffer2) == 0;
}


// Divide una cadena en tokens usando el delimitador especificado
static void split(char* s, char* delim, char*** out, int* count) {
    if (!s || !delim || !out || !count) return;
    
    *count = 0; 
    *out = NULL;
    char* tok = strtok(s, delim);
    while(tok) {
        *out = realloc(*out, (*count + 1) * sizeof(char*));
        if (!*out) {
            *count = 0;
            return; // Error de memoria
        }
        (*out)[(*count)++] = tok;
        tok = strtok(NULL, delim);
    }
}

/**
 * Normaliza un nombre para usar en CSV
 */
static void normalizarNombre(char* destino, const char* origen, size_t tam) {
    strncpy(destino, origen, tam - 1);
    destino[tam - 1] = '\0';
}
// 3: FUNCIONES DE GUARDADO

static void guardarAlumnosCSV(NodoAVL* a, FILE* f) {
    if (!a || !f) return;
    
    guardarAlumnosCSV(a->izq, f);
    
    Alumno al = a->alumno;
    // Formato CSV para Excel: sin espacios después de las comas
    fprintf(f, "%d,%s,%s,%d,", al.id, al.apellido, al.nombre, al.edad);

    // Materias inscriptas (en una sola celda, separadas por punto y coma)
    for(int i = 0; i < al.cantidadDeMateriasInscripto; i++) {
        fprintf(f, "%d", al.materiasInscripto[i]);
        if(i < al.cantidadDeMateriasInscripto - 1) 
            fputc(SEP_LISTA[0], f);
    }
    fputc(',', f);

    // Materias rendidas
    for(int i = 0; i < al.cantidadMateriasRendidas; i++) {
        MateriaRendida mr = al.materiasRendidas[i];
        fprintf(f, "%d:%0.2f:%d", mr.IDMateria, mr.nota, mr.aprobo);
        if(i < al.cantidadMateriasRendidas - 1) 
            fputc(SEP_LISTA[0], f);
    }

    fputc('\n', f);
    
    guardarAlumnosCSV(a->der, f);
}

static void guardarMateriasCSV(NodoMateria* m, FILE* f) {
    if (!f) return;
    
    while(m) {
        Materia mat = m->datos;
        // Formato CSV para Excel: sin espacios después de las comas
        fprintf(f, "%d,%s,", mat.id, mat.nombre);
        
        // Alumnos inscriptos
        for(int i = 0; i < mat.cantidadAlumnos; i++) {
            fprintf(f, "%d", mat.alumnosInscriptos[i]);
            if(i < mat.cantidadAlumnos - 1) 
                fputc(SEP_LISTA[0], f);
        }
        fputc('\n', f);
        
        m = m->siguiente;
    }
}

void guardarPlanEstudiosCSV(NodoMateria* lista) {
    if (!lista) return;
    
    FILE* f = fopen(PLAN_ESTUDIOS, "w");
    if (!f) {
        printf("Error: No se pudo abrir el archivo %s para escritura\n", PLAN_ESTUDIOS);
        return;
    }
    
    // Escribir encabezado
    fprintf(f, "codigo,nombre,correlativa1,correlativa2\n");
    
    // Recorrer la lista de materias y guardar cada una con sus correlatividades
    NodoMateria* actual = lista;
    while (actual) {
        // Normalizar el nombre de la materia antes de guardarla
        char nombreNormalizado[100];
        normalizarNombre(nombreNormalizado, actual->datos.nombre, sizeof(nombreNormalizado));
        
        // Escribir codigo y nombre
        fprintf(f, "%d,%s", actual->datos.id, nombreNormalizado);
        
        // Escribir correlatividades
        for (int i = 0; i < actual->datos.cantidadCorrelativas; i++) {
            fprintf(f, ",%d", actual->datos.correlativas[i]);
        }
        
        fprintf(f, "\n");
        actual = actual->siguiente;
    }
    
    fclose(f);
    printf("Plan de estudios guardado correctamente en %s.\n", PLAN_ESTUDIOS);
}

// Función principal para guardar todos los datos
void guardarDatos(NodoAVL* alumnos, NodoMateria* materias) {
    // Abrir archivos
    FILE* fa = fopen(ALUMNOS_CSV, "w");
    FILE* fm = fopen(MATERIAS_CSV, "w");
    if(!fa || !fm) {
        puts("Error abriendo archivos para guardar");
        if(fa) fclose(fa);
        if(fm) fclose(fm);
        return;
    }
    
    // Escribir encabezados para el archivo de alumnos
    fprintf(fa, "ID,Apellido,Nombre,Edad,Materias Inscriptas,Materias Rendidas\n");
    
    // Escribir encabezados para el archivo de materias
    fprintf(fm, "ID,Nombre,Alumnos Inscriptos\n");

    // Guardar datos
    guardarAlumnosCSV(alumnos, fa);
    guardarMateriasCSV(materias, fm);

    // Cerrar archivos
    fclose(fa); 
    fclose(fm);
    
    // Guardar tambien el plan de estudios con correlatividades
    guardarPlanEstudiosCSV(materias);
}

// 4: FUNCIONES DE CARGA DE DATOS
// Carga los alumnos desde un archivo CSV
static void cargarAlumnosCSV(NodoAVL** lista, FILE* f) {
    if (!lista || !f) return;
    
    char linea[512];
    
    // Leer y descartar la linea de encabezado
    if (fgets(linea, sizeof(linea), f) == NULL) {
        return; // Archivo vacio o error
    }
    
    // Procesar cada línea
    while(fgets(linea, sizeof(linea), f)) {
        // Inicializar alumno y eliminar salto de línea
        Alumno al = {0};
        char *ptr = strchr(linea, '\n'); 
        if(ptr) *ptr = '\0';

        // Separar campos
        char* campos[5];
        int i = 0; 
        char* tok = strtok(linea, ",");
        while(tok && i < 5) { 
            campos[i++] = tok; 
            tok = strtok(NULL, ","); 
        }
        if(i < 4) continue; // Necesitamos al menos ID, apellido, nombre y edad

        // Datos básicos
        al.id = atoi(campos[0]);
        normalizarNombre(al.apellido, campos[1], sizeof(al.apellido));
        normalizarNombre(al.nombre, campos[2], sizeof(al.nombre));
        al.edad = atoi(campos[3]);

        // Materias inscriptas
        al.cantidadDeMateriasInscripto = 0;
        if(i > 4 && strlen(campos[4])) {
            char** ids = NULL; 
            int n = 0; 
            split(campos[4], SEP_LISTA, &ids, &n);
            
            for(int k = 0; k < n; k++) 
                al.materiasInscripto[al.cantidadDeMateriasInscripto++] = atoi(ids[k]);
            
            free(ids);
        }

        // Materias rendidas
        al.cantidadMateriasRendidas = 0;
        if(tok && strlen(tok)) {
            char** rens = NULL;
            int n = 0;
            split(tok, SEP_LISTA, &rens, &n);
            
            for(int k = 0; k < n; k++) {
                char* p1 = strtok(rens[k], SEP_RENDIDA);
                char* p2 = strtok(NULL, SEP_RENDIDA);
                char* p3 = strtok(NULL, SEP_RENDIDA);
                
                if(p1 && p2 && p3) {
                    al.materiasRendidas[al.cantidadMateriasRendidas].IDMateria = atoi(p1);
                    al.materiasRendidas[al.cantidadMateriasRendidas].nota = atof(p2);
                    al.materiasRendidas[al.cantidadMateriasRendidas++].aprobo = atoi(p3);
                }
            }
            free(rens);
        }
        
        // Insertar alumno en el árbol
        *lista = insertarAVL(*lista, al);
    }
}

// Carga las materias desde un archivo CSV
static void cargarMateriasCSV(NodoMateria** lista, FILE* f) {
    if (!lista || !f) return;
    
    char linea[256];
    
    // Leer y descartar la linea de encabezado
    if (fgets(linea, sizeof(linea), f) == NULL) {
        return; // Archivo vacio o error
    }
    
    // Procesar cada línea
    while(fgets(linea, sizeof(linea), f)) {
        // Inicializar materia y eliminar salto de línea
        Materia m = {0};
        char* nl = strchr(linea, '\n'); 
        if(nl) *nl = '\0';

        // Separar campos
        char* id = strtok(linea, ",");
        char* nom = strtok(NULL, ",");
        char* ins = strtok(NULL, ",");

        // Verificar campos mínimos
        if(!id || !nom) continue;
        
        // Datos básicos
        m.id = atoi(id);
        normalizarNombre(m.nombre, nom, sizeof(m.nombre));

        // Alumnos inscriptos
        m.cantidadAlumnos = 0;
        if(ins && strlen(ins)) {
            char** ids = NULL; 
            int n = 0; 
            split(ins, SEP_LISTA, &ids, &n);
            
            for(int k = 0; k < n; k++) 
                m.alumnosInscriptos[m.cantidadAlumnos++] = atoi(ids[k]);
            
            free(ids);
        }
        
        // Agregar materia a la lista
        NodoMateria* resultado = agregarMateria(lista, m.nombre);
        if (!resultado) {
            printf("Error: No se pudo agregar la materia %s\n", m.nombre);
        }
    }
}

// Carga el plan de estudios y establece las correlatividades
void cargarPlanEstudiosCSV(NodoMateria** lista) {
    if (!lista) return;
    
    // Abrir archivo
    FILE* f = fopen(PLAN_ESTUDIOS, "r");
    if (!f) {
        printf("Error: No se pudo abrir el archivo %s\n", PLAN_ESTUDIOS);
        return;
    }
    
    char linea[256];
    int materiasLeidas = 0;
    int correlativasAgregadas = 0;
    
    // Leer y descartar la linea de encabezado
    if (fgets(linea, sizeof(linea), f) == NULL) {
        fclose(f);
        return; // Archivo vacio o error
    }
    
    // Procesar cada línea
    while (fgets(linea, sizeof(linea), f)) {
        char* nl = strchr(linea, '\n'); 
        if (nl) *nl = '\0';
        
        // Tokenizar la línea
        char* tokens[MAX_CORRELATIVAS + 2]; // Código, nombre, y hasta MAX_CORRELATIVAS correlativas
        int numTokens = 0;
        
        // Separar por comas
        char* token = strtok(linea, ",");
        while (token && numTokens < MAX_CORRELATIVAS + 2) {
            tokens[numTokens++] = token;
            token = strtok(NULL, ",");
        }
        
        if (numTokens < 2) continue; // Necesitamos al menos codigo y nombre
        
        int codigo = atoi(tokens[0]);
        if (codigo <= 0) continue; // ID invalido
        
        // Normalizar el nombre de la materia
        char nombreNormalizado[100];
        normalizarNombre(nombreNormalizado, tokens[1], sizeof(nombreNormalizado));
        
        // Buscar la materia en la lista
        NodoMateria* materia = buscarMateriaPorID(*lista, codigo);
        if (!materia) {
            // La materia no existe, la creamos
            materia = agregarMateria(lista, nombreNormalizado);
            if (!materia) {
                printf("Error: No se pudo agregar la materia %s\n", nombreNormalizado);
                continue;
            }
        }
        
        materiasLeidas++;
        
        // Agregar correlatividades
        materia->datos.cantidadCorrelativas = 0; // Reiniciar correlativas
        
        for (int i = 2; i < numTokens; i++) {
            int idCorrelativa = atoi(tokens[i]);
            if (idCorrelativa > 0) {
                if (materia->datos.cantidadCorrelativas < MAX_CORRELATIVAS) {
                    materia->datos.correlativas[materia->datos.cantidadCorrelativas++] = idCorrelativa;
                    correlativasAgregadas++;
                } else {
                    printf("Advertencia: Materia %d tiene más correlativas que el máximo permitido (%d)\n", 
                            codigo, MAX_CORRELATIVAS);
                    break;
                }
            }
        }
    }
    
    fclose(f);
    
    // Mostrar estadísticas
    printf("Plan de estudios cargado: %d materias leídas, %d correlatividades agregadas.\n", 
            materiasLeidas, correlativasAgregadas);
    
    // Mostrar informacion sobre la regla especial
    printf("Regla especial: Materias con ID >= %d requieren tener aprobadas todas las materias anteriores.\n", 
            ID_MATERIAS_AVANZADAS);
}

// Función principal para cargar todos los datos
void cargarDatos(NodoAVL** alumnos, NodoMateria** materias) {
    if (!alumnos || !materias) return;
    
    // Cargar alumnos
    FILE* fa = fopen(ALUMNOS_CSV, "r");
    if(fa) { 
        cargarAlumnosCSV(alumnos, fa); 
        fclose(fa);
    }
    
    // Cargar materias
    FILE* fm = fopen(MATERIAS_CSV, "r");
    if(fm) { 
        cargarMateriasCSV(materias, fm); 
        fclose(fm);
    }
    
    // Cargar plan de estudios
    cargarPlanEstudiosCSV(materias);
}