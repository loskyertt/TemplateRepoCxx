cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message("Setting default build type to Release")
endif()

# clangd 利用此文件提供代码补全等功能
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 统一设置所有目标的输出目录（Windows）
if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)
endif()

project(my_project_name VERSION 0.0.1 LANGUAGES C CXX)

message("\n--------------------------------------------")

add_subdirectory(./lib)

add_subdirectory(./src)

# add_subdirectory(./test)

add_subdirectory(./example)
