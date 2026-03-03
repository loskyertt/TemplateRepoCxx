# for each "example/x.cpp", generate target "x"
message("CMakeLists.txt for example directory")

file(GLOB_RECURSE all_examples *.cpp)
foreach(v ${all_examples})
  message("Processing: ${v}")
  get_filename_component(target_name ${v} NAME_WE) # 生成目标名（去掉路径和扩展名）

  add_executable(${target_name} ${v})

  # MSVC UTF-8 支持
  if(MSVC)
    target_compile_options(${target_name} PRIVATE /utf-8)
  endif()
endforeach()

message("\n--------------------------------------------")
