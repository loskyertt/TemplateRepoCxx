# 项目配置与排错指南

本文档详细说明本项目的构建配置、常见环境问题及解决方案。如果你在编译、IDE 配置或工具链使用过程中遇到问题，请先查阅本文档对应章节。

---

## 目录

- [1. 构建](#1-构建)
  - [1.1 配置](#11-配置)
  - [1.2 编译](#12-编译)
  - [1.3 GLOB 的注意事项](#13-glob-的注意事项)
- [2. Windows 下编码问题](#2-windows-下编码问题)
  - [2.1 中文输出乱码](#21-中文输出乱码)
  - [2.2 MSVC 编译时的 UTF-8 支持](#22-msvc-编译时的-utf-8-支持)
- [3. clangd 配置](#3-clangd-配置)
  - [3.1 问题根源](#31-问题根源)
  - [3.2 方案 1：使用集成 LLVM 的 MinGW](#32-方案-1使用集成-llvm-的-mingw)
  - [3.3 方案 2：配置 clangd 参数](#33-方案-2配置-clangd-参数)
  - [3.4 方案 3：改用 Clang 编译器](#34-方案-3改用-clang-编译器)
- [4. 使用 MSVC 编译器](#4-使用-msvc-编译器)
- [5. 常见问题速查](#5-常见问题速查)

---

## 1. 构建

### 1.1 配置

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

> 在 Windows 上使用 MSVC 时，建议通过 **Developer PowerShell for VS 2022** 执行上述命令，以确保环境变量正确。

### 1.2 编译

```bash
cmake --build build -j12
```

> `-j12` 表示并行编译线程数为 12，可根据 CPU 核心数调整。

### 1.3 GLOB 的注意事项

本项目使用 `file(GLOB_RECURSE *.cpp)` 自动发现源文件，这带来便利的同时也有一个限制：**CMake 不会在构建时自动检测新添加的 `.cpp` 文件**。

如果你添加了新文件但编译时未生成对应目标，请执行以下任一操作：

```bash
# 方式一：重新配置
cmake -B build -G Ninja ...

# 方式二：删除 build 目录后重新配置
rm -rf build && cmake -B build -G Ninja ...
```

---

## 2. Windows 下编码问题

### 2.1 中文输出乱码

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

### 2.2 MSVC 编译时的 UTF-8 支持

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

## 3. clangd 配置

### 3.1 问题根源

在 **VS Code + clangd** 配置下，可能会出现无法找到标准库头文件的情况，导致 clangd 报错，但编译可以通过。这个问题通常出现在**单独安装 LLVM 和 MinGW** 的场景下：clangd 基于 LLVM/Clang，默认在 MSVC 或自身 libc++ 环境下查找头文件，而 MinGW 使用 GCC 的 libstdc++，头文件路径不同。

### 3.2 方案 1：使用集成 LLVM 的 MinGW

下载 [winlibs](https://winlibs.com/)，选择 **UCRT runtime** 版并且包含有 **LLVM/Clang/LLD/LLDB** 的下载。

### 3.3 方案 2：配置 clangd 参数

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

### 3.4 方案 3：改用 Clang 编译器

直接用 Clang 编译，避免工具链混用：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

确保 `clang++` 能找到 MinGW 的 libstdc++，或使用 Clang 自带的 libc++。

---

## 4. 使用 MSVC 编译器

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

## 5. 常见问题速查

| 问题 | 可能原因 | 解决方案 |
|------|---------|---------|
| 添加了新 `.cpp` 但编译时未生成目标 | `GLOB` 不会自动检测新文件 | 重新运行 `cmake -B build` 或删除 `build/` 后重新配置 |
| 编译报错找不到标准库头文件 | clangd 与 MinGW 工具链混用 | 使用方案 1（winlibs）或方案 3（纯 Clang），或配置 `--query-driver` |
| 中文输出乱码 | 控制台编码与源码编码不一致 | `chcp 65001` 或在代码中 `SetConsoleOutputCP(CP_UTF8)` |
| MSVC 链接报错运行时库冲突 | 第三方库与项目使用不同的 CRT | 确保所有依赖使用相同的 `/MT` 或 `/MD` 选项 |
| 没有生成 `compile_commands.json` | 使用了 Visual Studio Generator | 改用 Ninja 或 Unix Makefiles 生成器 |
| 动态库符号未导出 | MSVC 下缺少导出配置 | 设置 `WINDOWS_EXPORT_ALL_SYMBOLS ON` |
