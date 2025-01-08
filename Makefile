CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lm -lgmp

SRC = enigma.c
OBJ = $(SRC:.c=.o)
HEADER = ast.h crypto.h

TARGET = enigma

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

