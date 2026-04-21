# tests/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Windows 上保持 MSVC 运行时一致性
if(MSVC)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
  unit/test_circle.cpp
  unit/test_rectangle.cpp
)

target_link_libraries(unit_tests
  PRIVATE core GTest::gtest_main
)

include(GoogleTest)  # 加载 CMake 内置的 GoogleTest 模块
gtest_discover_tests(unit_tests)
