# src/core/CMakeLists.txt

# core 模块
add_library(core STATIC
        impl/circle.cpp
        impl/rectangle.cpp
)
target_include_directories(core
        PUBLIC  include
        PRIVATE impl
)
