cmake_minimum_required(VERSION 3.20.0)

# 只在第一次配置时生效，用户手动 -DCMAKE_C_COMPILER=... 和 -DCMAKE_CXX_COMPILER=... 仍可覆盖
if(NOT CMAKE_C_COMPILER AND NOT CMAKE_CXX_COMPILER)
  if(WIN32)
    # Windows：优先用 VS；没找到就用 MinGW
    find_program(VS_CL cl)
    if(VS_CL)
      set(CMAKE_C_COMPILER cl)
      set(CMAKE_CXX_COMPILER cl)
      message(STATUS "Using Visual Studio compiler: ${VS_CL}")
    else()
      find_program(MINGW_GCC gcc)
      if(MINGW_GCC)
        set(CMAKE_C_COMPILER gcc)
        set(CMAKE_CXX_COMPILER g++)
        message(STATUS "Using MinGW compiler: ${MINGW_GCC}")
      endif()
    endif()
  else()
    # Linux / macOS / 其他：优先使用 clang
    find_program(CLANG_C clang)
    if(CLANG_C)
      set(CMAKE_C_COMPILER clang)
      set(CMAKE_CXX_COMPILER clang++)
      message(STATUS "Using Clang compiler: ${CLANG_C}")
    endif()
  endif()
endif()

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
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
endif()

project(my_project_name VERSION 0.0.1 LANGUAGES C CXX)

message("\n--------------------------------------------")

# add_subdirectory(./lib)

# add_subdirectory(./src)

# add_subdirectory(./test)

add_subdirectory(./example)
