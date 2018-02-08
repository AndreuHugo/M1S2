CC = clang
CCFLAGS = -Wall -std=c99 -lglut -lGLU -lGL -lm
OBJ = OutilsPGM.o

all: main

main : main.c $(OBJ)
	$(CC) $(CCFLAGS) -o main main.c $(OBJ)

OutilsPGM.o : OutilsPGM.c OutilsPGM.h
	$(CC) $(CCFLAGS) -c OutilsPGM.c

clean:
	rm -f main
	- rm -f *.o
