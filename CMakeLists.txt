cmake_minimum_required(VERSION 3.20.0)

# Set the C and C++ compilers to clang and clang++
if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER clang)
endif()
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++)
endif()

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release...")
endif()

# This is useful for tools like clangd, which use this file to provide code completion and other features.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(my_project_name VERSION 0.0.1 LANGUAGES C CXX)

# 在根 CMakeLists.txt 中定义接口库，然后在子目录中链接这个接口库
# add_library(project_options INTERFACE)
# target_compile_options(project_options INTERFACE -Wall -Wextra -Werror)
# target_include_directories(project_options INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(./src)

add_subdirectory(./test)

add_subdirectory(./example)

add_subdirectory(./lib)
