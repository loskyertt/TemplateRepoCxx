#pragma once

// 所有形状的抽象基类（对外公开接口）
class Shape {
 public:
  virtual ~Shape() = default;
  virtual double area() const = 0;
  virtual double perimeter() const = 0;
};
