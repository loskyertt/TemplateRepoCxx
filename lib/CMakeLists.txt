# for each "lib/x.cpp", generate shared library or static library
message("CMakeLists.txt for lib directory")

# 在这里添加动态库，库名称：math_utils
add_library(math_utils SHARED
  ./math_utils.cpp
)

target_include_directories(math_utils PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 可以继续往后补充......

message("\n--------------------------------------------")
