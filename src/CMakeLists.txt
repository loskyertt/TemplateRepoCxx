# for all "src/x.cpp", generate target "x"
message("CMakeLists.txt for src directory")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

add_executable(${TARGET} main.cpp)

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 链接构建好的动态库
target_link_libraries(${TARGET} PUBLIC math_utils)

message(STATUS "--------------------------------------------\n")
