EXECUTABLE = program
SRC = ./
INCLUDE = ../include/
LIBS = ../lib/
OBJ = $(wildcard *.o)
CC = gcc
CFILES = ./main.c ./world.c ./samath.c
OFILES = $(patsubst %.c,%.o, $(CFILES))

build: $(OFILES)
	gcc $(OFILES) -o $(EXECUTABLE) -L $(LIBS) -lraylib -lopengl32 -lgdi32 -lwinmm
	./$(EXECUTABLE)

%.o: %.c
	 gcc $? -o $@ -c -isystem $(INCLUDE)


clean:
	rm *.o
	clear