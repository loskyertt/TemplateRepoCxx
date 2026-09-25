# src/core/CMakeLists.txt

# core 模块
add_library(core STATIC
        circle.cpp
        rectangle.cpp
)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
