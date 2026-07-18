# 1. 源码下载

[g3log 源码](https://github.com/KjellKod/g3log)

```shell
git clone https://github.com/KjellKod/g3log.git
```

---

# 2. 构建和安装

1. **构建**：

```shell
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DG3_SHARED_LIB=OFF -DCMAKE_INSTALL_PREFIX=./install -DADD_G3LOG_UNIT_TEST=OFF -DADD_FATAL_EXAMPLE=OFF
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
│   └── g3log
│       ├── active.hpp
│       ├── atomicbool.hpp
│       ├── crashhandler.hpp
│       ├── filesink.hpp
│       ├── future.hpp
│       ├── g3log.hpp
│       ├── generated_definitions.hpp
│       ├── logcapture.hpp
│       ├── loglevels.hpp
│       ├── logmessage.hpp
│       ├── logworker.hpp
│       ├── moveoncopy.hpp
│       ├── shared_queue.hpp
│       ├── sinkhandle.hpp
│       ├── sink.hpp
│       ├── sinkwrapper.hpp
│       ├── stacktrace_windows.hpp
│       ├── stlpatch_future.hpp
│       └── time.hpp
└── lib
    ├── cmake
    │   └── g3log
    │       ├── g3logConfig.cmake
    │       ├── g3logTargets.cmake
    │       └── g3logTargets-release.cmake
    └── libg3log.a

6 directories, 23 files
```