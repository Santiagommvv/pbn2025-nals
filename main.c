#include <stdio.h>
#include "include/listaES.h"
#include "include/nodo.h"

extern Lista miLista;

int main() {
    int opcion;
    int valor;

    do {
        printf(
            "\nElije que desea realizar:\n"
            "1) agregar un numero a la lista\n"
            "2) eliminar un elemento de la lista\n"
            "3) buscar un elemento de la lista\n"
            "4) imprimir los elementos de la lista\n"
            "5) salir del programa\n\n");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1: {
            printf("\nIngrese el numero que desea agregar\n");
            scanf("%d", &valor);
            insertarAlInicio(&miLista.cabeza, valor);
            break;
        }
        case 2: {
            printf("Eliminar un elemento de la lista\n");
            scanf("%d", &valor);
            eliminarNodo(&miLista.cabeza, valor);
            break;
        }
        case 3: {
            printf("Buscar un elemento de la lista\n");
            scanf("%d", &valor);
            Nodo* encontrado = buscarNodo(miLista.cabeza, valor);
            if (encontrado) {
                printf("Elemento %d encontrado en la lista.\n", valor);
            } else {
                printf("Elemento %d no encontrado en la lista.\n", valor);
            }
            break;
        }
        case 4: {
            printf("Elementos de la lista:\n");
            imprimirLista(miLista.cabeza);
            break;
        }
        case 5: {
            int salir;
            printf(
                "\n¿Está seguro que desea salir del programa?\n"
                "1) SI\n"
                "2) NO\n");
            scanf("%d", &salir);
            if (salir == 1) {
                printf("Adios :)\n");
                return 0;
            } else if (salir == 2) {
                printf("\nVolviendo al menu principal\n");
                continue;
            } else {
                printf("\nOpción incorrecta, volviendo al menu principal\n");
            }
            break;
        }
        default:
            printf("\nOpción incorrecta, elija un número del 1 al 5\n");
            break;
        }
    } while (1);

    return 0;
}
