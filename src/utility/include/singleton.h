/**
 * @File    :   src/utility/include/singleton.h
 * @Time    :   2026/04/17 15:27:31
 * @Author  :   loskyertt
 * @Github  :   https://github.com/loskyertt
 * @Desc    :   构建通用的单例模式模板
 */

#pragma once

namespace sky {
namespace utility {

template <typename T>
class Singleton {
 public:
  static T &getInstance() {
    static T instance;
    return instance;
  }

 private:
  Singleton() = default;
  ~Singleton() = default;
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;
};

}  // namespace utility
}  // namespace sky
