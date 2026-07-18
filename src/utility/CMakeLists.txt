# src/utility/CMakeLists.txt

# utility::logger 模块
add_library(utility_logger STATIC
        impl/logger/logger.cpp
)
target_include_directories(utility_logger
  PUBLIC  include
  PRIVATE impl
)

# 如果是编译成动态库，需要导出 Windows 上的符号
#if(MSVC)
#    set_target_properties(utility_logger PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
#endif()

if (NOT TARGET utility::logger)
    add_library(utility::logger ALIAS utility_logger)
endif ()

# 测试
add_subdirectory(tests)
