CC = clang++
XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11 -lXext -lpthread
GLLIBS = -L/usr/X11R6/lib -lglut -lGL -lGLU
CXXFLAGS = -g -std=c++11 -Wall -pedantic -lm
OBJ = point.o mesh.o trackball.o

main : main.o $(OBJ)
	$(CC) -o main main.o $(GLLIBS) $(XLIBS) -lm $(OBJ)

point.o : point.cpp point.h
	$(CC) $(CXXFLAGS) -c point.cpp

mesh.o : mesh.cpp mesh.h
	$(CC) $(CXXFLAGS) -c mesh.cpp

trackball.o : trackball.cpp trackball.h
	$(CC) $(CXXFLAGS) -c trackball.cpp

clean:
		rm -f systeme
		- rm -f *.o
