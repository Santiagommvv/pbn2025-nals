#ifndef UTILS_H
#define UTILS_H

#include <time.h>

// Funciones de entrada/salida
int pedirInt(const char* mensaje);
float pedirFloat(const char* mensaje);
void pedirString(const char* mensaje, char* buffer, int tamanio);
int esSoloLetrasYEspacios(const char* str);
int edadValida(int edad);
char* strcasestr(const char* haystack, const char* needle);

// Funciones de interfaz de usuario
void limpiarPantalla();
void pausar();
void mostrarFechaActual(char* dia, char* fecha);

// Función para crear directorio de datos
void crearDirectorioData();

#endif // UTILS_H