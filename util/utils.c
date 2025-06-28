#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif
#include "utils.h"
#include "../config/config.h"

// FUNCIONES AUXILIARES PARA EVITAR REPETICION DE CODIGO

// Funciones para entrada de datos
int pedirInt(const char* mensaje) {
    if (!mensaje) {
        mensaje = "Ingrese un numero: "; // Mensaje predeterminado si es NULL
    }
    
    int numero;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d", &numero) == 1) {
            getchar(); // Consumir '\n' despues del numero
            return numero;
        }
        printf("Entrada invalida. Intente de nuevo.\n");
        while (getchar() != '\n'); // Limpiar el buffer
    }
}

float pedirFloat(const char* mensaje) {
    if (!mensaje) {
        mensaje = "Ingrese un numero: "; // Mensaje predeterminado si es NULL
    }
    
    float valor;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%f", &valor) == 1) {
            getchar(); // limpiar '\n'
            return valor;
        }
        printf("Entrada invalida. Ingrese solo numeros.\n");
        while (getchar() != '\n'); // limpiar buffer
    }
}

int esSoloLetrasYEspacios(const char* str) {
    if (!str) {
        return 0; // Si str es NULL, no es valido
    }
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            return 0; // contiene otro caracter
        }
    }
    return 1; // valido
}

void pedirString(const char* mensaje, char* buffer, int tamanio) {
    if (!buffer || tamanio <= 0) {
        return; // No podemos proceder sin un buffer valido
    }
    
    if (!mensaje) {
        mensaje = "Ingrese un texto: "; // Mensaje predeterminado si es NULL
    }
    
    while(1) {
        printf("%s", mensaje);
        if (fgets(buffer, tamanio, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0'; // Eliminar '\n'
            if (buffer[0] != '\0' && esSoloLetrasYEspacios(buffer)) {
                break;
            }    
        } 
        printf("Entrada invalida. Ingrese solo letras y espacios\n");
    }
}

int edadValida(int edad) {
    return (edad >= EDAD_MINIMA && edad <= EDAD_MAXIMA);
}

// Funcion para buscar subcadenas sin distinguir mayusculas/minusculas
char* strcasestr(const char* haystack, const char* needle) {
    if (!haystack || !needle) return NULL;
    
    size_t haystack_len = strlen(haystack);
    size_t needle_len = strlen(needle);
    
    if (haystack_len < needle_len) return NULL;
    
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        if (strncasecmp(haystack + i, needle, needle_len) == 0) {
            return (char*)(haystack + i);
        }
    }
    
    return NULL;
}

// Funciones de interfaz de usuario
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    printf("\nPresione Enter para continuar...");
    // Limpiar el buffer de entrada y esperar por Enter
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void mostrarFechaActual(char* dia, char* fecha) {
    // Obtener fecha y dia actual
    time_t ahora = time(NULL);
    struct tm *tiempo_local = localtime(&ahora);
    
    // Formato de fecha: DD/MM/AAAA HH:MM:SS
    strftime(fecha, 30, "%d/%m/%Y, %H:%M", tiempo_local);
    
    // Obtener el nombre del dia en español
    char *dias_semana[] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
    strcpy(dia, dias_semana[tiempo_local->tm_wday]);
}

// Función auxiliar para crear el directorio de datos
void crearDirectorioData() {
    #ifdef _WIN32
        mkdir("./data");
    #else
        mkdir("./data", 0777);
    #endif
}
