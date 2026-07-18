# 说明

这是一个现代化的 C++ 项目模板，演示了基于 CMake 的多模块工程组织方式。

本项目以简单的几何形状计算（圆、矩形）作为示例业务，展示了如何划分模块边界、管理接口与实现分离、集成 GoogleTest 单元测试，以及配置现代化的
C++ 开发工具链（clangd、clang-format、clang-tidy）。你可以将示例代码替换为自己的项目代码，保留目录结构与构建配置即可。

## 特性

- **清晰的模块边界**：`core` 模块负责业务逻辑，`utils` 模块提供通用工具，主程序负责组装
- **接口与实现分离**：`include/` 仅暴露公开头文件，`impl/` 存放实现细节，通过 CMake `PUBLIC` / `PRIVATE` 控制可见性
- **现代 CMake 范式**：目标级属性设置、传递依赖管理、编译器抽象
- **开箱即用的测试**：通过 `FetchContent` 自动拉取 GoogleTest，CTest 自动发现测试用例
- **IDE 友好**：内置 clangd / VS Code / CLion 配置，支持代码补全、跳转、静态分析
- **跨平台支持**：Windows (MSVC / MinGW)、Linux (GCC / Clang)、macOS (Clang)

## 技术栈

| 组件     | 版本 / 说明                                |
|--------|----------------------------------------|
| C++ 标准 | C++23                                  |
| C 标准   | C11                                    |
| 构建系统   | CMake 3.20+                            |
| 生成器    | Ninja（推荐）、Unix Makefiles、Visual Studio |
| 测试框架   | GoogleTest v1.14.0                     |
| 代码格式化  | clang-format（Google 风格扩展）              |
| 静态检查   | clang-tidy                             |
| 语言服务器  | clangd                                 |

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

# 3. 运行测试
cd build && ctest --output-on-failure

# 4. 运行程序
./build/src/geometry_app-<your-system-name>
```

> 使用 MSVC 时建议通过 **Developer PowerShell for VS 2022** 执行，并在配置时指定
`-DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl`。详见 [`docs/CONFIG.md`](docs/CONFIG.md)。

## 项目结构

```text
TemplateRepoCxx/
├── cmake
│   └── check_arch.cmake
├── CMakeLists.txt
├── docs
│   └── CONFIG.md
├── LICENSE
├── README.md
├── src
│   ├── CMakeLists.txt
│   ├── core                        # 核心业务模块
│   │   ├── CMakeLists.txt
│   │   ├── impl
│   │   │   ├── circle.cpp
│   │   │   └── rectangle.cpp
│   │   └── include
│   │       ├── circle.h
│   │       ├── rectangle.h
│   │       └── shape.h
│   ├── main.cpp
│   └── utility                     # 通用工具模块
│       ├── CMakeLists.txt
│       ├── impl
│       │   └── logger
│       │       └── logger.cpp
│       ├── include
│       │   ├── logger
│       │   │   └── logger.h
│       │   └── singleton.h
│       └── tests
│           ├── CMakeLists.txt
│           └── test_logger.cpp
└── tests
    ├── CMakeLists.txt
    └── unit                          # 单元测试
        ├── test_circle.cpp
        └── test_rectangle.cpp
```

## 架构说明

### 模块设计

本项目采用**分层模块化**设计，核心原则：**接口最小暴露、实现细节隐藏**。

| 模块                  | 类型           | 职责         | 可见性                                       |
|---------------------|--------------|------------|-------------------------------------------|
| `core`              | `STATIC`     | 几何形状的定义与计算 | `include/` 为 `PUBLIC`，`impl/` 为 `PRIVATE` |
| `utility`           | `STATIC`     | 通用日志工具     | `include/` 为 `PUBLIC`，`impl/` 为 `PRIVATE` |
| `template_repo_cxx` | `EXECUTABLE` | 程序入口，组装各模块 | 私有依赖 `core` + `utility`                   |
| `unit_tests`        | `EXECUTABLE` | 单元测试       | 仅依赖 `core`（体现最小依赖原则）                      |

### CMake PUBLIC / PRIVATE 语义

```cmake
target_include_directories(core
        PUBLIC core/include   # 使用 core 的目标也能看到此目录
        PRIVATE core/impl      # 仅 core 自己能看到此目录
)
```

- **`PUBLIC`**：当前目标及其依赖方都需要的头文件路径（接口头文件）
- **`PRIVATE`**：仅当前目标编译时需要的头文件路径（实现细节）

这种区分避免了头文件路径泄漏，确保模块边界清晰。

## 测试

本项目使用 **GoogleTest** 作为测试框架，通过 CMake `FetchContent` 在配置阶段自动下载，无需手动安装。

```bash
# 运行全部测试
cd build && ctest --output-on-failure

# 运行测试可执行文件
./build/tests/unit_tests
./build/tests/unit_tests --gtest_filter=CircleTest.*
```

### 添加新测试

1. 在 `tests/unit/` 下新建 `test_xxx.cpp`
2. 包含被测头文件与 `<gtest/gtest.h>`
3. 在 `tests/CMakeLists.txt` 的 `add_executable(unit_tests ...)` 中添加新文件
4. 重新构建并运行 `ctest`

## 开发工具

项目已预置以下工具配置，开箱即用：

| 工具           | 配置文件                                                                    | 用途                                                            |
|--------------|-------------------------------------------------------------------------|---------------------------------------------------------------|
| clang-format | [`.clang-format`](.clang-format)                                        | 代码格式化（Google 风格扩展）                                            |
| clang-tidy   | [`.clang-tidy`](.clang-tidy)                                            | 静态分析（bugprone / cert / modernize / performance / readability） |
| clangd       | [`.clangd`](.clangd) + [`.vscode/settings.json`](.vscode/settings.json) | 语言服务器（补全、跳转、诊断）                                               |

VS Code 用户安装 **clangd 扩展**后即可获得完整的 IDE 体验。

> 若使用 MinGW + 独立 LLVM 的组合，可能需要额外配置 `--query-driver`。详见 [`docs/CONFIG.md`](docs/CONFIG.md)。

## 文档索引

| 文档                                 | 内容                                                       |
|------------------------------------|----------------------------------------------------------|
| [`docs/CONFIG.md`](docs/CONFIG.md) | 构建配置详解、工具链排错、GTest 引入方式、Windows 编码问题、clangd 兼容性方案、常见问题速查 |

## 许可证

[Apache License 2.0](LICENSE)
