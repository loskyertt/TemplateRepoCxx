# for each "example/x.cpp", generate target "x"
message("CMakeLists.txt for example directory")

file(GLOB_RECURSE ALL_EXAMPLES *.cpp)
foreach(v ${ALL_EXAMPLES})
  message("Processing: ${v}")
  get_filename_component(TARGET_NAME ${v} NAME_WE) # 生成目标名（去掉路径和扩展名）

  add_executable(${TARGET_NAME} ${v})

  # MSVC UTF-8 支持
  if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /utf-8)
  endif()
endforeach()

message(STATUS "--------------------------------------------\n")
