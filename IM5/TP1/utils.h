/* Taille de la fenetre */
int winX = 800;
int winY = 600;

/* Nombre maximum de points de controle */
#define MAX_POINTS	200
#define MAX_POLYGONS	100

#define POINT_SIZE 10

typedef struct Point{
  float pos[3];
  int id;
} Point;

typedef struct Polygon{
  Point * points[MAX_POINTS];
  struct Polygon * holes[MAX_POLYGONS];
  int nbHole;
  int nbPoint;
  int id;
} Polygon;

typedef struct ListPoint{
  Point points[MAX_POINTS];
  int nbPoint;
} ListPoint;

typedef struct ListPolygon{
  Polygon polygons[MAX_POLYGONS];
  int nbPolygon;
} ListPolygon;

ListPoint listPoint;
ListPolygon listPolygon;
Point lastPoint;

int mouseX, mouseY;
int lastMouseX, lastMouseY;
int clicX, clicY;

int mouseDown;
int selectedPoint;
int selectedPolygon;
int showPointPos;
int polygonCanBeClose;
int glDrawStyle;
int showPoint;
int tool;
int helpWindow;
int helpWindowX;
int helpWindowY;
