# for all "src/x.cpp", generate target "x"
set(ALL_SRCS
    main.cpp
)

# 调试：打印所有找到的源文件
message("Detected sources: ${ALL_SRCS}")

# 查找 main.cpp
list(FIND ALL_SRCS "main.cpp" MAIN_INDEX)
if(MAIN_INDEX EQUAL -1)
    message(FATAL_ERROR "Required file 'main.cpp' not found")
endif()

add_executable(my_program ${ALL_SRCS})

target_include_directories(my_program PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(my_program PRIVATE -Wall -Wextra -Werror)
