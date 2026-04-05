#include "circle.h"
#include <cmath>

constexpr double PI = 3.14159265358979;

Circle::Circle(double radius) : radius_(radius) {}

double Circle::area() const {
  return PI * radius_ * radius_;
}

double Circle::perimeter() const {
  return 2 * PI * radius_;
}

double Circle::radius() const {
  return radius_;
}
