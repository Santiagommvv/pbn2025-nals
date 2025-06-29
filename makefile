# Compilador
CC = gcc

# Nombre del ejecutable
OUT = gestion.exe

# Directorios de include
INCLUDES = -Iutil -Idominio -Iextensiones

# Archivos fuente
SRCS = main.c \
	$(wildcard dominio/*.c) \
	$(wildcard extensiones/*.c) \
	$(wildcard util/*.c)

# Comando por defecto
all:
	$(CC) $(SRCS) -o $(OUT) $(INCLUDES)

# Limpiar archivos generados
clean:
	del $(OUT)