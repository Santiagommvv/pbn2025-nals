#ifndef UTILS_H
#define UTILS_H

int pedirInt(const char* mensaje);
float pedirFloat(const char* mensaje);
void pedirString(const char* mensaje, char* buffer, int tamanio);
int esSoloLetrasYEspacios(const char* str);
int edadValida(int edad);


#endif // UTILS_H