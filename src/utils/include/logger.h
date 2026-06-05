#pragma once
#include <string>

// 轻量日志工具，供各模块使用
class Logger {
 public:
  void info(const std::string &msg) const;
  void warn(const std::string &msg) const;
  void error(const std::string &msg) const;
};
