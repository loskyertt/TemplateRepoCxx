/**
 * @File    :   src/utility/include/logger/logger.h
 * @Time    :   2026/04/14 16:23:10
 * @Author  :   loskyertt
 * @Github  :   https://github.com/loskyertt
 * @Desc    :   轻量日志工具，供各模块使用
 */

#pragma once

#include "singleton.h"

#include <fstream>
#include <string>

#define Log_debug(format, ...)                                \
  sky::utility::Singleton<sky::utility::Logger>::getInstance().log( \
      sky::utility::Logger::Level::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_info(format, ...)                                 \
  sky::utility::Singleton<sky::utility::Logger>::getInstance().log( \
      sky::utility::Logger::Level::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_warn(format, ...)                                 \
  sky::utility::Singleton<sky::utility::Logger>::getInstance().log( \
      sky::utility::Logger::Level::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_error(format, ...)                                \
  sky::utility::Singleton<sky::utility::Logger>::getInstance().log( \
      sky::utility::Logger::Level::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_fatal(format, ...)                                \
  sky::utility::Singleton<sky::utility::Logger>::getInstance().log( \
      sky::utility::Logger::Level::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

namespace sky {
namespace utility {

class Logger {
  friend class sky::utility::Singleton<Logger>;  // 允许 Singleton 访问私有构造函数

 public:
  enum class Level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    LEVEL_COUNT,
  };

 private:
  std::string m_filename;                                               // 日志文件名
  std::ofstream m_fout;                                                 // 日志文件输出流
  static const char *s_level[static_cast<size_t>(Level::LEVEL_COUNT)];  // 日志级别字符串
  Level m_level;                                                        // 日志级别
  int m_len;                                                            // 当前日志长度
  int m_max;                                                            // 最大日志长度
  bool m_auto_flush;                                                    // 是否自动刷新

 public:
  /* 单例模式：全局访问点 */
  // static Logger &getInstance();

  void open(const std::string &filename);

  void close();

  /**
   * @brief 记录日志
   *
   * - @param level 日志级别
   * - @param file 文件名
   * - @param line 行号
   * - @param format 字符串的格式化
   * - @param ... 可变参数
   */
  void log(Level level, const char *file, int line, const char *format, ...);

  // setters and getters
 public:
  /* 设置日志级别 */
  void setLevel(Level level) { m_level = level; }

  /* 设置最大日志长度，单位：字节 */
  void setMax(int bytes) { m_max = bytes; }

  /* 设置自动刷新：默认是自动刷新 */
  void setAutoFlush(bool auto_flush) { m_auto_flush = auto_flush; }

 private:
  Logger();
  ~Logger();  // 避免用户手动删除单例对象（手动删除时会调用析构函数）
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  /* 日志翻滚：超出最大长度时，记载到新日志文件中 */
  void rotate();
};

}  // namespace utility

}  // namespace sky
