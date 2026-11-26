CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
TARGET = tarefas

SRCS = app.c lista.c validacao.c config.c impressao.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

app.o: app.c lista.h config.h impressao.h
	$(CC) $(CFLAGS) -c app.c

lista.o: lista.c lista.h validacao.h config.h
	$(CC) $(CFLAGS) -c lista.c

validacao.o: validacao.c validacao.h lista.h
	$(CC) $(CFLAGS) -c validacao.c

config.o: config.c config.h lista.h validacao.h
	$(CC) $(CFLAGS) -c config.c

impressao.o: impressao.c impressao.h lista.h config.h validacao.h
	$(CC) $(CFLAGS) -c impressao.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean