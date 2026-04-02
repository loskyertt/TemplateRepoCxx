# for each "test/x.cpp", generate target "x"
message("CMakeLists.txt for test directory")

file(GLOB_RECURSE ALL_TESTS *.cpp)

foreach(v ${ALL_TESTS})
  message("Processing: ${v}")
  get_filename_component(TARGET_NAME ${v} NAME_WE) # 生成目标名（去掉路径和扩展名）

  add_executable(${TARGET_NAME} ${v})

  # 为当前目标添加头文件路径
  target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

  # target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Werror)

  # MSVC UTF-8 支持
  if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /utf-8)
  endif()
endforeach()

message(STATUS "--------------------------------------------\n")
