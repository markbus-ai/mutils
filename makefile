# 1. Variables de configuración ⚙️
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = mutils

# 2. Archivos fuente y objetos 📂
# Usamos wildcard para agarrar todos los .c en la carpeta tools
SRC = main.c $(wildcard tools/*.c)
# Transformamos los nombres .c en .o (objetos)
OBJ = $(SRC:.c=.o)

# 3. Regla principal (el "plato final") 🍽️
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# 4. Regla para limpiar los archivos temporales 🧹
clean:
	rm -f $(OBJ) $(TARGET)# 1. Variables de configuración ⚙️
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = mutils

# 2. Archivos fuente y objetos 📂
# Usamos wildcard para agarrar todos los .c en la carpeta tools
SRC = main.c $(wildcard tools/*.c)
# Transformamos los nombres .c en .o (objetos)
OBJ = $(SRC:.c=.o)

# 3. Regla principal (el "plato final") 🍽️
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# 4. Regla para limpiar los archivos temporales 🧹
clean:
	rm -f $(OBJ) $(TARGET)
