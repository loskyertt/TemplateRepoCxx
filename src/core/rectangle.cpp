#include "rectangle.h"

Rectangle::Rectangle(double w, double h) : width_(w), height_(h) {}

double Rectangle::area() const {
  return width_ * height_;
}

double Rectangle::perimeter() const {
  return 2 * (width_ + height_);
}
