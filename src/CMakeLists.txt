# src/CMakeLists.txt
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

add_subdirectory(core)

add_subdirectory(utility)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PRIVATE core utility::logger)
