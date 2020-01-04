CC := gcc
CFLAGS := -std=c99 -pedantic

HDRS :=
SRCS := main.c heuristics.c interpreter.c args.c repl.c

OBJS := $(SRCS:.c=.o)
EXEC := bfc
all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
