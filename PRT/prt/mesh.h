#ifndef PARSE_STL_H
#define PARSE_STL_H

#include <string>
#include <vector>

#include "point.h"

using namespace std;

class Triangle {
public:
  Point normal;
  Point points[3];
  double area;

  Triangle(Point normalp, Point v1p, Point v2p, Point v3p) : normal(normalp), points{v1p, v2p, v3p} {}
};

class Mesh {
public:
  string name;
  vector<Triangle> triangles;

  Mesh(string namep) : name(namep) {}

  void copy(Mesh * dest);
  double support();
  void rotate(double theta, int x, int y, int z);
  void load(const string& stl_path);
  void parse(const string& stl_path);
  void normalize();
};

#endif
