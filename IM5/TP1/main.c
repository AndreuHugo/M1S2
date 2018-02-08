#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "utils.h"


/* Includes standards */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/* Defines d'informations */
#define TP_TITLE			"IM5 - Concepts et methodes de visualisation"
#define TP_GROUP			"ANDREU Hugo"
#define TP_PROMO			"2017-2018"
void						usage();


int intersect(Point * p1, Point p2){
  if(p1->pos[0]+POINT_SIZE >= p2.pos[0] && p1->pos[0]-POINT_SIZE <= p2.pos[0] &&
       p1->pos[1]+POINT_SIZE >= p2.pos[1] && p1->pos[1]-POINT_SIZE <= p2.pos[1]){
    return 1;
  }
  return 0;
}

/* On compte le nombre d'intersection entre un point et un polygone */
int countIntersect(Polygon * l, Point * p){
  int i,j;
  int p1, p2;
  int nbCol = 0;
  float xi;
  for(j = 0; j < l->nbPoint-1; j++){
    p1 = p2 = 0;
    xi = 0.;

    /*Gestion direction HD-BD | BD-HD */
    if(l->points[j]->pos[0] > p->pos[0]) p1 += 1;
    if(l->points[j]->pos[1] > p->pos[1]) p1 += 2;

    if(l->points[j+1]->pos[0] > p->pos[0]) p2 += 1;
    if(l->points[j+1]->pos[1] > p->pos[1]) p2 += 2;

    /*Gestion direction HG-BD | HD-BG */
    xi = l->points[j]->pos[0] + (p->pos[1] - l->points[j]->pos[1]) * ( (l->points[j+1]->pos[0]-l->points[j]->pos[0]) / (l->points[j+1]->pos[1]-l->points[j]->pos[1]) );

    if((p1 == 3 && p2 == 1) || (p1 == 1 && p2 == 3))
      nbCol++;
    else if((p1 == 3 && p2 == 0) || (p1 == 2 && p2 == 1) || (p1 == 0 && p2 == 3) || (p1 == 1 && p2 == 2)){
      if(xi > p->pos[0])
        nbCol++;
    }
  }
  return nbCol;
}

/* Ajout le polygone en tant que trou si besoin */
void addHoleToPolygon(Polygon * polygon, ListPolygon * p){
  int i;
  int intersection;
  for(i = 0; i < p->nbPolygon; i++){
    if(p->polygons[i].id != polygon->id && countIntersect(&p->polygons[i], polygon->points[0]) %2!= 0){
      p->polygons[i].holes[p->polygons[i].nbHole] = polygon;
      p->polygons[i].nbHole++;
      printf("polygon %d added to polygon %d as hole\n", polygon->id, p->polygons[i].id);
    }
  }
}

/* Fonction de gestion d'un polygone */
void addPointToPolygon(Polygon * polygon, ListPoint * pointList, ListPolygon * polyList,Point point){
  int i;
  if(polygon->nbPoint < MAX_POINTS){
    for(i = 0; i < polygon->nbPoint; i++){
      if(intersect(polygon->points[i], point)){
        if(i == 0){
          lastPoint.pos[0] = pointList->points[i].pos[0];
          lastPoint.pos[1] = pointList->points[i].pos[1];
          lastPoint.pos[2] = 0;
          polygon->points[polygon->nbPoint] = polygon->points[0];
          polygon->nbPoint++;
          polygon->nbHole = 0;
          polyList->nbPolygon++;
          polygon->id = polygon->points[0]->pos[0]*polygon->points[1]->pos[0];
          tool = 0;
          addHoleToPolygon(polygon, polyList);
          printf("Point %d added to polygon %d\n", point.id, polygon->id);
          printf("End of polygon \n");
        }
        return;
      }
    }
    for(i = 0; i < pointList->nbPoint; i++){
      if(intersect(&pointList->points[i], point)){
        lastPoint.pos[0] = pointList->points[i].pos[0];
        lastPoint.pos[1] = pointList->points[i].pos[1];
        lastPoint.pos[2] = 0;
        polygon->points[polygon->nbPoint] = &pointList->points[i];
        polygon->nbPoint++;
        printf("Point %d added to polygon\n", point.id);
      }
    }
  }
  else{
    printf("Too much points !\n");
  }
}

/* Fonction de gestion de la liste des points */

int pointExist(ListPoint * l, Point point){
  int i;
  for(i = 0; i < l->nbPoint; i++){
    if(intersect(&l->points[i], point)){
      selectedPoint = i;
      return 1;
    }
  }
  return 0;
}
void addPointToList(ListPoint * l, Point point){
  int i;
  if(l->nbPoint < MAX_POINTS){
    if(selectedPoint == -1){
      l->points[l->nbPoint] = point;
      printf("Point %d added [%3.f %3.f]\n",l->points[l->nbPoint].id, l->points[l->nbPoint].pos[0], l->points[l->nbPoint].pos[1]);
      l->nbPoint++;
    }
    else{
      selectedPoint = -1;
    }
  }
  else{
    printf("Too much points !\n");
  }
}

/* Fonction de gestion des points */
Point createPoint(int x, int y){
  Point p;
  p.pos[0] = (float)x;
  p.pos[1] = winY-(float)y;
  p.pos[2] = 0;
  p.id = x*y-x;
  return p;
}


int secant(Point a, Point b, Point c, Point d){
  Point ca, cb, da, db, ac, bc, ad, bd;
  ca.pos[0] = a.pos[0]-c.pos[0]; ca.pos[1] = a.pos[1]-c.pos[1];
  cb.pos[0] = b.pos[0]-c.pos[0]; cb.pos[1] = b.pos[1]-c.pos[1];
  da.pos[0] = a.pos[0]-d.pos[0]; da.pos[1] = a.pos[1]-d.pos[1];
  db.pos[0] = b.pos[0]-d.pos[0]; db.pos[1] = b.pos[1]-d.pos[1];

  ac.pos[0] = c.pos[0]-a.pos[0]; ac.pos[1] = c.pos[1]-a.pos[1];
  bc.pos[0] = c.pos[0]-b.pos[0]; bc.pos[1] = c.pos[1]-b.pos[1];
  ad.pos[0] = d.pos[0]-a.pos[0]; ad.pos[1] = d.pos[1]-a.pos[1];
  bd.pos[0] = d.pos[0]-b.pos[0]; bd.pos[1] = d.pos[1]-b.pos[1];
  if((ca.pos[0]*cb.pos[1] - cb.pos[0]*ca.pos[1]) / (da.pos[0]*db.pos[1] - db.pos[0]*da.pos[1]) > 0)
    return 1;
  else{
    if((ac.pos[0]*ad.pos[1] - ad.pos[0]*ac.pos[1]) / (bc.pos[0]*bd.pos[1] - bd.pos[0]*bc.pos[1]) < 0)
      return 0;
    return 1;
  }

}


void removePolygon(ListPolygon * l, int indice){
  int i;
  for(i = indice; i < l->nbPolygon-1; i++){
    l->polygons[i] = l->polygons[i+1];
  }
  l->nbPolygon--;
  l->polygons[l->nbPolygon].nbPoint = 0;
  l->polygons[l->nbPolygon].id = 0;
  l->polygons[l->nbPolygon].nbHole = 0;
}

/* On verifie si le curseur de la souris est dans un polygone */
int isInsidePolygon(ListPolygon * list, int indice, Point p){
  int nbCol = countIntersect(&list->polygons[indice], &p);
  int others = 0;
  int i;
  if(nbCol%2 != 0){
    for(i = 0; i < list->nbPolygon; i++){
      if( i != indice){
        others += countIntersect(&list->polygons[i], &p);
      }
    }
  }
  return nbCol+others;
}

/* Fonction pour afficher du texte en OpenGL */
void glPrintText(int x, int y, const char * text)
{
	int i;
	glRasterPos2i(x, y);
	for(i = 0; i < (int)strlen(text); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
}

/* Callback OpenGL d'affichage */
void displayGL()
{
  int i,j,k;
  char buffer[32];
  glClear(GL_COLOR_BUFFER_BIT);

  /* Affichage du mode pour le mode de dessin */
  if(glDrawStyle == 1 || showPoint){
    glColor4f(1.,0.,0., 1.);
    glPrintText(winX-180, winY-15, "GL_POINTS");
    glColor4f(0.,0.,1., 0.2);
    glPrintText(winX-100, winY-30, "GL_LINE_LOOP");
    glPrintText(winX-100, winY-15, "GL_LINES");
    glPrintText(winX-100, winY-45, "GL_LINE_STRIP");
    glBegin(GL_POINTS);
    for(i = 0; i < listPoint.nbPoint; i++){
      if(i == selectedPoint) glColor3f(0., 1., 0.);
      else glColor3f(1., 0., 0.);
      if(tool == 2){
        for(j = 0; j <= listPolygon.nbPolygon; j++)
          for(k = 0; k < listPolygon.polygons[j].nbPoint; k++){
            if(intersect(listPolygon.polygons[j].points[k], listPoint.points[i]))
              glColor3f(0.,0.,1.);
          }
      }
      glVertex3fv(listPoint.points[i].pos);
    }
    glEnd();
  }
  else{
    glColor4f(0.,0.,1., 0.2);
    glPrintText(winX-180, winY-15, "GL_POINTS");
  }

  /* Differentes options pour l'affichage des polygones */
  for(i = 0; i <= listPolygon.nbPolygon; i++){
    glColor3f(0.,0.,1.);
      switch(glDrawStyle){
        case 2:
          glColor3f(1.,0.,0.);
          glPrintText(winX-100, winY-15, "GL_LINES");
          glColor4f(0.,0.,1., 0.2);
          glPrintText(winX-100, winY-30, "GL_LINE_LOOP");
          glPrintText(winX-100, winY-45, "GL_LINE_STRIP");
          glColor4f(0.,0.,0., 1.);
          if(i == selectedPolygon) glColor4f(1.,0.,0., 1.);
          glBegin(GL_LINES);
            for(j = 0; j < listPolygon.polygons[i].nbPoint-1; j++){
              glVertex3fv(listPolygon.polygons[i].points[j]->pos);
              glVertex3fv(listPolygon.polygons[i].points[j+1]->pos);
            }
          glEnd();
        break;
        case 3:
          glColor3f(1.,0.,0.);
          glPrintText(winX-100, winY-30, "GL_LINE_LOOP");
          glColor4f(0.,0.,1., 0.2);
          glPrintText(winX-100, winY-15, "GL_LINES");
          glPrintText(winX-100, winY-45, "GL_LINE_STRIP");
          glColor4f(0.,0.,0., 1.);
          if(i == selectedPolygon) glColor4f(1.,0.,0., 1.);
          glBegin(GL_LINE_LOOP);
            for(j = 0; j < listPolygon.polygons[i].nbPoint; j++)
              glVertex3fv(listPolygon.polygons[i].points[j]->pos);
          glEnd();
        break;
        case 4:
          glColor4f(0.,0.,1., 0.2);
          glPrintText(winX-100, winY-30, "GL_LINE_LOOP");
          glPrintText(winX-100, winY-15, "GL_LINES");
          glColor3f(1.,0.,0.);
          glPrintText(winX-100, winY-45, "GL_LINE_STRIP");
          glColor4f(0.,0.,0., 1.);
          if(i == selectedPolygon) glColor4f(1.,0.,0., 1.);
          glBegin(GL_LINE_STRIP);
            for(j = 0; j < listPolygon.polygons[i].nbPoint-1; j++){
              glVertex3fv(listPolygon.polygons[i].points[j]->pos);
              glVertex3fv(listPolygon.polygons[i].points[j+1]->pos);
            }
            if(listPolygon.polygons[i].nbPoint > 0){
              glVertex3fv(listPolygon.polygons[i].points[listPolygon.polygons[i].nbPoint-1]->pos);
              glVertex3fv(listPolygon.polygons[i].points[0]->pos);
            }
          glEnd();
        break;
      }
  }

  /* Dessin du segment de polygone en cours de création */
  if(tool == 2 && listPolygon.polygons[listPolygon.nbPolygon].nbPoint > 0){
    if(polygonCanBeClose)
      glColor3f(0.,1.,0.);
    else
      glColor3f(1.,0.,0.);
    glBegin(GL_LINES);
      glVertex3f(listPolygon.polygons[listPolygon.nbPolygon].points[listPolygon.polygons[listPolygon.nbPolygon].nbPoint-1]->pos[0], listPolygon.polygons[listPolygon.nbPolygon].points[listPolygon.polygons[listPolygon.nbPolygon].nbPoint-1]->pos[1], 0);
      glVertex3f(mouseX, mouseY, 0.);
    glEnd();
  }

  /* Affichage des positions des points */
  if(showPointPos){
    for(i = 0; i < listPoint.nbPoint; i++){
      sprintf(buffer, "[%3.f %3.f]", listPoint.points[i].pos[0], listPoint.points[i].pos[1]);
      glPrintText(listPoint.points[i].pos[0]+POINT_SIZE, listPoint.points[i].pos[1]+POINT_SIZE, buffer);
    }
  }

  /* Affichage des infos du mode de création */
  if(tool == 2){
    sprintf(buffer, "POLYGONE");
    glColor4f(1.,0.,0., 1.);
    glPrintText(10, winY-15, buffer);
    sprintf(buffer, "POINT");
    glColor4f(0.,0.,1., 0.2);
    glPrintText(0, winY-30, buffer);
    sprintf(buffer, "SELECTION");
    glColor4f(0.,0.,1., 0.2);
    glPrintText(0, winY-45, buffer);
  }
  else if(tool ==1){
    sprintf(buffer, "POLYGONE");
    glColor4f(0.,0.,1., 0.2);
    glPrintText(0, winY-15, buffer);
    sprintf(buffer, "POINT");
    glColor4f(1.,0.,0.,1.);
    glPrintText(10, winY-30, buffer);
    sprintf(buffer, "SELECTION");
    glColor4f(0.,0.,1., 0.2);
    glPrintText(0, winY-45, buffer);
  }
  else{
    sprintf(buffer, "POLYGONE");
    glColor4f(0.,0.,1., 0.2);
    glPrintText(0, winY-15, buffer);
    sprintf(buffer, "POINT");
    glColor4f(0.,0.,1.,0.2);
    glPrintText(0, winY-30, buffer);
    sprintf(buffer, "SELECTION");
    glColor4f(1.,0.,0., 1.);
    glPrintText(10, winY-45, buffer);
  }

  if(helpWindow){
    glColor4f(0.,0.,0.,1.);
    glBegin(GL_LINE_LOOP);
      glVertex3f(helpWindowX, helpWindowX, 0.);
      glVertex3f(winX-helpWindowX, helpWindowX, 0.);
      glVertex3f(winX-helpWindowX, helpWindowY, 0.);
      glVertex3f(helpWindowX, helpWindowY, 0.);
    glEnd();
    glColor4f(0.8,0.8,0.8,0.8);
    glBegin(GL_QUADS);
      glVertex3f(helpWindowX+1., helpWindowX+1., 0.);
      glVertex3f(helpWindowX, helpWindowY-1, 0.);
      glVertex3f(winX-helpWindowX+1, helpWindowY-1, 0.);
      glVertex3f(winX-helpWindowX+1, helpWindowX+1, 0.);
    glEnd();
    glColor4f(0.,0.,0., 1.);
    glPrintText(helpWindowX+10, helpWindowY-20, "[Utilisation]");
    glPrintText(helpWindowX+10, helpWindowY-50, "    Clic gauche   : ajouter point");
    glPrintText(helpWindowX+10, helpWindowY-70, "    Clic droit    : supprimer point");
    glPrintText(helpWindowX+10, helpWindowY-100, "[Manuel]");
    glPrintText(helpWindowX+10, helpWindowY-130, "    h | H        : Afficher / masquer la fenêtre d'aide");
    glPrintText(helpWindowX+10, helpWindowY-150, "    c | C        : Afficher / masquer les positions des points");
    glPrintText(helpWindowX+10, helpWindowY-170, "      &          : Afficher / masquer points");
    glPrintText(helpWindowX+10, helpWindowY-190, "    p | P        : Passer mode Polygone / Point / Selection");
    glPrintText(helpWindowX+10, helpWindowY-210, "    1 2 3 4     : Changer mode dessin");
    glPrintText(helpWindowX+10, helpWindowY-230, "    q | Esc     : Quitter");
  }
  else{
    glColor4f(1.,0.,0.,1.);
    glPrintText(winX/2-90,winY-15, "touche H pour afficher l'aide");
  }
  if(selectedPoint != -1){
    glColor4f(1.,0.,0.,1.);
    sprintf(buffer, "Id du point selectionne: %d", listPoint.points[selectedPoint].id);
    glPrintText(10,10, buffer);
  }
  else if(selectedPolygon != -1){
    glColor4f(1.,0.,0.,1.);
    sprintf(buffer, "Id du polygone selectionne: %d", listPolygon.polygons[selectedPolygon].id);
    glPrintText(10,10, buffer);
  }

	glutSwapBuffers();
}


/* Callback OpenGL de redimensionnement */
void reshapeGL(int w, int h)
{
	winX = w;
	winY = h;

	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, winX, 0.0, winY);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

/* Callback OpenGL de gestion de souris */
void mouseGL(int button, int state, int x, int y)
{
  int i;
  Point p = createPoint(x, y);
  if(!mouseDown){
  	if(button == GLUT_LEFT_BUTTON){
      mouseDown = !mouseDown;
      clicX = x; clicY = y;
      if(tool > 0){
        /* Si le point n'existe pas on le créer */
        if(!pointExist(&listPoint, p))
          addPointToList(&listPoint, p);
        /* Mode création de polygone activé */
        if(tool == 2){
          selectedPoint = -1;
          /* Si on peut ajouter un point au polygone on l'ajoute */
          if(polygonCanBeClose){
            for(i = 0; i < listPoint.nbPoint; i++)
              if(intersect(&listPoint.points[i], p))
                addPointToPolygon(&listPolygon.polygons[listPolygon.nbPolygon], &listPoint, &listPolygon, p);
          }
        }
      }
      else{
        int i;
        selectedPolygon = -1;
        selectedPoint = -1;
        pointExist(&listPoint, p);
        for(i = 0; i < listPolygon.nbPolygon; i++){
          if(isInsidePolygon(&listPolygon, i, p)%2 == 1)
            selectedPolygon = i;
        }
      }
    }

  	if(button == GLUT_RIGHT_BUTTON){
      printf("%d\n", tool);
      mouseDown = !mouseDown;
      if(tool ==0){
        for(i = 0; i < listPolygon.nbPolygon; i++){
          if(isInsidePolygon(&listPolygon, i, p)%2 == 1){
            removePolygon(&listPolygon, i);
            break;
          }
        }
      }
  	}
  }
  else{
    mouseDown = !mouseDown;
  }

	glutPostRedisplay();
}


/* Callback OpenGL de gestion de clavier */
void keyboardGL(unsigned char k, int x, int y)
{
	if(k == 27 || k == 'q' || k == 'Q')
		exit(0);

	if(k == 'h' || k == 'H')
		usage();

  if(k == 'p' || k == 'P'){
    selectedPoint = -1;
    selectedPolygon = -1;
    lastPoint.pos[0] = -100;
    lastPoint.pos[1] = -100;
    if(tool < 2) tool++;
    else tool = 0;
    if(tool != 0){
      clicX = -100;
      clicY = -100;
    }
  }
   if(k == 'c' || k == 'C'){
     showPointPos = !showPointPos;
   }
   if(k == '1'){
     glDrawStyle = 1;
   }
   if(k == '2'){
     glDrawStyle = 2;
   }
   if(k == '3'){
     glDrawStyle = 3;
   }
   if(k == '4'){
     glDrawStyle = 4;
   }
   if(k == '&')
     showPoint = !showPoint;

  if(k == 'h' || k == 'H')
    helpWindow = !helpWindow;

	glutPostRedisplay();
}


/* Callback OpenGL de gestion des touches speciales de claviers */
void keyboardSpecialGL(int k, int x, int y)
{
	glutPostRedisplay();
}


/* Callback OpenGL de gestion de drag */
void motionGL(int x, int y)
{
  int i,j;
  /* Mise à jour de la position de la souris */
  if(mouseDown && selectedPoint != -1){
    selectedPolygon = -1;
	 listPoint.points[selectedPoint].pos[0] = x;
   listPoint.points[selectedPoint].pos[1] = winY - y;
  }
  else if(mouseDown && selectedPolygon != -1){
    selectedPoint = -1;
    for(i = 0; i < listPolygon.polygons[selectedPolygon].nbPoint-1; i++){
      listPolygon.polygons[selectedPolygon].points[i]->pos[0] += x-lastMouseX;
      listPolygon.polygons[selectedPolygon].points[i]->pos[1] += lastMouseY-y;
    }
    for(i = 0; i < listPolygon.polygons[selectedPolygon].nbHole; i++){
      for(j = 0; j <  listPolygon.polygons[selectedPolygon].holes[i]->nbPoint-1; j++){
        listPolygon.polygons[selectedPolygon].holes[i]->points[j]->pos[0] += x - lastMouseX;
        listPolygon.polygons[selectedPolygon].holes[i]->points[j]->pos[1] += lastMouseY - y;
      }
    }
  }
  lastMouseX = x;
  lastMouseY = y;
	glutPostRedisplay();
}


/* Callback OpenGL de gestion de survol */
void passiveMotionGL(int x, int y)
{
  int i,j;
  Point a, b, c;
  mouseX = x;
  mouseY = winY-y;
  lastMouseX = x;
  lastMouseY = y;

  /* Verification que le segment en construction n'est secant a aucun autre segment sauf le precedent */
  if(createPoint && listPolygon.polygons[listPolygon.nbPolygon].nbPoint > 0){
    Point p = createPoint(x, y);
    polygonCanBeClose = 1;
    for(i = 0; i < listPolygon.nbPolygon; i++){
      for(j = 0; j < listPolygon.polygons[i].nbPoint-1; j++){
        a.pos[0] = listPolygon.polygons[i].points[j]->pos[0];
        a.pos[1] = listPolygon.polygons[i].points[j]->pos[1];
        b.pos[0] = listPolygon.polygons[i].points[j+1]->pos[0];
        b.pos[1] = listPolygon.polygons[i].points[j+1]->pos[1];
        c.pos[0] = lastPoint.pos[0];
        c.pos[1] = lastPoint.pos[1];
        if(!secant(a, b, c, p))
          polygonCanBeClose = 0;
      }
    }
    if(listPolygon.polygons[listPolygon.nbPolygon].nbPoint > 1)
    for(i = 0; i < listPolygon.polygons[listPolygon.nbPolygon].nbPoint-2; i++){
      a.pos[0] = listPolygon.polygons[listPolygon.nbPolygon].points[i]->pos[0];
      a.pos[1] = listPolygon.polygons[listPolygon.nbPolygon].points[i]->pos[1];
      b.pos[0] = listPolygon.polygons[listPolygon.nbPolygon].points[i+1]->pos[0];
      b.pos[1] = listPolygon.polygons[listPolygon.nbPolygon].points[i+1]->pos[1];
      c.pos[0] = lastPoint.pos[0];
      c.pos[1] = lastPoint.pos[1];
      if(!secant(a, b, c, p))
        polygonCanBeClose = 0;
    }

    /* Verification que le segment en construction ne soit sécant avec aucun point du polygone excepté le premier */
    for(i = 1; i < listPolygon.polygons[listPolygon.nbPolygon].nbPoint; i++){
      if(intersect(listPolygon.polygons[listPolygon.nbPolygon].points[i], p))
        polygonCanBeClose = 0;
    }
  }
  glutPostRedisplay();
}


/* Fonction d'initialisation des variables */
void init()
{
  mouseDown = 0;
  selectedPoint = -1;
  selectedPolygon = -1;
  listPolygon.nbPolygon = 0;
  showPointPos = 0;
  polygonCanBeClose = 1;
  glDrawStyle = 2;
  showPoint = 1;
  tool = 0;
  helpWindow = 0;
  helpWindowX = 49;
  helpWindowY = winY-71;
}


/* Fonction d'initialisation OpenGL */
void initGL()
{
	glClearColor(0.8, 0.8, 0.7, 1.0);
  glPointSize(POINT_SIZE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );
}


/* Fonction d'affichage du manuel */
void usage()
{
	printf("---------------------------------------------------------------------------\n");
	printf("%c[%dm", 27, 1);
	printf("%s\n", TP_TITLE);
	printf("%c[%dm", 27, 0);
	printf("%s (%s)\n", TP_GROUP, TP_PROMO);
	printf("\n");

	printf("%c[%dm", 27, 1);
	printf("[Utilisation]\n");
	printf("%c[%dm", 27, 0);
	printf("    Clic gauche   :\t ajouter point\n");
  printf("    Clic droit    :\t  supprimer point\n");
	printf("\n");

	printf("%c[%dm", 27, 1);
	printf("[Manuel]\n");
	printf("%c[%dm", 27, 0);
  printf("    'c | C'       :\t Afficher / masquer les positions des points\n");
  printf("    'a | A'       :\t Afficher / masquer points\n");
  printf("    'p | P'       :\t Passer mode Polygone / Point\n");
  printf("    '1 2 3 4'     :\t Changer mode dessin\n");
	printf("    'q'/Esc       :\t Quitter\n");
	printf("---------------------------------------------------------------------------\n");
}


/* Fonction principale */
int main(int argc, char ** argv)
{
	int posX, posY;

	glutInit(&argc, argv);
	usage();

	posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
	posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;

	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(posX, posY);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(TP_TITLE);

	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);

	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutSpecialFunc(keyboardSpecialGL);

	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);

	init();
	initGL();
	glutMainLoop();

	return 0;
}
