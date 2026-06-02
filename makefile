CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = wordle_cti
SRCS = main.c logica.c ui.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lncurses

%.o: %.c wordle.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
