# 1. geometry_app 项目模板说明

> 这只是一个项目模板，可以把里面的源文件和头文件替换成自己项目的。

项目是一个 **geometry_app**，包含两个模块 `core`（形状计算）和 `utils`（日志），点击任意文件可查看完整内容。

几个值得注意的设计点：

- **`include/` 与 `impl/` 的边界**：`core/include/` 里只有接口声明（`.h`），调用方只需包含这一层。`impl/` 里的实现细节对外完全不可见，CMake 把 `impl/` 设为 `PRIVATE`，编译器不会将它的路径传递给依赖方。

- **CMake 的 `PUBLIC` / `PRIVATE` 区分**：`target_include_directories` 中 `PUBLIC` 表示"我自己用，依赖我的人也能用"，`PRIVATE` 表示"只有我自己用"。这是现代 CMake 的核心思路，避免头文件路径泄漏。

- **测试只依赖 `core`，不依赖 `utils`**：`tests/CMakeLists.txt` 里 `target_link_libraries` 只链接了 `core` 和 `GTest`，体现了模块边界清晰的好处——测试目标最小化。

**构建命令**：
```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
cmake --build build
cd build && ctest --output-on-failure
```

# 2. 配置说明

详见 [CONFIG.md](./docs/CONFIG.md)
