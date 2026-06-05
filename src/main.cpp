#include "circle.h"
#include "rectangle.h"
#include "logger.h"

#include <print>

int main() {
  std::println("hello project");

  Logger logger;

  Circle c(5.0);
  logger.info("Circle area: " + std::to_string(c.area()));

  Rectangle r(4.0, 6.0);
  logger.info("Rectangle area: " + std::to_string(r.area()));

  return 0;
}
