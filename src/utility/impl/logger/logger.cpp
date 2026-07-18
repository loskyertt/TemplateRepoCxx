/**
 * @File    :   src/utility/impl/logger.cpp
 * @Time    :   2026/04/14 16:23:05
 * @Author  :   loskyertt
 * @Github  :   https://github.com/loskyertt
 * @Desc    :   .....
 */

#include "logger/logger.h"

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace sky::utility;

const char *Logger::s_level[static_cast<size_t>(Level::LEVEL_COUNT)] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

Logger::Logger() : m_level(Level::DEBUG), m_len(0), m_max(0), m_auto_flush(true) {}

Logger::~Logger() {
  close();
}

// Logger &Logger::getInstance() {
//   static Logger instance;
//   return instance;
// }

void Logger::open(const std::string &filename) {
  m_filename = filename;
  m_fout.open(filename, std::ios::app);
  if (m_fout.fail()) {
    throw std::logic_error("open log file failed: " + filename);
  }

  m_fout.seekp(0, std::ios::end);
  m_len = static_cast<int>(m_fout.tellp());
}

void Logger::close() {
  m_fout.close();
}

void Logger::log(Level level, const char *file, int line, const char *format, ...) {
  if (level < m_level) {
    return;
  }

  if (m_fout.fail()) {
    throw std::logic_error("log file failed: " + m_filename);
  }

  // 获取当前时间
  time_t now = time(0);
  struct tm *ptm_struct = localtime(&now);
  char time_str[32];
  std::memset(time_str, 0, sizeof(time_str));
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm_struct);

  const char *fmt = "[%s] %s %s:%d ";  // 时间、日志级别、文件名、行号
  // 先计算格式化后的字符串长度
  int size = std::snprintf(nullptr, 0, fmt, time_str, s_level[static_cast<size_t>(level)], file, line);
  if (size > 0) {
    char *buffer = new char[size + 1];  // 包括 \0
    std::snprintf(buffer, size + 1, fmt, time_str, s_level[static_cast<size_t>(level)], file, line);
    buffer[size] = '\0';  // 确保字符串以 \0 结尾
    m_fout << buffer;
    m_len += size;
    delete[] buffer;
  }

  va_list arg_ptr;
  va_start(arg_ptr, format);
  size = std::vsnprintf(nullptr, 0, format, arg_ptr);
  va_end(arg_ptr);
  if (size > 0) {
    char *content = new char[size + 1];
    va_start(arg_ptr, format);
    std::vsnprintf(content, size + 1, format, arg_ptr);
    va_end(arg_ptr);
    content[size] = '\0';
    m_fout << content;
    m_len += size;
    delete[] content;
  }
  m_fout << "\n";
  if (m_auto_flush) {
    m_fout.flush();
  }

  if (m_len >= m_max && m_max > 0) {
    rotate();
  }
}

void Logger::rotate() {
  close();  // 关闭之前打开的文件

  // 获取当前时间
  time_t now = time(0);
  struct tm *ptm_struct = localtime(&now);
  char time_str[32];
  std::memset(time_str, 0, sizeof(time_str));
  strftime(time_str, sizeof(time_str), ".%Y-%m-%d_%H-%M-%S", ptm_struct);

  std::string filename = m_filename + time_str;
  // rename 是重命名磁盘上的 test.log 文件
  if (rename(m_filename.c_str(), filename.c_str()) != 0) {
    throw std::logic_error("failed to rename log file: " + std::string(strerror(errno)));
  }
  open(m_filename);
}
