#include <stdio.h>
#include "./src/listaES.c"
#include "./src/nodo.c"

int main() {
    int opcion;
    int valor;

    do {
        printf(
            "\nElije que desea realizar:\n"
            "1) Agregar un número a la lista\n"
            "2) Eliminar un elemento de la lista\n"
            "3) Buscar un elemento de la lista\n"
            "4) Imprimir los elementos de la lista\n"
            "5) Vaciar la lista\n"
            "6) Salir del programa\n\n");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                int decision = 0;
                int valor;

                while (decision != 3) {
                    printf("\n¿Dónde desea agregar el número?\n"
                        "1) Inicio\n"
                        "2) Final\n"
                        "3) Salir al menu\n");
                    scanf("%d", &decision);

                    if (decision == 1) {
                        printf("\nIngrese el número que desea agregar:\n");
                        scanf("%d", &valor);
                        insertarAlInicio(&miLista.cabeza, valor);
                        break;
                    } else if (decision == 2) {
                        printf("\nIngrese el número que desea agregar:\n");
                        scanf("%d", &valor);
                        insertarAlFinal(&miLista.cola, valor);
                        break; 
                    } else if (decision == 3) {
                        printf("\n Volviendo al menú principal\n");
                    } else {
                        printf("\nOpción inválida. Intente nuevamente.\n");
                    }
                }

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
            printf("Lista Vacia\n");
            liberarLista(&miLista.cabeza);
            break;
        }
        case 6: {
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
                printf("\nVolviendo al menú principal\n");
                continue;
            } else {
                printf("\nOpción incorrecta, volviendo al menú principal\n");
            }
            break;
        }
        default:
            printf("\nOpción incorrecta, elija un número del 1 al 6\n");
            break;
        }
    } while (1);
}
