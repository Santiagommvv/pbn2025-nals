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

#define PAGINADO_TAM 5

#define TRUE 1
#define FALSE 0

#define ALUMNOS_CSV  "alumnos.csv"
#define MATERIAS_CSV "materias.csv"
#define SEP_LISTA    "|"   // separa IDs dentro de una celda
#define SEP_RENDIDA  ":"   // separa id:nota:aprobo

#endif // CONFIG_H