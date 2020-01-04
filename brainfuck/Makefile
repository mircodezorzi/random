CC := gcc
CFLAGS := -O3

HDRS :=
SRCS := main.c heuristics.c interpreter.c args.c

OBJS := $(SRCS:.c=.o)
EXEC := bf
DEST := /usr/local/bin

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

install: $(EXEC)
	cp $(EXEC) $(DEST)

uninstall:
	rm $(DEST)/$(EXEC)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
