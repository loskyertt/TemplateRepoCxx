# for each "example/x.cpp", generate target "x"
message("CMakeLists.txt for example directory")

file(GLOB_RECURSE all_examples *.cpp)
foreach(v ${all_examples})
  string(REGEX MATCH "example/.*" relative_path ${v})
  message(${relative_path})
  string(REGEX REPLACE "example/" "" target_name ${relative_path})
  string(REGEX REPLACE ".cpp" "" target_name ${target_name})

  add_executable(${target_name} ${v})

  # MSVC UTF-8 支持
  if(MSVC)
    target_compile_options(${target_name} PRIVATE /utf-8)
  endif()
endforeach()

message("\n--------------------------------------------")
