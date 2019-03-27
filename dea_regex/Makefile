BUILDDIR=./builddir/
CC=gcc
CFLAGS=-O3 -Wall -I.

DEPS = dea.h adjlist2dea.h
PROCESS_OBJ = dea.o adjlist2dea.o main.o
CONTAINS_OBJ = dea.o adjlist2dea.o contains.o


all: clean prepare process contains

prepare:
	mkdir -p $(BUILDDIR)

%.o: %.c $(DEPS)
		 $(CC) -c -o $(BUILDDIR)$@ $< $(CFLAGS)

process: $(PROCESS_OBJ)
		cd $(BUILDDIR) && $(CC) -o $@ $^ $(CFLAGS)

contains: $(CONTAINS_OBJ)
		cd $(BUILDDIR) && $(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(BUILDDIR)
