cmake_minimum_required(VERSION 3.20.0)

# 设置标准（建议在 project 之前）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# 导出编译命令（用于 clangd/vscode）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(geometry_app VERSION 0.0.1 LANGUAGES C CXX)

# 开启 gtest_discover_tests 注册测试用例依赖
enable_testing()

# 脚本
# include(cmake/checkArch.cmake)

# 子模块
add_subdirectory(src)
add_subdirectory(tests)
