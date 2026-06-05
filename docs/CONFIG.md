# 项目配置与排错指南

本文档详细说明本项目的构建配置、常见环境问题及解决方案。如果你在编译、IDE 配置或工具链使用过程中遇到问题，请先查阅本文档对应章节。

---

## 目录

- [1. 项目结构](#1-项目结构)
- [2. 构建](#2-构建)
  - [2.1 使用 Ninja](#21-使用-ninja)
  - [2.2 编译](#22-编译)
  - [2.3 运行测试](#23-运行测试)
- [3. 项目配置](#3-项目配置)
  - [3.1 Windows 下编码问题](#31-windows-下编码问题)
    - [3.1.1 中文输出乱码](#311-中文输出乱码)
    - [3.1.2 MSVC 编译时的 UTF-8 支持](#312-msvc-编译时的-utf-8-支持)
  - [3.2 clangd 配置](#32-clangd-配置)
    - [3.2.1 方案 1：使用集成 LLVM 的 MinGW](#321-方案-1使用集成-llvm-的-mingw)
    - [3.2.2 方案 2：配置 clangd 参数](#322-方案-2配置-clangd-参数)
    - [3.2.3 方案 3：改用 Clang 编译器](#323-方案-3改用-clang-编译器)
  - [3.3 使用 MSVC 编译器](#33-使用-msvc-编译器)
- [4. GTest 配置](#4-gtest-配置)
  - [4.1 方式一：联网下载（FetchContent）](#41-方式一联网下载fetchcontent)
  - [4.2 方式二：本地源码（3rdparty）](#42-方式二本地源码3rdparty)
  - [4.3 方式三：系统包管理器](#43-方式三系统包管理器)
- [5. 常见问题速查](#5-常见问题速查)

---

## 1. 项目结构

```
project_root/
├── CMakeLists.txt          # 顶层构建配置（现代 CMake 范式）
├── src/                    # 源代码目录
│   ├── main.cpp            # 程序入口
│   ├── core/               # 核心业务模块
│   │   ├── include/        # 对外公开接口头文件
│   │   └── impl/           # 内部实现源文件和内部私有头文件
│   └── utils/              # 通用工具模块
│       ├── include/        # 对外公开接口头文件
│       └── impl/           # 内部实现源文件
├── tests/                  # 测试代码
│   ├── unit/               # 单元测试
│   └── CMakeLists.txt      # 测试构建配置
├── cmake/                  # 自定义 CMake 模块
├── docs/                   # 技术文档与架构说明
├── .clang-format           # 代码格式化规则
├── .clang-tidy             # 静态检查规则
├── .clangd                 # clangd 语言服务器配置
└── .vscode/                # VS Code 工作区配置
    └── settings.json
```

---

## 2. 构建

### 2.1 使用 Ninja

> Ninja 的构建速度比 Make 更快，推荐作为首选生成器。

**LLVM / Clang：**
```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

**GCC：**
```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
```

**MSVC：**
```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
```

### 2.2 编译

```bash
cmake --build build -j12
```

> `-j12` 表示并行编译线程数为 12，可根据 CPU 核心数调整。

### 2.3 运行测试

```bash
cd build && ctest --output-on-failure
```

或运行测试可执行文件：
```bash
./build/tests/unit_tests
./build/tests/unit_tests --gtest_filter=CircleTest.*
```

---

## 3. 项目配置

### 3.1 Windows 下编码问题

#### 3.1.1 中文输出乱码

Windows 控制台默认使用 GB2312 编码，而项目源码采用 UTF-8。当程序输出中文时会出现乱码。

**方式一：临时修改 cmd 编码**
```shell
chcp 65001
```

**方式二：修改 PowerShell 配置文件（永久生效）**
```shell
notepad $PROFILE
```

在配置文件中写入：
```powershell
# 设置默认编码为 UTF-8
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
[Console]::InputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

# 设置环境变量，让子进程也使用 UTF-8
$env:PYTHONIOENCODING = "utf-8"
```

修改完毕后，重启 PowerShell 即可。

**方式三：在代码中设置（适用于 VS 内运行）**

在 `main` 函数开头添加：
```cpp
#include <windows.h>

int main() {
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  // 其它代码...
}
```

#### 3.1.2 MSVC 编译时的 UTF-8 支持

如果采用 MSVC 编译，需要在 `CMakeLists.txt` 中启用 UTF-8 支持：

```cmake
if(MSVC)
  add_compile_options(/utf-8)
endif()
```

对于动态库，需要额外配置符号导出：
```cmake
if(MSVC)
  set_target_properties(utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
```

> `WINDOWS_EXPORT_ALL_SYMBOLS` 是专门为动态库（`SHARED`）设计的——它让 MSVC 自动生成 `.lib` 导入库和导出符号。静态库直接被链接进可执行文件，没有"导出符号"这个概念，所以不需要配置。

---

### 3.2 clangd 配置

在 **VS Code + clangd** 配置下，可能会出现无法找到标准库头文件的情况，导致 clangd 报错，但编译可以通过。这个问题通常出现在**单独安装 LLVM 和 MinGW** 的场景下：clangd 基于 LLVM/Clang，默认在 MSVC 或自身 libc++ 环境下查找头文件，而 MinGW 使用 GCC 的 libstdc++，头文件路径不同。

#### 3.2.1 方案 1：使用集成 LLVM 的 MinGW

下载 [winlibs](https://winlibs.com/)，选择 **UCRT runtime** 版并且包含有 **LLVM/Clang/LLD/LLDB** 的下载。

#### 3.2.2 方案 2：配置 clangd 参数

在 VS Code 的 `settings.json` 中添加：

```json
{
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build",
        "--query-driver=C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe",
        "--background-index",
        "--header-insertion=iwyu",
        "--completion-style=bundled",
        "--pch-storage=memory",
        "--cross-file-rename",
        "--enable-config"
    ]
}
```

> `--query-driver` 告诉 clangd 允许使用哪些编译器驱动程序来分析代码。通常也可用通配符设置：`"--query-driver=C:/Apps/CLion 2025.3.2/bin/mingw/bin/**.exe"`

再在项目根目录下创建 `.clangd` 文件，手动指定 MinGW 的头文件搜索路径：

```yaml
CompileFlags:
  Add:
    - -std=c++23
    - "-IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++"
    - "-IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++/x86_64-w64-mingw32"
    - "-IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include"
    - "-IC:/Apps/CLion 2025.3.2/bin/mingw/include"
    - "-IC:/Apps/CLion 2025.3.2/bin/mingw/x86_64-w64-mingw32/include"
  Compiler: C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe
```

> 记得把上面的路径改成你自己的实际路径。

如果导入第三方库时遇到 `compile_commands.json` 失效的情况（比如导入头文件报错），可以手动添加第三方库的头文件检索路径：

```yaml
CompileFlags:
  CompilationDatabase: build/
  Add:
    - "-IC:/path/to/your/third_party/include"
```

#### 3.2.3 方案 3：改用 Clang 编译器

直接用 Clang 编译，避免工具链混用：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

确保 `clang++` 能找到 MinGW 的 libstdc++，或使用 Clang 自带的 libc++。

---

### 3.3 使用 MSVC 编译器

有时候不可避免地要用到 MSVC 编译器，因为网上有不少预编译好的 Windows 第三方库都是 MSVC 编译的。

安装 Visual Studio 时，会自动安装 **Developer Command Prompt for VS 2022** 和 **Developer PowerShell for VS 2022**，这两个终端集成了 MSVC 编译器的环境变量。在该终端中使用指令 `where.exe cl` 会有如下输出：

```shell
PS C:\Apps\Microsoft Visual Studio\2022\Community> where.exe cl
C:\Apps\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe
```

打开 **Developer PowerShell for VS 2022**，然后进入项目根目录：

```shell
code .        # 打开 VS Code
clion64.exe . # 或打开 CLion
```

> 前提是把 VS Code 和 CLion 添加到环境变量中。

在编译时指定编译器：
```shell
cmake -B build -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
```

如果使用 MSVC 生成器（Visual Studio Generator），会生成 VS 解决方案，**但不会生成 `compile_commands.json` 文件**：

```shell
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug
```

因此，除非要使用 VS 进行开发，否则生成器请选择 **Ninja** 或 **Unix Makefiles**。

---

## 4. GTest 配置

### 4.1 方式一：联网下载（FetchContent）

在顶层 `CMakeLists.txt` 中添加：

```cmake
enable_testing()
```

> `gtest_discover_tests` 注册测试用例依赖 `enable_testing()` 这个开关，没有它 CTest 完全不知道有测试存在。该选项必须在顶层、`add_subdirectory` 之前。

再在 `tests/CMakeLists.txt` 中添加：

```cmake
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Windows 上保持 MSVC 运行时一致性
if(MSVC)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
  unit/test_circle.cpp
  unit/test_rectangle.cpp
)

target_link_libraries(unit_tests
  PRIVATE core GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
```

**关键说明：**

- **`set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)`**：仅在 Windows + MSVC 下有意义。MSVC 编译时有两种运行时库：`/MT`（静态）和 `/MD`（动态）。如果项目用 `/MD`，但 GTest 默认编译成 `/MT`，链接时会报冲突。这个变量强制 GTest 跟项目保持一致。

- **`include(GoogleTest)`**：加载 CMake 内置的 `GoogleTest` 模块，提供 `gtest_discover_tests` 函数。

- **`gtest_discover_tests(unit_tests)`**：让 CMake 在构建完成后自动运行 `unit_tests` 可执行文件，扫描里面所有 `TEST()`、`TEST_F()` 的用例，并逐一注册到 CTest 中。这样 `ctest` 才能发现并运行它们。

- **缓存机制**：第一次 configure 时，CMake 会把 GTest 下载并解压到 `build/_deps/googletest-src/`。后续只要 `build/` 目录存在，CMake 检测到 `_deps` 已存在就直接跳过下载。

### 4.2 方式二：本地源码（3rdparty）

手动从 [google/googletest](https://github.com/google/googletest) 下载源码，解压缩后放到 `3rdparty/` 目录下。

在顶层 `CMakeLists.txt` 中添加：

```cmake
enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/googletest-1.17.0)
```

> 把 `googletest-1.17.0` 改成你自己的源码目录名。

在 `tests/CMakeLists.txt` 中：

```cmake
if(MSVC)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_executable(unit_tests
  unit/test_circle.cpp
  unit/test_rectangle.cpp
)

target_link_libraries(unit_tests
  PRIVATE core GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
```

### 4.3 方式三：系统包管理器

如果使用的是 vcpkg 管理的 GTest，或者在 Linux 下通过包管理器安装，则不需要 `add_subdirectory`，直接 `find_package` 即可：

```cmake
enable_testing()
find_package(GTest REQUIRED)
```

---

## 5. 常见问题速查

| 问题 | 可能原因 | 解决方案 |
|------|---------|---------|
| 编译报错找不到标准库头文件 | clangd 与 MinGW 工具链混用 | 使用方案 1（winlibs）或方案 3（纯 Clang），或配置 `--query-driver` |
| 中文输出乱码 | 控制台编码与源码编码不一致 | `chcp 65001` 或在代码中 `SetConsoleOutputCP(CP_UTF8)` |
| MSVC 链接报错运行时库冲突 | GTest 与项目使用不同的 CRT | 设置 `gtest_force_shared_crt ON` |
| `ctest` 找不到测试 | 未启用 `enable_testing()` 或未调用 `gtest_discover_tests` | 检查顶层 `CMakeLists.txt` 和 `tests/CMakeLists.txt` |
| 没有生成 `compile_commands.json` | 使用了 Visual Studio Generator | 改用 Ninja 或 Unix Makefiles 生成器 |
| 动态库符号未导出 | MSVC 下缺少导出配置 | 设置 `WINDOWS_EXPORT_ALL_SYMBOLS ON` |
