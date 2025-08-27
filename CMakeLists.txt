cmake_minimum_required(VERSION 3.20.0)

# 只在第一次配置时生效；用户手动 -D CMAKE_C_COMPILER=... 仍可覆盖
if(NOT CMAKE_C_COMPILER)
  if(WIN32)
    # Windows：优先用 VS；没找到就用 MinGW
    find_program(VS_CL cl)
    if(VS_CL)
      set(CMAKE_C_COMPILER cl)
      set(CMAKE_CXX_COMPILER cl)
    else()
      find_program(MINGW_GCC gcc)
      if(MINGW_GCC)
        set(CMAKE_C_COMPILER gcc)
        set(CMAKE_CXX_COMPILER g++)
      endif()
    endif()
  else()
    # Linux / macOS / 其他
    find_program(CLANG_C clang)
    if(CLANG_C)
      set(CMAKE_C_COMPILER clang)
      set(CMAKE_CXX_COMPILER clang++)
    endif()
  endif()
endif()

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message("Setting default build type to Release...")
endif()

# This is useful for tools like clangd, which use this file to provide code completion and other features.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 统一设置所有目标的输出目录（Windows）
if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

project(my_project_name VERSION 0.0.1 LANGUAGES C CXX)

message("\n--------------------------------------------")

add_subdirectory(./lib)

add_subdirectory(./src)

add_subdirectory(./test)

add_subdirectory(./example)
