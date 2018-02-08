#include <cmath>

#include "trackball.h"

static int		tbW		= 0;
static int		tbH		= 0;
static int		tbX		= 0;
static int		tbY		= 0;
static double	tbScale	= 1.0;
static mlVec4	tbQuat;
static mlMat4	tbMatrix;


void mlMat4Set(mlMat4 out, double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o, double p) {
	out[ 0] = a;
	out[ 1] = b;
	out[ 2] = c;
	out[ 3] = d;
	out[ 4] = e;
	out[ 5] = f;
	out[ 6] = g;
	out[ 7] = h;
	out[ 8] = i;
	out[ 9] = j;
	out[10] = k;
	out[11] = l;
	out[12] = m;
	out[13] = n;
	out[14] = o;
	out[15] = p;
}

void mlMat4Copy(mlMat4 out, const mlMat4 in) {
	mlMat4Set(out, in[ 0], in[ 1], in[ 2], in[ 3],
					 in[ 4], in[ 5], in[ 6], in[ 7],
					 in[ 8], in[ 9], in[10], in[11],
					 in[12], in[13], in[14], in[15]);
}

void mlMat4Identity(mlMat4 out) {
	mlMat4Set(out, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

void mlMat4MultMat(mlMat4 out, const mlMat4 in1, const mlMat4 in2) {
	mlMat4 m;

	m[ 0] = in1[ 0] * in2[ 0] + in1[ 1] * in2[ 4] + in1[ 2] * in2[ 8] + in1[ 3] * in2[12];
	m[ 1] = in1[ 0] * in2[ 1] + in1[ 1] * in2[ 5] + in1[ 2] * in2[ 9] + in1[ 3] * in2[13];
	m[ 2] = in1[ 0] * in2[ 2] + in1[ 1] * in2[ 6] + in1[ 2] * in2[10] + in1[ 3] * in2[14];
	m[ 3] = in1[ 0] * in2[ 3] + in1[ 1] * in2[ 7] + in1[ 2] * in2[11] + in1[ 3] * in2[15];
	m[ 4] = in1[ 4] * in2[ 0] + in1[ 5] * in2[ 4] + in1[ 6] * in2[ 8] + in1[ 7] * in2[12];
	m[ 5] = in1[ 4] * in2[ 1] + in1[ 5] * in2[ 5] + in1[ 6] * in2[ 9] + in1[ 7] * in2[13];
	m[ 6] = in1[ 4] * in2[ 2] + in1[ 5] * in2[ 6] + in1[ 6] * in2[10] + in1[ 7] * in2[14];
	m[ 7] = in1[ 4] * in2[ 3] + in1[ 5] * in2[ 7] + in1[ 6] * in2[11] + in1[ 7] * in2[15];
	m[ 8] = in1[ 8] * in2[ 0] + in1[ 9] * in2[ 4] + in1[10] * in2[ 8] + in1[11] * in2[12];
	m[ 9] = in1[ 8] * in2[ 1] + in1[ 9] * in2[ 5] + in1[10] * in2[ 9] + in1[11] * in2[13];
	m[10] = in1[ 8] * in2[ 2] + in1[ 9] * in2[ 6] + in1[10] * in2[10] + in1[11] * in2[14];
	m[11] = in1[ 8] * in2[ 3] + in1[ 9] * in2[ 7] + in1[10] * in2[11] + in1[11] * in2[15];
	m[12] = in1[12] * in2[ 0] + in1[13] * in2[ 4] + in1[14] * in2[ 8] + in1[15] * in2[12];
	m[13] = in1[12] * in2[ 1] + in1[13] * in2[ 5] + in1[14] * in2[ 9] + in1[15] * in2[13];
	m[14] = in1[12] * in2[ 2] + in1[13] * in2[ 6] + in1[14] * in2[10] + in1[15] * in2[14];
	m[15] = in1[12] * in2[ 3] + in1[13] * in2[ 7] + in1[14] * in2[11] + in1[15] * in2[15];

	mlMat4Copy(out, m);
}

double mlClamp(double x, double a, double b) {
	return (x < a) ? a : ((x > b) ? b : x);
}

double mlSquare(double x) {
	return x * x;
}

void mlVec3Set(mlVec3 in, double x, double y, double z) {
	in[0] = x;
	in[1] = y;
	in[2] = z;
}

void mlVec3Copy(mlVec3 out, const mlVec3 in) {
	mlVec3Set(out, in[0], in[1], in[2]);
}

void mlVec3Scale(mlVec3 out, const mlVec3 in, double k) {
	mlVec3Set(out, in[0] * k, in[1] * k, in[2] * k);
}

double mlVec3Dot(const mlVec3 in1, const mlVec3 in2) {
	return in1[0] * in2[0] + in1[1] * in2[1] + in1[2] * in2[2];
}

void mlVec3Cross(mlVec3 out, const mlVec3 in1, const mlVec3 in2) {
	mlVec3 v;

	v[0] = in1[1] * in2[2] - in1[2] * in2[1];
	v[1] = in1[2] * in2[0] - in1[0] * in2[2];
	v[2] = in1[0] * in2[1] - in1[1] * in2[0];

	mlVec3Copy(out, v);
}

double mlVec3Norm2(const mlVec3 in) {
	return in[0] * in[0] + in[1] * in[1] + in[2] * in[2];
}

double mlVec3Norm(const mlVec3 in) {
	return sqrt(mlVec3Norm2(in));
}

void mlVec3Normalize(mlVec3 out, const mlVec3 in) {
	double norm = mlVec3Norm(in);

	if(fabs(norm) > mlEps)
		mlVec3Scale(out, in, 1.0 / norm);
	else
		mlVec3Set(out, 1.0, 0.0, 0.0);
}

void mlVec4Set(mlVec4 in, double w, double x, double y, double z) {
	in[0] = w;
	in[1] = x;
	in[2] = y;
	in[3] = z;
}

void mlVec4Zero(mlVec4 out) {
	mlVec4Set(out, 0.0, 0.0, 0.0, 0.0);
}

void mlVec4Copy(mlVec4 out, const mlVec4 in) {
	mlVec4Set(out, in[0], in[1], in[2], in[3]);
}

void mlVec4Scale(mlVec4 out, const mlVec4 in, double k) {
	mlVec4Set(out, in[0] * k, in[1] * k, in[2] * k, in[3] * k);
}

double mlVec4Norm2(const mlVec4 in) {
	return in[0] * in[0] + in[1] * in[1] + in[2] * in[2] + in[3] * in[3];
}

double mlVec4Norm(const mlVec4 in) {
	return sqrt(mlVec4Norm2(in));
}

void tbQuatNormalize(mlVec4 qout, mlVec4 qin) {
	mlVec4 r;
	double n = mlVec4Norm(qin);

	if(n < mlEps) {
		mlVec4Zero(r);
		r[0] = 1.0;
	}
	else {
		mlVec4Scale(r, qin, 1.0 / n);
	}

	mlVec4Copy(qout, r);
}


void tbToSphere(mlVec3 v, int x, int y) {
	v[0] = (2.0 * x - tbW) / tbW;
	v[1] = (tbH - 2.0 * y) / tbH;
	v[2] = 1.0 - mlSquare(v[0]) - mlSquare(v[1]);

	if(v[2] > 0.0)
		v[2] = sqrt(v[2]);
	else
		v[2] = 0.0;

	mlVec3Normalize(v, v);
}


void tbToQuat(mlVec4 q, int ox, int oy, int nx, int ny) {
	mlVec3 ov, nv, axis;
	double cosx, sinx;

	tbToSphere(ov, ox, oy);
	tbToSphere(nv, nx, ny);

	mlVec3Cross(axis, nv, ov);
	mlVec3Normalize(axis, axis);

	cosx = mlVec3Dot(ov, nv);
	cosx = mlClamp(cosx, -1.0, 1.0);
	sinx = sqrt(1.0 - mlSquare(cosx));

	q[0] = cosx;
	q[1] = sinx * axis[0];
	q[2] = sinx * axis[1];
	q[3] = sinx * axis[2];
}


void tbToMatrix(mlMat4 m, mlVec4 q) {
	double xw = q[1] * q[0];
	double xx = q[1] * q[1];
	double xy = q[1] * q[2];
	double xz = q[1] * q[3];

	double yw = q[2] * q[0];
	double yy = q[2] * q[2];
	double yz = q[2] * q[3];

	double zw = q[3] * q[0];
	double zz = q[3] * q[3];

	m[ 0] = 1.0 - 2.0 * (yy + zz);
	m[ 1] = 	   2.0 * (xy - zw);
	m[ 2] = 	   2.0 * (xz + yw);
	m[ 3] = 0.0;

	m[ 4] = 	   2.0 * (xy + zw);
	m[ 5] = 1.0 - 2.0 * (xx + zz);
	m[ 6] = 	   2.0 * (yz - xw);
	m[ 7] = 0.0;

	m[ 8] = 	   2.0 * (xz - yw);
	m[ 9] = 	   2.0 * (yz + xw);
	m[10] = 1.0 - 2.0 * (xx + yy);
	m[11] = 0.0;

	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
}


void tbUpdate(int nx, int ny) {
	mlVec4 q, r;
	mlMat4 s;

	tbToQuat(q, tbX, tbY, nx, ny);
	tbQuatNormalize(q, q);

	r[0] = tbQuat[0] * q[0] - tbQuat[1] * q[1] - tbQuat[2] * q[2] - tbQuat[3] * q[3];
	r[1] = tbQuat[0] * q[1] + tbQuat[1] * q[0] + tbQuat[2] * q[3] - tbQuat[3] * q[2];
	r[2] = tbQuat[0] * q[2] - tbQuat[1] * q[3] + tbQuat[2] * q[0] + tbQuat[3] * q[1];
	r[3] = tbQuat[0] * q[3] + tbQuat[1] * q[2] - tbQuat[2] * q[1] + tbQuat[3] * q[0];

	mlVec4Copy(tbQuat, r);
	tbToMatrix(tbMatrix, tbQuat);

	mlMat4Identity(s);
	s[ 0] = tbScale;
	s[ 5] = tbScale;
	s[10] = tbScale;

	mlMat4MultMat(tbMatrix, s, tbMatrix);

	tbX = nx;
	tbY = ny;
}


void tbInit(int w, int h) {
	tbW = w;
	tbH = h;
	tbX = 0;
	tbY = 0;

	mlVec4Zero(tbQuat);
	tbQuat[0] = 1.0;

	tbToMatrix(tbMatrix, tbQuat);
}


void tbClick(int x, int y) {
	tbX = x;
	tbY = y;

	tbUpdate(x, y);
}


void tbMotion(int x, int y) {
	tbUpdate(x, y);
}


void tbRelease(int x, int y) {
	tbUpdate(x, y);
}


void tbZoom(double zoom) {
	tbScale += zoom;

	if(tbScale < mlEps)
		tbScale = mlEps;

	tbUpdate(tbX, tbY);
}


double * tbGetTransformation()
{
	return tbMatrix;
}
