#include "logger.h"
#include <iostream>

void Logger::info(const std::string &msg) const {
  std::cout << "[INFO] " << msg << '\n';
}

void Logger::warn(const std::string &msg) const {
  std::cout << "[WARN]  " << msg << '\n';
}

void Logger::error(const std::string &msg) const {
  std::cerr << "[ERROR] " << msg << '\n';
}
