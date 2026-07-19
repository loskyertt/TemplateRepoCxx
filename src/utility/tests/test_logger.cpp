/**
 * @File    :   src/utility/test/test_logger.cpp
 * @Time    :   2026/04/14 20:28:45
 * @Author  :   loskyertt
 * @Github  :   https://github.com/loskyertt
 * @Desc    :   Logger 测试
 */

#include "logger/logger.h"

using namespace sky::utility;

int main() {
  auto &logger = Singleton<Logger>::instance();

  logger.open("log/test_logger.log");
  // logger.set_level(Singleton<Logger>::Level::ERROR);
  logger.set_max(1000);

  Log_debug("name is %s, age is %d", "sky", 22);
  Log_info("name is %s, age is %d", "sky", 22);
  Log_warn("name is %s, age is %d", "sky", 22);
  Log_error("name is %s, age is %d", "sky", 22);
  Log_fatal("name is %s, age is %d", "sky", 22);
  return 0;
}
