# src/utility/tests/CMakeLists.txt
add_executable(test_logger test_logger.cpp)
target_link_libraries(test_logger PRIVATE utility::logger)
