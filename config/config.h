#ifndef CONFIG_H
#define CONFIG_H

#define MAX_MATERIAS_POR_ALUMNO 10
#define MAX_ALUMNOS_POR_MATERIA 50
#define MAX_ALUMNOS 500
#define MAX_MATERIAS 50
#define MAX_CORRELATIVAS 5
#define MAX_NOMBRE 100

#define EDAD_MINIMA 18
#define EDAD_MAXIMA 99

#define NOTA_MINIMA 0.0
#define NOTA_MAXIMA 10.0
#define NOTA_APROBACION 4.0

#define PAGINADO_TAM 10

#define ALUMNOS_CSV  "data/alumnos.csv"
#define MATERIAS_CSV "data/materias.csv"
#define PLAN_ESTUDIOS "./data/planDeEstudios.txt"
#define SEP_LISTA    "|"   // separa IDs dentro de una celda
#define SEP_RENDIDA  ":"   // separa id:nota:aprobo
#define ID_MATERIAS_AVANZADAS 29  // ID a partir del cual se requiere tener todas las materias anteriores

#endif // CONFIG_H