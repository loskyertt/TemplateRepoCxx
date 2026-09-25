#pragma once
#include "shape.h"

class Rectangle : public Shape {
 public:
  Rectangle(double width, double height);

  double area() const override;
  double perimeter() const override;

 private:
  double width_, height_;
};
