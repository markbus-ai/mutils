CC = gcc
CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags tesseract lept)
LDFLAGS = $(shell pkg-config --libs tesseract lept)

TARGET = mutils

SRC = main.c $(wildcard tools/*.c)
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
