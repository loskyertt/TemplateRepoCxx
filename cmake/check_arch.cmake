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

# C. 检查生成器平台 (仅 VS 有效)
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
