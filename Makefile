.SUFFIXES:
.SUFFIXES: .out .o .c

CFLAGS= -g -std=gnu99
CC=gcc

srcfiles := $(shell find . -name "*.c")
outfiles := $(patsubst %.c, %.out, $(srcfiles))

all: $(outfiles)

%.out : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean :
	rm -f $(outfiles)
