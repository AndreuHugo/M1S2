#ifndef POINT_H
#define POINT_H

#include <iostream>
using namespace std;

class Point {
public:
  double x, y, z;

  Point();
  Point(double x, double y, double z);

  void rotateX(double theta);
  void rotateY(double theta);
  void rotateZ(double theta);
  void copy(Point a);
  void add(Point a);
  void sub(Point a);
  void scale(double k);
  void neg();
  void normalize();
  double norm();
  double norm2();
  double dot(Point a);
  double distance(Point a);
  double angle(Point a);
  double cross(Point a);

  void print() const;
};

#endif
