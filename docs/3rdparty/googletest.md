# 1. 源码下载

[g3log 源码](https://github.com/KjellKod/g3log)

```shell
git clone https://github.com/KjellKod/g3log.git
```

---

# 2. 构建和安装

1. **构建**：

```shell
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install
```

`-DCMAKE_INSTALL_PREFIX=./install` 表示安装到当前目录下的 `install` 目录中。

可以执行下面这条命令查看 cmake 构建时的选项：

```shell
cmake -B build -LAH
```

2. **编译**：

```shell
cmake --build build/ -j4
```

3. **安装**：

```shell
cmake --install build/
```

目录结构：

```text
install/
├── include
│   ├── gmock
│   │   ├── gmock-actions.h
│   │   ├── gmock-cardinalities.h
│   │   ├── gmock-function-mocker.h
│   │   ├── gmock.h
│   │   ├── gmock-matchers.h
│   │   ├── gmock-more-actions.h
│   │   ├── gmock-more-matchers.h
│   │   ├── gmock-nice-strict.h
│   │   ├── gmock-spec-builders.h
│   │   └── internal
│   │       ├── custom
│   │       │   ├── gmock-generated-actions.h
│   │       │   ├── gmock-matchers.h
│   │       │   ├── gmock-port.h
│   │       │   └── README.md
│   │       ├── gmock-internal-utils.h
│   │       ├── gmock-port.h
│   │       └── gmock-pp.h
│   └── gtest
│       ├── gtest-assertion-result.h
│       ├── gtest-death-test.h
│       ├── gtest.h
│       ├── gtest-matchers.h
│       ├── gtest-message.h
│       ├── gtest-param-test.h
│       ├── gtest_pred_impl.h
│       ├── gtest-printers.h
│       ├── gtest_prod.h
│       ├── gtest-spi.h
│       ├── gtest-test-part.h
│       ├── gtest-typed-test.h
│       └── internal
│           ├── custom
│           │   ├── gtest.h
│           │   ├── gtest-port.h
│           │   ├── gtest-printers.h
│           │   └── README.md
│           ├── gtest-death-test-internal.h
│           ├── gtest-filepath.h
│           ├── gtest-internal.h
│           ├── gtest-param-util.h
│           ├── gtest-port-arch.h
│           ├── gtest-port.h
│           ├── gtest-string.h
│           └── gtest-type-util.h
└── lib
    ├── cmake
    │   └── GTest
    │       ├── GTestConfig.cmake
    │       ├── GTestConfigVersion.cmake
    │       ├── GTestTargets.cmake
    │       └── GTestTargets-release.cmake
    ├── libgmock.a
    ├── libgmock_main.a
    ├── libgtest.a
    ├── libgtest_main.a
    └── pkgconfig
        ├── gmock_main.pc
        ├── gmock.pc
        ├── gtest_main.pc
        └── gtest.pc

12 directories, 52 files
```

如果只用 cmake 进行构建，不用 `pkg-config` 工具，那么那么 `pkgconfig` 目录（所有的 `.pc` 文件）是可以删除的。
