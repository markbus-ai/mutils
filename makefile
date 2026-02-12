# 1. Variables de configuración ⚙️
CC = gcc
# flags para encontrar los headers (.h)
CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags tesseract lept)
# flags para linkear las librerías (.so / .a)
LDFLAGS = $(shell pkg-config --libs tesseract lept)

TARGET = mutils

# 2. Archivos fuente y objetos 📂
SRC = main.c $(wildcard tools/*.c)
OBJ = $(SRC:.c=.o)

# 3. Regla principal (el "plato final") 🍽️
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# 4. Regla para compilar cada .c a .o
# esto asegura que los flags se usen en cada archivo de tools/
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Limpiar 🧹
clean:
	rm -f $(OBJ) $(TARGET)
