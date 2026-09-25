#include "circle.h"
#include "rectangle.h"
#include "logger/logger.h"

#include <print>

using sky::utility::Logger;
using sky::utility::Singleton;

int main() {
  auto& logger = Singleton<Logger>::instance();
  logger.open("log/core.log");
  logger.set_max(1000);

  std::println("hello project");

  Circle c(5.0);
  Log_info("Circle area: %d", c.area());

  Rectangle r(4.0, 6.0);
  Log_info("Rectangle area: %d", r.area());

  return 0;
}
