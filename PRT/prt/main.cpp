/** Includes OpenGL **/
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cassert>
#include <iostream>

#include "mesh.h"
#include "trackball.h"

/* Taille de la fenetre */
int winX = 1200;
int winY = 800;

char * name;

int mouseLeftDown;

Mesh m("mesh");
Mesh m2("copy");
Mesh src("src");

double angleXmin, angleYmin, angleZmin;

void drawGrid()
{
	glDisable(GL_LIGHTING);
	glColor3f(0.5, 0.5, 0.5);
	glLineWidth(2.);
	for(double i = -1; i < 1; i+=0.1) {
		glBegin(GL_LINES);
			glVertex3f (i, -0.5, 1);
			glVertex3f (i, -0.5, -1);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f (1, -0.5, i);
			glVertex3f (-1, -0.5, i);
		glEnd();
	}
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_POLYGON);
		glVertex3f (1, -0.5, 1);
		glVertex3f (-1, -0.5, 1);
		glVertex3f (-1, -0.5, -1);
		glVertex3f (1, -0.5, -1);
	glEnd();
	glEnable(GL_LIGHTING);
}

void drawMesh() {
	Point a(0, -0.5, 0);
	glBegin(GL_TRIANGLES);
  for (auto t : m2.triangles) {
		if(t.normal.angle(a) < 45 )
			glColor3f(1., .0, .0);
		else
			glColor3f(.8, .8, .8);
		glNormal3f(t.normal.x, t.normal.y, t.normal.z);
		for(int i = 0; i < 3; i++)
			glVertex3f(t.points[i].x, t.points[i].y, t.points[i].z);
  }
	glEnd();
}

/* Callback OpenGL d'affichage */
void displayGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMultMatrixd(tbGetTransformation());
	drawGrid();
	drawMesh();

  glPopMatrix();
	glutSwapBuffers();
}

/* Callback OpenGL de redimensionnement */
void reshapeGL(int _w, int _h)
{
	winX = _w;
	winY = _h;

	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)winX/(double)winY, 0.1, 64.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0, 0, 2, // position
	 0, 0, 0,  // point vise
	 0.0, 1.0, 0.0);

	tbInit(winX, winY);
	glutPostRedisplay();
}

/* Callback OpenGL de gestion de souris */
void mouseGL(int _button, int _state, int _x, int _y)
{
	if(_button == GLUT_LEFT_BUTTON) {
		mouseLeftDown = (_state == GLUT_DOWN);

		if(mouseLeftDown == 1)
			tbClick(_x, _y);
		else
			tbRelease(_x, _y);
	}
	else if(_state == GLUT_UP) {
		if(_button == 3)
			tbZoom(-0.05);
		else if(_button == 4)
			tbZoom(0.05);
	}

	glutPostRedisplay();
}

/* Callback OpenGL de gestion de drag */
void motionGL(int x, int y)
{
	if(mouseLeftDown == 1)
		tbMotion(x, y);

	glutPostRedisplay();
}

void keyboardGL(unsigned char k, int x, int y)
{
	if(k == 27 || k == 'q' || k == 'Q')
		exit(0);

	if(k == 'z' || k == 'Z'){
		double angleX, angleY, angleZ;
		double valMin;
		double temp;
		double pas = 45;
		angleX = angleY = angleZ = 0;
		angleXmin = angleYmin = angleZmin = 0;
		src.copy(&m);
		valMin = m.support();
		printf("Début: x = %f y = %f z = %fval = %f\n",angleXmin, angleYmin, angleZmin, valMin);
		for(angleX = 0 ; angleX < 360; angleX+= pas){
			m.rotate(pas, 1, 0, 0);
			for(angleY = 0; angleY < 360; angleY += pas){
				m.rotate(pas, 0, 1, 0);
				for( angleZ = 0; angleZ < 360; angleZ+=pas){
					m.rotate(pas, 0, 0, 1);
					temp = m.support();
					if(temp < valMin){
						valMin = temp;
						angleZmin = angleZ;
						angleXmin= angleX;
						angleYmin = angleY;
						m.copy(&m2);
					}
				}
			}
		}
		printf("Fin: x = %f y = %f z = %f val = %f\n",angleXmin, angleYmin, angleZmin, valMin);
		temp = m2.support();
		printf("%f\n", temp);

	}

	if(k == 'a' || k == 'A'){
			double temp;
			double pas = 45;
			double valMin;
			double i;

			src.copy(&m);

			valMin = m.support();;
			angleYmin = angleZmin = angleXmin = 0;
			printf("Debut: %f %f %f %f\n",angleXmin, angleYmin, angleZmin, valMin);
			for(i = 0; i < 360; i+=pas){
				m.rotate(pas, 1,0,0);
				temp = m.support();
				printf("x %f %f\n", temp, i);
				if(temp <= valMin){
					valMin = temp;
					angleXmin = i;
					m.copy(&m2);
				}
			}
			for(i = 0; i <= angleXmin; i+=pas){
				m.rotate(pas, 1, 0, 0);
			}

			for(i = 0; i < 360; i+=pas){
				m.rotate(pas, 0,1,0);
				temp = m.support();
				printf("y %f %f\n", temp, i);
				if(temp <= valMin){
					valMin = temp;
					angleYmin = i;
					m.copy(&m2);
				}
			}
			for(i = 0; i <= angleYmin; i+=pas){
				m.rotate(pas, 0, 1, 0);
			}

			for(i = 0; i < 360; i+=pas){
				m.rotate(pas, 0,0,1);
				temp = m.support();
				printf("z %f %f\n", temp, i);
				if(temp <= valMin){
					valMin = temp;
					angleYmin = i;
					m.copy(&m2);
				}
			}
			for(i = 0; i <= angleZmin; i+=pas){
				m.rotate(pas, 0, 0, 1);
			}
			printf("Fin: %f %f %f %f\n",angleXmin, angleYmin, angleZmin, valMin);
			temp = m2.support();
			printf("%f\n", temp);
	}

	glutPostRedisplay();
}

/* Fonction d'initialisation OpenGL */
void initGL()
{
	float mat_specular   [4] = { 0.18, 0.18, 0.18, 0.18 };
	float mat_shininess  [ ] = { 128 };
	float global_ambient [ ] = { 0.02, 0.02, 0.05, 0.05 };
	float light0_ambient [ ] = { 0, 0, 0, 0 };
	float light0_diffuse [ ] = { 0.85, 0.85, 0.85, 0.85 };
	float light0_specular[ ] = { 0.85, 0.85, 0.15, 0.85 };

	glClearColor(0.8, 0.8, 0.8, 1.0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
}

int main(int argc, char* argv[]) {
	name = argv[1];
  src.load(name);
	src.normalize();
	m2.load(name);
	m.load(name);

	src.rotate(-90, 1, 0, 0);
	src.copy(&m2);
	src.copy(&m);

  int posX, posY;

	glutInit(&argc, argv);

	posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
	posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;

	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(posX, posY);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Viewer");

	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);

	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutMotionFunc(motionGL);

	initGL();
	glutMainLoop();

	return EXIT_SUCCESS;

}
