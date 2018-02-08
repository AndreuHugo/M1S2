#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <cmath>

#include "mesh.h"

float parseFloat(std::ifstream& s) {
  char f_buf[sizeof(float)];
  s.read(f_buf, 4);
  float* fptr = (float*) f_buf;
  return *fptr;
}

Point parsePoint(std::ifstream& s) {
  float x = parseFloat(s);
  float y = parseFloat(s);
  float z = parseFloat(s);
  return Point(x, y, z);
}

double Mesh::support(){
  double weight = 0;
  Point a(0, -0.5, 0);
  for (auto t : this->triangles) {
		if(t.normal.angle(a) < 45 ){
      weight+= t.area;
    }
  }
  return weight;
}

void Mesh::rotate(double theta, int x, int y, int z){
  int i,j;
  theta = theta *M_PI/180;
  for(i = 0; i < this->triangles.size(); i++){
    this->triangles[i].normal.rotateX(theta*x);
    this->triangles[i].normal.rotateY(theta*y);
    this->triangles[i].normal.rotateZ(theta*z);
    for(j = 0; j < 3; j++){
      if(x == 1)
        this->triangles[i].points[j].rotateX(theta);
      if(y == 1)
        this->triangles[i].points[j].rotateY(theta);
      if(z == 1)
        this->triangles[i].points[j].rotateZ(theta);
    }
  }
}

void Mesh::copy(Mesh * dest){
  for(int i = 0;i < this->triangles.size(); i++){
    dest->triangles[i].normal = this->triangles[i].normal;
    dest->triangles[i].area = this->triangles[i].area;
    for(int j = 0; j < 3; j++){
      dest->triangles[i].points[j] = this->triangles[i].points[j];
    }
  }
}

void Mesh::load(const std::string& stl_path) {
  std::ifstream stl_file(stl_path.c_str(), std::ios::in | std::ios::binary);
  if (!stl_file) {
    std::cout << "ERROR: COULD NOT READ FILE" << std::endl;
    assert(false);
  }

  char header_info[80] = "";
  char n_triangles[4];
  stl_file.read(header_info, 80);
  stl_file.read(n_triangles, 4);
  std::string h(header_info);
  this->name = h;
  unsigned int* r = (unsigned int*) n_triangles;
  unsigned int num_triangles = *r;
  for(unsigned int i = 0; i < num_triangles; i++) {
    auto normal = parsePoint(stl_file);
    auto v1 = parsePoint(stl_file);
    auto v2 = parsePoint(stl_file);
    auto v3 = parsePoint(stl_file);
    this->triangles.push_back(Triangle(normal, v1, v2, v3));
    char dummy[2];
    stl_file.read(dummy, 2);
  }
}

void Mesh::normalize() {
  Point pos;
  Point min, max;
  double sca = 1.0;

  min.copy(this->triangles[0].points[0]);
  max.copy(this->triangles[0].points[0]);

  for (auto t : this->triangles) {
    for(int i = 0; i < 3; i++) {
      if(t.points[i].x < min.x) min.x = t.points[i].x;
      if(t.points[i].x > max.x) max.x = t.points[i].x;

      if(t.points[i].y < min.y) min.y = t.points[i].y;
      if(t.points[i].y > max.y) max.y = t.points[i].y;

      if(t.points[i].z < min.z) min.z = t.points[i].z;
      if(t.points[i].z > max.z) max.z = t.points[i].z;
    }
  }

  pos.copy(min);
  pos.add(max);
  pos.scale(0.5);

  Point size(fabs(max.x-min.x), fabs(max.y-min.y), fabs(max.z-min.z));

  if(size.x > size.y) {
		if(size.x > size.z)
			sca = 1.0 / size.x;
		else
			sca = 1.0 / size.z;
	}
	else {
		if(size.y > size.z)
			sca = 1.0 / size.y;
		else
			sca = 1.0 / size.z;
	}

  for(int j = 0; j < this->triangles.size(); j++) {
    for(int i = 0; i < 3; i++) {
      this->triangles[j].points[i].sub(pos);
      this->triangles[j].points[i].scale(sca);
    }
  }

  for(int i = 0; i < this->triangles.size(); i++){
    Point ab;
    ab.x = this->triangles[i].points[1].x-this->triangles[i].points[0].x;
    ab.y = this->triangles[i].points[1].y-this->triangles[i].points[0].y;
    ab.z = this->triangles[i].points[1].z-this->triangles[i].points[0].z;
    Point ac;
    ac.x = this->triangles[i].points[2].x-this->triangles[i].points[0].x;
    ac.y = this->triangles[i].points[2].y-this->triangles[i].points[0].y;
    ac.z = this->triangles[i].points[2].z-this->triangles[i].points[0].z;
    this->triangles[i].area =  ab.cross(ac)/2;
  }

}
