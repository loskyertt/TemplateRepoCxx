# tests/CMakeLists.txt

add_executable(unit_tests
        unit/test_circle.cpp
        unit/test_rectangle.cpp
)

if (NOT GTest_FOUND)
    message(STATUS "GTest not found, downloading googletest of v1.14.0 from github ...")

    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif ()

target_link_libraries(unit_tests
        PRIVATE
        core
        GTest::gtest_main
)

# 加载 CMake 内置的 GoogleTest 模块
include(GoogleTest)

gtest_discover_tests(unit_tests)
