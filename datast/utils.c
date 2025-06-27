#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

int pedirInt(const char* mensaje) {
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
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            return 0; // contiene otro caracter
        }
    }
    return 1; // valido
}

void pedirString(const char* mensaje, char* buffer, int tamanio) {
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