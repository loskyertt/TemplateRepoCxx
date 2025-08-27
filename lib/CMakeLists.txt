# for each "lib/x.cpp", generate shared library or static library
message("CMakeLists.txt for lib directory")

file(GLOB_RECURSE all_libs CONFIGURE_DEPENDS *.cpp)

foreach(v ${all_libs})
  message("Processing: ${v}")
  file(RELATIVE_PATH relative_path ${PROJECT_SOURCE_DIR}/lib ${v})
  string(REPLACE "/" "_" target_name ${relative_path})
  get_filename_component(target_name ${target_name} NAME_WE)
  message("Relative path: ${relative_path}")
  message("Target name: ${target_name}")

  if(NOT target_name)
    message(WARNING "Skipping invalid target name for file: ${v}")
    continue()
  endif()

  # 生成动态库
  add_library(${target_name} SHARED ${v})
  # 生成静态库
  # add_library(${target_name} STATIC ${v})

  # 设置属性
  target_include_directories(${target_name} PUBLIC
    ${PROJECT_SOURCE_DIR}/include # 公共头文件路径
    ${CMAKE_CURRENT_SOURCE_DIR} # 当前目录下的头文件
  )

  target_compile_options(${target_name} PRIVATE -Wall -Wextra -Werror)

  # 可选：设置输出目录
  # set_target_properties(${target_name} PROPERTIES
  #     LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib # 动态库输出路径
  #     ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib # 静态库输出路径
  # )
endforeach()

message("\n--------------------------------------------")
