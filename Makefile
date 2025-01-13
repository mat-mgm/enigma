CC = clang
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LIBS = -lgmp -lm

SRCS = crypto.c ast.c enigma.c
OBJS = $(SRCS:.c=.o)
TARGET = enigma

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
