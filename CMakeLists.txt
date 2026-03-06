cmake_minimum_required(VERSION 3.20.0)

# 1. 设置标准 (建议在 project 之前)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# 2. 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
  message(STATUS "Setting default build type to Release")
endif()

# 3. 导出编译命令 (用于 clangd/vscode)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 4. Windows 特定设置
if(WIN32)
  # set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)  # 启用 Windows 下的符号自动导出功能（全局开启，不推荐）
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)
endif()

# 5. 定义项目 (这会触发编译器探测，从而设置 CMAKE_SIZEOF_VOID_P)
project(YOUR_PROJECT_NAME VERSION 0.0.1 LANGUAGES C CXX)

# ---------------------------------------------------------
# 6. 架构检测逻辑 (放在 project 之后)
# ---------------------------------------------------------

message(STATUS "--------------------------------------------")
message(STATUS "编译架构检测信息：")

# A. 检查指针大小 (最准确的 32/64 位判断)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS ">> 目标位数：64 位 (x64)")
  set(TARGET_ARCH_BIT "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(STATUS ">> 目标位数：32 位 (x86)")
  set(TARGET_ARCH_BIT "32")
else()
  message(STATUS ">> 目标位数：未知 (指针大小：${CMAKE_SIZEOF_VOID_P})")
  set(TARGET_ARCH_BIT "Unknown")
endif()

# B. 检查处理器架构
message(STATUS ">> 目标处理器 (System Processor): ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS ">> 主机处理器 (Host Processor)  : ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# C. 检查生成器平台 (仅 VS 有效，其他为空，避免困惑)
if(CMAKE_GENERATOR_PLATFORM)
  message(STATUS ">> 生成器平台 (Generator Platform): ${CMAKE_GENERATOR_PLATFORM}")
else()
  message(STATUS ">> 生成器平台 (Generator Platform): (当前生成器不支持此变量，如 Ninja/MinGW)")
endif()

# D. MSVC 特有检查
if(MSVC)
  if(CMAKE_CL_64)
    message(STATUS ">> MSVC 模式：64 位编译器")
  else()
    message(STATUS ">> MSVC 模式：32 位编译器")
  endif()
endif()

message(STATUS "--------------------------------------------\n")

add_subdirectory(./lib)

add_subdirectory(./src)

# add_subdirectory(./test)

# add_subdirectory(./example)
