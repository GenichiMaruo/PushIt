CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lncurses

SRCS = main.c player.c object.c windowmng.c error.c
OBJS = $(SRCS:.c=.o)
EXEC = main.exe

.PHONY: all clean force

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

force: clean all

clean:
	rm -f $(OBJS) $(EXEC)