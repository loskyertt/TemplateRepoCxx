# for each "test/x.cpp", generate target "x"
message("CMakeLists.txt for test directory")

file(GLOB_RECURSE all_tests *.cpp)

foreach(v ${all_tests})
  message("Processing: ${v}")
  # 获取相对于 test/ 的路径
  file(RELATIVE_PATH relative_path ${PROJECT_SOURCE_DIR}/test ${v})
  # 替换路径分隔符为下划线，并去掉扩展名
  string(REPLACE "/" "_" target_name ${relative_path})
  message("Relative path: ${relative_path}")
  get_filename_component(target_name ${target_name} NAME_WE)
  message("Target name: ${target_name}")

  add_executable(${target_name} ${v})

  # 为当前目标添加头文件路径
  target_include_directories(${target_name} PUBLIC ${PROJECT_SOURCE_DIR}/include)

  target_compile_options(${target_name} PRIVATE -Wall -Wextra -Werror)
endforeach()

message("\n--------------------------------------------")
