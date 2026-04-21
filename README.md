# 1. 说明

该项目仅适合单个 C++ 文件编译。

---

# 2. 构建

## 2.1 使用 Ninja

> `Ninja`的构建速度很快。

如果采用的是 LLVM：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

如果采用的是 GCC：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
```

如果采用的是 MSVC：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
```

## 2.2 编译

```bash
cmake --build build -j12
```

> `-j12` 表示指定的线程数是 12。

---

# 3. 项目配置

> 下面是可能在编写代码时遇到的问题，以及对应的解决办法。

## 3.1 Windows 下编码问题

### 3.1.1 中文输出乱码问题

> Windows 的控制台编码一般采用的是 GB2312 编码的，但源代码编码采用的是 UTF-8 编码（不建议把源代码文件编码改为 GB2312 格式！），所以当有中文在控制台输出时，会产生乱码。解决方法如下：

- 方式一：如果使用的是 **cmd**，在每次从终端执行可编译好的程序之前，执行：

```shell
chcp 65001
```

执行完后，再执行程序就不会乱码。

- 方式二：如果使用的是 **PowerShell**，可以修改配置文件：

打开配置文件（如果没有的话，会创建）：

```shell
notepad $PROFILE
```

在配置文件中写入：

```shell
# 设置默认编码为 UTF-8
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
[Console]::InputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

# 设置环境变量，让子进程也使用 UTF-8
$env:PYTHONIOENCODING = "utf-8"
```

修改完毕后，重启 PowerShell 即可。

如果是在 VS 中运行程序，因为默认打开的是 cmd，所以建议在代码 `main` 函数开头添加：

```cpp
int main(){
  // 设置控制台为 UTF-8
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  // 其它代码......
}
```

### 3.1.2 MSVC 编译时的问题

如果采用 MSVC 编译，需要在 `CMakeLists.txt` 中给目标程序（包括静态库和动态库）添加编译选项以启用 MSVC 下的 UTF-8 支持：

```cmake
if(MSVC)
  target_compile_options(my_app PRIVATE /utf-8)
endif()
```

如果有多个 `target` 都要加 `/utf-8`，可以提到顶层或用 `add_compile_options` 避免重复：

```cmake
if(MSVC)
  add_compile_options(/utf-8)
endif()
```

> 这会作用于当前 CMakeLists 下所有 target，包括 `add_subdirectory` 下的子目录。

对于动态库的链接：

```cmake
if(MSVC)
  set_target_properties(utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
```

> `WINDOWS_EXPORT_ALL_SYMBOLS` 是专门为动态库（`SHARED`）设计的——它让 MSVC 自动生成 `.lib` 导入库和导出符号。静态库直接被链接进可执行文件，没有"导出符号"这个概念，所以不需要配置。

## 3.2 clangd 配置

在 **VSCode + clangd** 配置下，可能会出现无法找到标准库头文件的情况，导致 clangd 报错，但是编译是可以通过的。一般单独分别安装 llvm 和 mingw 时，会出现这个错误。这个错误是因为 **clangd 无法找到 MinGW 的标准库头文件路径**。虽然你生成了 `compile_commands.json`，但里面的编译器是 MinGW 的 `g++.exe`，而 clangd 需要知道这些头文件在哪里。

其问题根源是 clangd 和 MinGW 是两个不同的工具链：
- **clangd** 基于 LLVM/Clang，默认在 MSVC 或自身 libc++ 环境下查找头文件
- **MinGW** 使用 GCC 的 libstdc++，头文件路径不同

### 3.2.1 方案 1：使用集成有 llvm 的 mingw

下载：[winlibs](https://winlibs.com/)

选择 **UCRT runtime** 版并且包含有 **LLVM/Clang/LLD/LLDB** 的下载。

### 3.2.2 方案 2：配置 clangd 参数

> 比如我这里使用的是 CLion 集成的 mingw。

在 VS Code 的 `settings.json` 中添加：

```json
{
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build", // 指定 compile_commands.json 所在目录
        "--query-driver=C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe", // 查询指定编译器的系统 include 路径
        "--background-index", // 后台索引整个项目，加速代码跳转
        "--header-insertion=iwyu", // 自动插入头文件时遵循 "Include What You Use" 原则
        "--completion-style=bundled", // 代码补全风格：打包式
        "--pch-storage=memory", // 预编译头文件存储在内存中
        "--cross-file-rename", // 启用跨文件重命名功能
        "--enable-config", // 允许从 .clangd 文件读取配置
    ],
}
```

`--query-driver` 作用是告诉 clangd 允许使用哪些编译器驱动程序来分析代码。它主要用于：
- Linux 系统上的权限问题：clangd 默认可能无权限访问系统编译器
- 跨平台编译时：指定允许使用的目标编译器
- 通常也用通配符设置：`"--query-driver=C:/Apps/CLion 2025.3.2/bin/mingw/bin/**.exe"`

再在项目根目录下创建 `.clangd` 文件，在里面手动指定 MinGW 的头文件搜索路径：

```yaml
CompileFlags:
  Add:
    - -std=c++20
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++/x86_64-w64-mingw32
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/include
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/x86_64-w64-mingw32/include
  Compiler: C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe
```

这个 `Compiler` 选项是给 clangd 语言服务器用的，让它知道用哪个编译器来解析代码、提供准确的代码补全、进行静态分析、报告错误和警告。

> 记得把上面的路径改成你自己的。

如果有时候在导入第三方库遇到 `compile_commands.json` 失效的情况（比如导入头文件报错），可以手动添加第三方库的头文件检索路径，比如这样添加 SDL3 库：

```yaml
CompileFlags:
  # 指定 compile_commands.json 的位置（如果不在根目录）
  CompilationDatabase: build/

  Add:
    # 包含路径（-I）示例，按需修改为实际路径（建议用绝对路径）
    - "-IC:/Users/sky/Desktop/Repositories/Games/GhostEscape/3rdparty/glm-1.0.3"
    - "-IC:/Users/sky/Desktop/Repositories/Games/GhostEscape/3rdparty/SDL3-3.4.2/include"
    - "-IC:/Users/sky/Desktop/Repositories/Games/GhostEscape/3rdparty/SDL3_image-3.4.0/include"
    - "-IC:/Users/sky/Desktop/Repositories/Games/GhostEscape/3rdparty/SDL3_mixer-3.2.0/include"
    - "-IC:/Users/sky/Desktop/Repositories/Games/GhostEscape/3rdparty/SDL3_ttf-3.2.2/include"
    - "-IC:/Users/sky/Desktop/Repositories/Games/GhostEscape/include"
```

### 3.2.3 方案 3：改用 Clang 编译器

可以尝试直接用 Clang 编译，避免工具链混用：

```bash
# 在 CMake 配置时指定 Clang
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

确保 `clang++` 能找到 MinGW 的 libstdc++，或使用 Clang 自带的 libc++。

## 3.3 使用 MSVC 编译器

> 有时候不可避免得要用到 MSVC 编译器，因为网上有不少预编译好的 Windows 第三方库，都是用的 MSVC 编译器。

下载后 VS 时（比如下载的是 VS 2022 Community），会自动安装 **Developer Command Prompt for VS 2022** 和 **Developer PowerShell for VS 2022**，这两个终端集成了 MSVC 编译器的环境变量。在该终端中使用指令 `where.exe cl` 会有如下输出：

```shell
PS C:\Apps\Microsoft Visual Studio\2022\Community> where.exe cl
C:\Apps\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe
```

打开 **Developer PowerShell for VS 2022**，然后用 `cd` 命令进入到项目根目录，输入：

```shell
# 打开 VSCode
code .

# 或者打开 CLion
clion64.exe .
```

这样打开的 IDE 就会继承  **Developer PowerShell for VS 2022** 中的环境。

> 前提是要把 VSCode 和 CLion 添加到环境变量中。

在编译时指定编译器：`-DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl`。

如果使用 MSVC 生成器（Visual Studio Generator），这会生成 VS 解决方案，但是不会生成 `compile_commands.json` 文件：

```shell
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug
```

因此，除非要使用 VS 进行开发，其他情况生成器请选择 Ninja 或者 Unix Makefiles。
