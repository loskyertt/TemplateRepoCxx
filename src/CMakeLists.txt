# src/CMakeLists.txt
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
  set_target_properties(utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# 主可执行文件
add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PRIVATE core utils)
