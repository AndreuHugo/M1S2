#ifndef TRACKBALL
#define TRACKBALL

static const double mlEps = 1.0e-09;
typedef double mlVec3[3];
typedef double mlVec4[4];
typedef double mlMat4[16];

void tbInit(int w, int h);													/* Initialize the trackball with the dimensions of the current window */

void tbClick(int x, int y);												/* Record a simple mouse click event at the point (x, y) */
void tbMotion(int x, int y);												/* Rotate the object according to the new coordinates (x, y) */
void tbRelease(int x, int y);												/* Record a simple mouse release event at the point (x, y) */
void tbZoom(double zoom);													/* Just a zoom... */

double * tbGetTransformation();													/* Return a pointer to the transformation matrix (to send to the glMultMatrixd(...) procedure) */


#endif
