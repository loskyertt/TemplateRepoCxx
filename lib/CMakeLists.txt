# for each "lib/x.cpp", generate shared library or static library
message("CMakeLists.txt for lib directory")

# 在这里添加动态库，库名称：math_utils
add_library(math_utils SHARED
  ./math_utils.cpp
)

target_include_directories(math_utils PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(MSVC)
  set_target_properties(math_utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  target_compile_options(math_utils PRIVATE /utf-8)  # UTF-8 支持
endif()

message("\n--------------------------------------------")
