#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>
#include "OutilsPGM.h"

int CreerImage(Image *image, int largeur, int hauteur)
        {
        image->width=largeur;
        image->height=hauteur;
        image->size=largeur*hauteur;
        image->data=(Pixel*)malloc(image->size * sizeof(Pixel));
        if (image->data == NULL)
                {
                printf ("ERREUR malloc (%lu)\n", image->size * sizeof(Pixel));
                return -1;
                }
        return 0;
        }

int LireImage(char *nom, Image *image)
        {
        FILE *f;
        char s[MAXLIGNE];
        int width, height, maxgrey, n, k;
        unsigned char ligne[2048];

        /* Ouverture du fichier */
        f = fopen (nom, "rb");
        if (f == NULL)
                {
                printf ("ERREUR fopen(%s)\n", nom);
                return -1;
                }

        /* Lecture MagicValue
         * On evite fscanf pour eviter buffer overflow */
        fgets (s, MAXLIGNE, f);
        if (s[0] != 'P' || s[1] != '5')
                {
                printf ("ERREUR MagicValue <> P5\n");
                fclose (f);
                return -1;
                }

        /* Lecture width height
         * On saute les ligne commen�ant par un '#' */
        do fgets (s, MAXLIGNE, f);
        while (s[0] == '#');
        sscanf (s, "%d %d", &width, &height);

        /* Lecture maxgrey
         * On ne tient pas compte de maxgrey, qui est presque toujours 255.
         * On saute les ligne commen�ant par un '#' */
        do fgets (s, MAXLIGNE, f);
        while (s[0] == '#');
        sscanf (s, "%d", &maxgrey);

        /* Allocation Image */
        if (CreerImage (image, width, height) < 0)
                {
                fclose (f);
                return -1;
                }

        /* Lecture image */
        for (n=0;n<height;n++)
                {
                if (fread(ligne,sizeof(unsigned char)*width,1,f)!=1)
                        return -1; /* Erreur de lecture */
                for (k=0;k<width;k++)
                        image->data[n*width+k]=(Pixel)ligne[k];
                }

        fclose (f);
        return 0;
        }

int EcrireImage(char *nom, Image *image)
        {
        FILE *f;
        int n,k;
        unsigned char ligne[2048];

        /* Ouverture du fichier */
        f = fopen (nom, "wb");
        if (f == NULL)
                {
                printf ("ERREUR fopen(%s)\n", nom);
                return -1;
                }

        /* Ecriture du header */
        fprintf (f, "P5\n");
        fprintf (f, "# width height\n");
        fprintf (f, "%d %d\n", image->width, image->height);
        fprintf (f, "255\n");

        /* Ecriture du tableau */
        for (n=0;n<image->height;n++)
                {
                for (k=0;k<image->width;k++)
                        ligne[k]=(unsigned char)image->data[n*image->width+k];
                if (fwrite (ligne, sizeof(unsigned char)*image->width,1,f)!=1)
                        {
                        printf ("ERREUR fwrite()\n");
                        fclose (f);
                        return -1;
                        }
                }

        fclose (f);
        return 0;
        }

void CopierImage(Image * I, Image * O){
  int i;
  for(i = 0; i < I->size; i++)
    O->data[i] = I->data[i];
}

/* Fonction pour afficher du texte en OpenGL */
void glPrintText(int x, int y, const char * text)
{
	int i;
	glRasterPos2i(x, y);
	for(i = 0; i < (int)strlen(text); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
}

double getPixelValueAt(Image * img, int x, int y){
  return img->data[x + img->width * y];
}

void setPixelValueAt(Image * img, int x, int y, double value){
  img->data[x + img->width * y] = value;
}

int isInside(int x, int y, int bx1, int bx2, int by1, int by2){
  if(x > bx1 && y > by1 && x < bx2 && y < by2)
    return 1;
  return 0;
}

void basculeImage(GLubyte *I, Image * img) {
  for(int i = 0; i < img->height; i++)
    for(int j = 0; j < img->width; j++)
      I[(img->height-1-i) * img->width + j] = (GLubyte) getPixelValueAt(img, j, i);
}

void showImage(Image * img) {
  GLubyte *I = NULL;
  I = malloc(img->height * img->width * sizeof(GLubyte));
  glClear(GL_COLOR_BUFFER_BIT);
  basculeImage(I, img);
  glDrawPixels(img->width, img->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, I);
  glFlush();
}


void LibererImage(Image *i)
        {
        if (i->data != NULL) free(i->data);
        i->data = NULL;
        }
