# src/utility/CMakeLists.txt

# utility::logger 模块
add_library(utility STATIC
        logger/logger.cpp
)
target_include_directories(utility
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC common
)

# 如果是编译成动态库，需要导出 Windows 上的符号
#if(MSVC)
#    set_target_properties(utility PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
#endif()

# 测试
add_subdirectory(tests)
