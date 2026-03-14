# for each "test/x.cpp", generate target "x"
message("CMakeLists.txt for test directory")

file(GLOB_RECURSE all_tests *.cpp)

foreach(v ${all_tests})
  message("Processing: ${v}")
  get_filename_component(target_name ${v} NAME_WE) # 生成目标名（去掉路径和扩展名）

  add_executable(${target_name} ${v})

  # 为当前目标添加头文件路径
  target_include_directories(${target_name} PUBLIC ${PROJECT_SOURCE_DIR}/include)

  # target_compile_options(${target_name} PRIVATE -Wall -Wextra -Werror)

  # MSVC UTF-8 支持
  if(MSVC)
    target_compile_options(${target_name} PRIVATE /utf-8)
  endif()
endforeach()

message(STATUS "--------------------------------------------\n")
