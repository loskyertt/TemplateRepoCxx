# src/CMakeLists.txt

if(MSVC)
  add_compile_options(/utf-8)  # 作用于当前 CMakeLists 下所有 target
endif()

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# core 模块：编译为静态库
add_library(core STATIC
  core/impl/circle.cpp
  core/impl/rectangle.cpp
)
target_include_directories(core
  PUBLIC  core/include
  PRIVATE core/impl
)

# utils 模块：编译为动态库
add_library(utils SHARED
  utils/impl/logger.cpp
)
target_include_directories(utils
  PUBLIC  utils/include
  PRIVATE utils/impl
)

if(MSVC)
  # 只对动态库 utils 启用 Windows 下的符号自动导出功能，core 是静态库不需要
  set_target_properties(utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# 主可执行文件
add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PRIVATE core utils)
