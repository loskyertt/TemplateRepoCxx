#pragma once
#include "shape.h"

class Circle : public Shape {
 public:
  explicit Circle(double radius);

  double area() const override;
  double perimeter() const override;
  double radius() const;

 private:
  double radius_;
};
