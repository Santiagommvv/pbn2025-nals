#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "randomizador.h"

// Nombres y apellidos base - Ampliados para permitir más combinaciones
const char* nombres[] = {
    "Juan", "Ana", "Carlos", "Laura", "Mauro", "Lucia", "Pedro", "Martina",
    "Miguel", "Sofia", "Diego", "Valentina", "Gabriel", "Camila", "Jose", "Victoria",
    "Alejandro", "Natalia", "Fernando", "Mariana", "Roberto", "Julia", "Marcelo", "Florencia",
    "Daniel", "Catalina", "Eduardo", "Agustina", "Ricardo", "Paula", "Javier", "Rocio",
    "Gustavo", "Daniela", "Raul", "Silvina", "Alberto", "Carolina", "Hector", "Luz"
};

const char* apellidos[] = {
    "Perez", "Gonzalez", "Martinez", "Gomez", "Rodriguez", "Fernandez", "Sanchez", "Rosamilia",
    "Lopez", "Diaz", "Torres", "Ramirez", "Flores", "Acosta", "Benitez", "Romero",
    "Alvarez", "Gutierrez", "Molina", "Vega", "Sosa", "Castro", "Ortiz",
    "Medina", "Suarez", "Herrera", "Aguirre", "Peralta", "Rios", "Morales", "Ferrari",
    "Gimenez", "Correa", "Quiroga", "Campos", "Silva", "Vargas", "Blanco", "Navarro"
};

const char* materias[] = {
    "Programacion de bajo nivel", "AyP1", "AyP2", "Algebra", "Analisis I", "Analisis II",
    "Fisica I", "Fisica II", "Algebra II", "Sistemas Operativos", "Bases de Datos", "Redes",
    "Matematica Discreta", "Diseño Logico", "Ingenieria de Software", "Seguridad Informatica",
    "Cuestiones de Sociologia, Economia y Politica", "Probabilidad y Estadistica", "Arquitectura de Computadoras", "Matematica Discreta",
    "Algebra III", "Arquitectura de computadoras", "Ingenieria de Requisitos", "Gestion de Proyectos",
    "Problemas de Historia del siglo XX", "Estructuras de Datos", "Comunicacion de Datos", "Etica Profesional"
};

Alumno crearAlumnoAleatorio() {
    // Obtener un nombre aleatorio
    const char* nombre = nombres[rand() % (sizeof(nombres)/sizeof(char*))];
    
    // Obtener un apellido aleatorio
    const char* apellido = apellidos[rand() % (sizeof(apellidos)/sizeof(char*))];
    
    int edad = EDAD_MINIMA + rand() % (EDAD_MAXIMA - EDAD_MINIMA + 1);
    
    Alumno a = crearAlumno(nombre, apellido, edad);
    return a;
}

Materia crearMateriaAleatoria(){
    Materia m;
    memset(&m, 0, sizeof(Materia));
    
    // Añadir un número aleatorio para reducir colisiones
    int randomIndex = rand() % (sizeof(materias)/sizeof(char*));
    int randomSuffix = rand() % 100;
    
    // Decidir si agregar el sufijo aleatorio (70% de probabilidad)
    if (randomSuffix > 30) {
        char nombreConSufijo[100];
        snprintf(nombreConSufijo, sizeof(nombreConSufijo), "%s %02d", 
                 materias[randomIndex], randomSuffix);
        strncpy(m.nombre, nombreConSufijo, sizeof(m.nombre) - 1);
    } else {
        strncpy(m.nombre, materias[randomIndex], sizeof(m.nombre) - 1);
    }
    
    m.nombre[sizeof(m.nombre) - 1] = '\0';
    return m;
}

void generarAlumnosAleatorios(NodoAVL** raiz, int cantidad){
    if (!raiz) {
        printf("Error: Puntero a raíz es NULL\n");
        return;
    }
    
    srand(time(NULL));
    int alumnosGenerados = 0;
    int alumnosFallidos = 0;
    
    for(int i=0; i<cantidad; i++){
        Alumno a = crearAlumnoAleatorio();
        
        // Verificar si el alumno se creó correctamente (ID > 0)
        if (a.id > 0) {
            *raiz = insertarAVL(*raiz, a);
            alumnosGenerados++;
        } else {
            alumnosFallidos++;
        }
    }
    
    if (alumnosFallidos > 0) {
        printf("Atención: No se pudieron generar %d alumnos debido a errores.\n", alumnosFallidos);
    }
    
    printf("%d alumnos aleatorios generados correctamente.\n", alumnosGenerados);
}

void generarMateriasAleatorias(NodoMateria** lista, int cantidad) {
    if (!lista) {
        printf("Error: Puntero a lista es NULL\n");
        return;
    }
    
    srand(time(NULL));
    int materiasGeneradas = 0;
    int intentos = 0;
    const int maxIntentos = cantidad * 3; // Limitamos intentos para evitar bucles infinitos
    
    while (materiasGeneradas < cantidad && intentos < maxIntentos) {
        Materia m = crearMateriaAleatoria();
        
        // Verificar si ya existe una materia con este nombre
        if (buscarMateriaPorNombre(*lista, m.nombre) == NULL) {
            NodoMateria* resultado = agregarMateria(lista, m.nombre);
            if (resultado) {
                materiasGeneradas++;
            } else {
                printf("Error: No se pudo agregar la materia %s\n", m.nombre);
            }
        }
        intentos++;
    }
    
    printf("%d materias aleatorias generadas.\n", materiasGeneradas);
}