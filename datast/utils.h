#ifndef UTIL_H
#define UTIL_H

int pedirInt(const char* mensaje);
float pedirFloat(const char* mensaje);
void pedirString(const char* mensaje, char* buffer, int tamanio);
int esSoloLetrasYEspacios(const char* str);


#endif // UTIL_H