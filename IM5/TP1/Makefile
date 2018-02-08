XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11 -lXext -lpthread
GLLIBS = -L/usr/X11R6/lib -lglut -lGL -lGLU

main : main.o
	gcc -o main main.o $(GLLIBS) $(XLIBS) -lm
	rm main.o
