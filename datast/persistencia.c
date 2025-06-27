#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "../include/config.h"
#include <sys/stat.h>

//me fijo si hay datos persistidos de una sesión anterior
int datosGuardadosDisponibles() {
    struct stat buffer1, buffer2;
    return stat(ALUMNOS_CSV, &buffer1) == 0 && stat(MATERIAS_CSV, &buffer2) == 0;
}

//Guardar
static void guardarAlumnosCSV(NodoAVL* a, FILE* f){
    if (!a) return;
    
    guardarAlumnosCSV(a->izq, f);
    

    Alumno al = a->alumno;
    fprintf(f, "%d, %s, %d", al.id, al.nombre, al.edad);

    // Materias inscriptas
    for(int i=0; i<al.cantidadDeMateriasInscripto; i++){
        fprintf(f, "%d", al.materiasInscripto[i]);
        if(i<al.cantidadDeMateriasInscripto-1) fputc(SEP_LISTA[0], f);
    }
    fputc(',',f);

    // materias rendidas
    for(int i=0; i<al.cantidadMateriasRendidas; i++){
        MateriaRendida mr = al.materiasRendidas[i];
        fprintf(f, "%d:%0.2f:%d", mr.IDMateria, mr.nota, mr.aprobo);
        if(i<al.cantidadMateriasRendidas-1) fputc(SEP_LISTA[0],f);
    }

    fputc('\n', f);
    
    guardarAlumnosCSV(a->der,f);
}

static void guardarMateriasCSV(NodoMateria* m, FILE* f){
    while(m){
        Materia mat = m->datos;
        fprintf(f,"%d, %s", mat.id, mat.nombre);
        for(int i=0;i<mat.cantidadAlumnos;i++){
            fprintf(f, "%d", mat.alumnosInscriptos[i]);
            if(i<mat.cantidadAlumnos-1) fputc(SEP_LISTA[0],f);
        }
        fputc('\n', f);
        m=m->siguiente;
    }
}

void guardarDatos(NodoAVL* alumnos, NodoMateria* materias){
    FILE* fa = fopen(ALUMNOS_CSV, "w");
    FILE* fm = fopen(MATERIAS_CSV, "w");
    if(!fa||!fm){puts("Error abriendo archivos para guardar"); return;}

    guardarAlumnosCSV(alumnos, fa);
    guardarMateriasCSV(materias,fm);

    fclose(fa); fclose(fm);
    puts("Datos guardados.");
}

//Cargar
static void split(char* s, char* delim, char*** out, int* count){
    *count = 0; *out=NULL;
    char* tok=strtok(s, delim);
    while(tok){
        *out = realloc(*out,(*count+1) *sizeof(char*));
        (*out)[(*count)++]=tok;
        tok=strtok(NULL, delim);
    }
}

static void cargarAlumnosCSV(NodoAVL** lista, FILE* f){
    char linea[512];
    while(fgets(linea,sizeof(linea),f)){
        Alumno al = {0};
        char *ptr=strchr(linea, '\n'); if(ptr) *ptr='\0';

        char* campos[4];
        int i=0; char* tok=strtok(linea, ",");
        while(tok&&i<4){ campos[i++] = tok; tok=strtok(NULL, ","); }
        if(i<3) continue;

        al.id = atoi(campos[0]);
        strncpy(al.nombre,campos[1],sizeof(al.nombre)-1);
        al.edad = atoi(campos[2]);

        // inscriptas
        al.cantidadDeMateriasInscripto=0;
        if(i>3 && strlen(campos[3])){
            char** ids=NULL; int n=0; split(campos[3], SEP_LISTA, &ids, &n);
            for(int k=0;k<n;k++) al.materiasInscripto[al.cantidadDeMateriasInscripto++]=atoi(ids[k]);
            free(ids);
        }

        // rendidas (si hay 4­° campo)
        al.cantidadMateriasRendidas=0;
        if(tok && strlen(tok)){
            char** rens=NULL;int n=0;split(tok,SEP_LISTA,&rens,&n);
            for(int k=0;k<n;k++){
                char* p1=strtok(rens[k],SEP_RENDIDA);
                char* p2=strtok(NULL,SEP_RENDIDA);
                char* p3=strtok(NULL, SEP_RENDIDA);
                if(p1&&p2&&p3){
                    al.materiasRendidas[al.cantidadMateriasRendidas].IDMateria=atoi(p1);
                    al.materiasRendidas[al.cantidadMateriasRendidas].nota=atof(p2);
                    al.materiasRendidas[al.cantidadMateriasRendidas++].aprobo=atoi(p3);
                }
            }
            free(rens);
        }
        *lista = insertarAVL(*lista, al);
    }
}

static void cargarMateriasCSV(NodoMateria** lista, FILE* f){
    char linea[256];
    while(fgets(linea,sizeof(linea),f)){
        Materia m={0};
        char* nl=strchr(linea,'\n'); if(nl) *nl='\0';

        char* id=strtok(linea,",");
        char* nom=strtok(NULL,",");
        char* ins=strtok(NULL,",");

        if(!id||!nom) continue;
        m.id=atoi(id);
        strncpy(m.nombre,nom,sizeof(m.nombre)-1);

        m.cantidadAlumnos=0;
        if(ins && strlen(ins)){
            char** ids=NULL; int n=0; split(ins,SEP_LISTA,&ids,&n);
            for(int k=0;k<n;k++) m.alumnosInscriptos[m.cantidadAlumnos++]=atoi(ids[k]);
            free(ids);
        }
        agregarMateria(lista,m.nombre);
    }
}

void cargarDatos(NodoAVL** alumnos, NodoMateria** materias){
    FILE* fa=fopen(ALUMNOS_CSV, "r");
    FILE* fm=fopen(MATERIAS_CSV, "r");
    if(fa){ cargarAlumnosCSV(alumnos, fa); fclose(fa);}
    if(fm){ cargarMateriasCSV(materias, fm); fclose(fm);}

}