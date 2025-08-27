# for all "src/x.cpp", generate target "x"
message("CMakeLists.txt for src directory")

set(ALL_SRCS
  main.cpp
)

# 调试：打印所有找到的源文件
message("Detected sources: ${ALL_SRCS}")

add_executable(my_program ${ALL_SRCS})

target_include_directories(my_program PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_options(my_program PRIVATE -Wall -Wextra -Werror)

message("\n--------------------------------------------")
