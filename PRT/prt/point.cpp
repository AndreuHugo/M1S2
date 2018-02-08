#include <cmath>
#include <iostream>

#include "point.h"

using namespace std;

Point::Point() : x(0), y(0) , z(0)
{}

Point::Point(double x, double y, double z) : x(x), y(y), z(z)
{}



void Point::rotateZ(double theta){
  double x = this->x;
  double y = this->y;
  this->x = x*cos(theta)-y*sin(theta);
  this->y = x*sin(theta)+y*cos(theta);
}
void Point::rotateX(double theta){
  double z = this->z;
  double y = this->y;
  this->z = y*sin(theta)+z*cos(theta);
  this->y = y*cos(theta)-z*sin(theta);
}
void Point::rotateY(double theta){
  double x = this->x;
  double z = this->z;
  this->x = x*cos(theta)+z*sin(theta);
  this->z = -x*sin(theta)+z*cos(theta);
}

void Point::copy(Point a) {
  this->x = a.x;
  this->y = a.y;
  this->z = a.z;
}

void Point::add(Point a) {
  this->x += a.x;
  this->y += a.y;
  this->z += a.z;
}

void Point::sub(Point a) {
  this->x -= a.x;
  this->y -= a.y;
  this->z -= a.z;
}

void Point::scale(double k) {
  this->x *= k;
  this->y *= k;
  this->z *= k;
}

void Point::neg() {
  this->x = -this->x;
  this->y = -this->y;
  this->z = -this->z;
}

void Point::normalize() {
  double n = norm();

  if(fabs(n) > 1.0e-09)
    scale(1.0 / n);
	else {
    this->x = 1.0;
    this->y = 0.0;
    this->z = 0.0;
  }
}

double Point::norm() {
  return sqrt(norm2());
}

double Point::norm2() {
  return this->x * this->x + this->y * this->y + this->z * this->z;
}

double Point::distance(Point a) {
  return sqrt((this->x - a.x) * (this->x - a.x) + (this->y - a.y) * (this->y - a.y) + (this->z - a.z) * (this->z - a.z));
}

double Point::dot(Point a) {
  return (this->x * a.x) + (this->y * a.y) + (this->z * a.z);
}

double Point::cross(Point a){
  return sqrt(pow(this->y*a.z-this->z*a.y,2)+pow(this->z*a.x-this->x*a.z,2)+pow(this->x*a.y-this->y*a.x,2));
}

double Point::angle(Point a) {
  return acos(this->dot(a)/sqrt(this->norm2() * a.norm2())) * (180.0/M_PI);
}


void Point::print() const {
    cout << "X : " << this->x << endl;
    cout << "Y : " << this->y << endl;
    cout << "Z : " << this->z << endl;
}
