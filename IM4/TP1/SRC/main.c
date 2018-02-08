#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>
#include "OutilsPGM.h"

#define PI 3.14159265358979323846
#define GAUSSIEN_3 2
#define LAPLACIEN_3 1
#define NONE 0

Image img;
Image img2;
Image img3;
Filter filter;

int window, window2, window3;

int D, n;
double dt;

double t;

//------------------------------------------------------------------------------
//--------------------------------UTILS-----------------------------------------
//------------------------------------------------------------------------------

void initFilter(Filter * f, int size){
	int i;
	f->mat = NULL;
	f->size = size;
	f->mat = malloc(sizeof(float *)*(2*size+1));
	for(i = 0 ; i < 2*size+1; i++)
		f->mat[i] = malloc(sizeof(float)*(2*size+1));
}

void loadFilter(Filter * f, int type, bool show){
	int i,j;
	switch(type){
		case NONE:
			for(i = 0; i < f->size; i++)
				for(j = 0; j < f->size; j++)
					f->mat[i][j] = 0.;
		break;
		case LAPLACIEN_3:
      initFilter(f, 1);
			f->mat[0][0] = 0; f->mat[0][1] = 1; f->mat[0][2] = 0;
			f->mat[1][0] = 1; f->mat[1][1] = -4; f->mat[1][2] = 1;
			f->mat[2][0] = 0; f->mat[2][1] = 1; f->mat[2][2] = 0;
      if(show){
				printf("Filtre Laplacien 3x3\n");
        for(i = 0; i < 2*f->size+1; i++){
          for(j = 0; j < 2*f->size+1; j++)
            printf(" %2.f ", f->mat[i][j]);
          printf("\n");
        }
        printf("\n");
      }
		break;
		case GAUSSIEN_3:{
			initFilter(f, 1);
			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					f->mat[i][j] = (1/(2*PI*t))*exp(-(i*i+j*j)/(t));
			if(show){
				printf("Filtre Gaussien 3x3\n");
        for(i = 0; i < 2*f->size+1; i++){
          for(j = 0; j < 2*f->size+1; j++)
            printf(" %2.5f ", f->mat[i][j]);
          printf("\n");
        }
        printf("\n");
      }
		}
	}
}


//------------------------------------------------------------------------------
//--------------------------------FONCTIONS TP----------------------------------
//------------------------------------------------------------------------------

double convolution(Image * I, Filter * filtre, int x, int y){
	int i,j;
	double value = 0;
	for(i = -filtre->size; i <= filtre->size; i++){
		for(j = -filtre->size; j <= filtre->size; j++){
			if(isInside(x+i, y+j, 0, I->width, 0, I->height)){
				value += getPixelValueAt(I, x+i,y+j)*(float)filtre->mat[i+filtre->size][j+filtre->size];
			}
			else{
				value += getPixelValueAt(I, x,y)*(float)filtre->mat[i+filtre->size][j+filtre->size];
			}
		}
	}
	return value;
}

void filtrage(Image * I, Image * O, Filter * filtre){
  int i,j;
	for(i = 0; i < I->width; i++)
    for(j = 0; j < I->height; j++){
		  setPixelValueAt(O, i, j, convolution(I, filtre, i,j));
    }
}

void chaleur(Image * I, Image * O, double _dt, double _D, int _n){
  int i, j,k;
  Image TMP;
  CreerImage(&TMP, I->width, I->height);
	CopierImage(I, O);

	for(i = 0; i < _n; i++){
		filtrage(O, &TMP, &filter);
		for(j = 0; j < O->width; j++)
			for(k = 0; k < O->height; k++)
				setPixelValueAt(O, j,k, getPixelValueAt(O, j,k)+_dt*getPixelValueAt(&TMP, j,k));
	}
}

void filtreGaussien(Image * I, Image * O, double t){

}

//------------------------------------------------------------------------------
//--------------------------------INIT------------------------------------------
//------------------------------------------------------------------------------

void initGL(void) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glClearColor(0.0, 0.0, 0.0, 1.0);
	dt = 0.1;
	n = 10;
	D = 1;
	t = 1;
}

//------------------------------------------------------------------------------
//----------------------------AFFICHAGE FENETRE---------------------------------
//------------------------------------------------------------------------------

void render(void) {
  showImage(&img);
}

void render2(void) {
  showImage(&img2);
}

void render3(void){
	glClear(GL_COLOR_BUFFER_BIT);
	char buffer[32];
	sprintf(buffer, "N = %d",n);
	glPrintText(50, img.height-30, buffer);
	sprintf(buffer, "dt = %2.2f",dt);
	glPrintText(50, img.height-60, buffer);
	sprintf(buffer, "D = %d",D);
	glPrintText(50, img.height-90, buffer);
}

//------------------------------------------------------------------------------
//----------------------------REDIM---------------------------------------------
//------------------------------------------------------------------------------

void redim(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}


//------------------------------------------------------------------------------
//--------------------------------CLAVIER----------------------------------------
//------------------------------------------------------------------------------

void keyboard(unsigned char k, int x, int y) {
	if(k == 'c' || k == 'C'){
		chaleur(&img2, &img3, dt, D, n);
		CopierImage(&img3, &img2);
	}
	if(k == 'r' || k == 'R')
		CopierImage(&img, &img2);
	if(k == 'z' || k == 'Z')
		dt+=0.1;
	if(k == 'a' || k =='A')
		if(dt > 0.1) dt-=0.1;
	if(k == 's' || k =='S')
		D++;
	if(k == 'q' || k =='Q')
		if(D > 1) D--;
	if(k == 'x' || k =='X')
		n++;
	if(k == 'w' || k =='W')
		if(n>1) n--;


	glutSetWindow(window2);
  glutPostRedisplay();
	glutSetWindow(window3);
  glutPostRedisplay();
}

//------------------------------------------------------------------------------
//--------------------------------MAIN----------------------------------------
//------------------------------------------------------------------------------

int main(int argc, char** argv) {

  if(argc != 2){ printf("Pas assez d'arguments\n"); return 0;}
	else LireImage(argv[1], &img);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	CreerImage(&img2, img.width, img.height);
	CreerImage(&img3, img.width, img.height);

  loadFilter(&filter, LAPLACIEN_3, true);

	chaleur(&img, &img2, dt, D, n);

  //Premiere fenetre
  glutInitWindowSize(img.width , img.height);
  glutInitWindowPosition(70, 100);
  window = glutCreateWindow("Source");
  glutDisplayFunc(render);
  glutReshapeFunc(redim);
  glutKeyboardFunc(keyboard);

	//Deuxième fenetre
	glutInitWindowSize(img.width , img.height);
	glutInitWindowPosition(img.width+70, 100);
	window2 = glutCreateWindow("Laplacien");
	glutDisplayFunc(render2);
	glutReshapeFunc(redim);
	glutKeyboardFunc(keyboard);

	//Troisième fenetre
	glutInitWindowSize(200 , img.height);
	glutInitWindowPosition(2*img.width+70, 100);
	window3 = glutCreateWindow("Infos");
	glutDisplayFunc(render3);
	glutReshapeFunc(redim);
	glutKeyboardFunc(keyboard);

  initGL();
  glutMainLoop();

  return 0;

}
