#include <stdio.h>
#include "include/listaES.h" //definir bandera si queremos sacar include/ de la cadena
#include "include/nodo.h"

extern Lista miLista;

int main(){

    int opcion;
    do{
    printf(
        "\nElije que desea realizar:\n"
        "1) agregar un numero a la lista\n"
        "2) eliminar un elemento de la lista\n"
        "3) buscar un elemento de la lista\n"
        "4) imprimir los elementos de la lista\n"
        "5) salir del programa\n\n"); 
        scanf("%d", &opcion);

    if (opcion < 1 || opcion > 5){
        printf("\nopcion incorrecta, elija un numero del 1 al 5\n");
        continue;
    }


    switch (opcion){
    case 1:
        int valor;
        printf("\nIngrese el numero que desea agregar\n");
        scanf("%d", &valor);
        insertarAlInicio(&miLista.cabeza,valor);
        break;
    case 2:
        printf("eliminar un elemento de la lista");
    case 3:
        printf("buscar un elemento de la lista"); 
    case 4:     
        printf("imprimir los elementos de la lista");  
    
    case 5:{
        int salir;
        printf(
            "\n¿esta seguro que desea salir de la lista?\n" 
            "1) SI\n"
            "2) NO\n");
        scanf("%d", &salir);
        if (salir==1){
           printf("Adios :)\n");
        return 0;
         }else if(salir == 2){
           printf("\nVolviendo al menu principal\n");
            continue;
        }else{
           printf("\nLa opcion es incorrecta, volviendo al menu principal\n");
        }
      }
    }
  }while(1);
}