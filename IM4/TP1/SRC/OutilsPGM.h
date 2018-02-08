#define MAXLIGNE 256

typedef float Pixel;

typedef struct
	{
	int width, height, size;
	Pixel *data;
	}Image;

typedef struct Filter Filter;
struct Filter{
	int size;
	float ** mat;
};

int CreerImage(Image *image, int largeur, int hauteur);
int LireImage(char *nom, Image *image);
int EcrireImage(char *nom, Image *image);
void CopierImage(Image * I, Image * O);
void glPrintText(int x, int y, const char * text);
double getPixelValueAt(Image * img, int x, int y);
void setPixelValueAt(Image * img, int x, int y, double value);
int isInside(int x, int y, int bx1, int bx2, int by1, int by2);
void basculeImage(GLubyte *I, Image * img);
void showImage(Image * img);
void LibererImage(Image *i);
