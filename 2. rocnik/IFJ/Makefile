PRJ=ifj18
#
PROGS=$(PRJ)
FILES=main.c scanner.c parser.c str.c psa.c symtable.c code_generator.c
CC=gcc
CFLAGS=-Wall -W -Wextra -g -std=c99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
