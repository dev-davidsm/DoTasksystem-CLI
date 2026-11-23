CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
TARGET = tarefas

SRCS = app.c lista.c validacao.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

app.o: app.c lista.h
	$(CC) $(CFLAGS) -c app.c

lista.o: lista.c lista.h validacao.h
	$(CC) $(CFLAGS) -c lista.c

validacao.o: validacao.c validacao.h
	$(CC) $(CFLAGS) -c validacao.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean