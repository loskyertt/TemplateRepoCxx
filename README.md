# TemplateRepoCxx

一个轻量级的 C++ 项目模板，专为**单文件快速验证、算法练习、小型原型开发**而设计。

本项目采用极简的 CMake 结构：每个 `.cpp` 源文件会被自动发现并编译为独立可执行目标，无需为每个新文件手动修改 `CMakeLists.txt`。同时预置了现代化的 C++ 开发工具链配置（clangd、clang-format、clang-tidy），让你可以专注于代码本身。

## 特性

- **零配置添加目标**：将 `.cpp` 文件放入 `examples/` 或 `tests/`，CMake 自动为其生成可执行文件
- **现代 C++ 标准**：默认启用 C++23，支持 `std::print` / `std::println` 等最新特性
- **开箱即用的 IDE 支持**：内置 clangd / VS Code / CLion 配置，支持代码补全、跳转、静态分析
- **跨平台支持**：Windows (MSVC / MinGW)、Linux (GCC / Clang)、macOS (Clang)

## 技术栈

| 组件 | 版本 / 说明 |
|------|------------|
| C++ 标准 | C++23 |
| C 标准 | C11 |
| 构建系统 | CMake 3.20+ |
| 生成器 | Ninja（推荐）、Unix Makefiles、Visual Studio |
| 代码格式化 | clang-format（Google 风格扩展） |
| 静态检查 | clang-tidy |
| 语言服务器 | clangd |

## 快速开始

### 前置要求

- [CMake](https://cmake.org/download/) 3.20 或更高版本
- [Ninja](https://ninja-build.org/)（推荐，构建速度更快）
- C++23 兼容的编译器：
  - **Windows**：MSVC 2022+ / MinGW-w64 GCC 13+ / Clang 17+
  - **Linux**：GCC 13+ / Clang 17+
  - **macOS**：Clang 15+（Xcode Command Line Tools）

### 构建与运行

```bash
# 1. 配置（以 Ninja + GCC 为例）
cmake -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

# 2. 编译
cmake --build build -j$(nproc)

# 3. 运行示例
./build/examples/example01

# 4. 运行测试
./build/tests/test01
./build/tests/test02
```

> 使用 MSVC 时建议通过 **Developer PowerShell for VS 2022** 执行，并在配置时指定 `-DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl`。详见 [`docs/CONFIG.md`](docs/CONFIG.md)。

### 添加自己的代码

1. 在 `examples/` 目录下新建 `.cpp` 文件：
   ```cpp
   #include <print>

   int main() {
     std::println("Hello, World!");
     return 0;
   }
   ```

2. 重新运行 `cmake --build build`

3. 直接运行生成的可执行文件：`./build/examples/your_file`

> 无需修改任何 `CMakeLists.txt`——`examples/` 和 `tests/` 目录下的所有 `.cpp` 文件会被 `file(GLOB_RECURSE)` 自动发现并编译为独立目标。

## 项目结构

```
TemplateRepoCxx/
├── CMakeLists.txt              # 顶层构建配置
├── examples/                   # 示例代码目录
│   ├── CMakeLists.txt          # 自动发现 .cpp 并生成可执行文件
│   └── example01.cpp           # 示例：使用 std::println 输出
├── tests/                      # 测试代码目录
│   ├── CMakeLists.txt          # 自动发现 .cpp 并生成可执行文件
│   ├── test01.cpp              # 测试示例 1
│   └── test02.cpp              # 测试示例 2
├── cmake/
│   └── check_arch.cmake        # 架构检测脚本（构建时输出目标位数、处理器类型等）
├── docs/
│   └── CONFIG.md               # 详细配置与排错指南
├── .vscode/
│   └── settings.json           # VS Code 配置（clangd 参数等）
├── .clang-format               # 代码格式化规则（Google 风格扩展）
├── .clang-tidy                 # 静态检查规则
├── .clangd                     # clangd 语言服务器配置
├── .gitignore
└── LICENSE                     # Apache-2.0
```

## 工作原理

### 自动目标发现

`examples/CMakeLists.txt` 和 `tests/CMakeLists.txt` 使用相同的模式：

```cmake
file(GLOB_RECURSE ALL_SRCS *.cpp)

foreach(v ${ALL_SRCS})
  get_filename_component(TARGET_NAME ${v} NAME_WE)
  add_executable(${TARGET_NAME} ${v})
endforeach()
```

- `file(GLOB_RECURSE ALL_SRCS *.cpp)`：递归收集目录下所有 `.cpp` 文件
- `get_filename_component(TARGET_NAME ${v} NAME_WE)`：提取无扩展名的文件名作为目标名
- `add_executable(${TARGET_NAME} ${v})`：每个源文件编译为独立的可执行文件

> **注意**：`GLOB` 模式不会自动检测新增文件。添加新 `.cpp` 后，需要重新运行 `cmake -B build` 或删除 `build/` 目录后重新配置，才能被 CMake 识别。

### 顶层 CMake 配置

[CMakeLists.txt](CMakeLists.txt) 的关键设置：

| 配置项 | 说明 |
|--------|------|
| `CMAKE_CXX_STANDARD 23` | 强制使用 C++23 |
| `CMAKE_C_STANDARD 11` | C 代码使用 C11 |
| `CMAKE_EXPORT_COMPILE_COMMANDS ON` | 导出 `compile_commands.json`，供 clangd 使用 |
| `add_compile_options(/utf-8)` (MSVC) | 启用 UTF-8 源码编码支持 |
| `include(cmake/check_arch.cmake)` | 构建时打印目标架构信息 |

## 开发工具

项目已预置以下工具配置，开箱即用：

| 工具 | 配置文件 | 用途 |
|------|---------|------|
| clang-format | [`.clang-format`](.clang-format) | 代码格式化（基于 Google 风格，120 列宽、2 空格缩进、自定义大括号换行策略） |
| clang-tidy | [`.clang-tidy`](.clang-tidy) | 静态分析（启用 bugprone / cert / modernize / performance / readability 等检查套件） |
| clangd | [`.clangd`](.clangd) + [`.vscode/settings.json`](.vscode/settings.json) | 语言服务器（补全、跳转、诊断、内联提示、跨文件重命名） |

VS Code 用户安装 **clangd 扩展**后即可获得完整的 IDE 体验。

> 若使用 MinGW + 独立 LLVM 的组合，可能需要额外配置 `--query-driver` 或手动指定头文件路径。详见 [`docs/CONFIG.md`](docs/CONFIG.md)。

## 文档索引

| 文档 | 内容 |
|------|------|
| [`docs/CONFIG.md`](docs/CONFIG.md) | 各编译器配置详解（Clang / GCC / MSVC）、Windows 编码问题、clangd 兼容性方案、常见问题速查 |

## 许可证

[Apache License 2.0](LICENSE)
