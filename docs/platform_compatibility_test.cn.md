# Ubuntu 平台兼容性测试

本仓库提供 Docker 冒烟测试，用于验证 Ubuntu 18.04、20.04 和 22.04 中的工具链、SDK 动态库和全部 C++ 示例能否正常构建、加载。

## 当前验证结果

验证日期：2026-08-19。SDK 子模块为 `5cd4d5f`（`industry-v3.3.0.260211`，Diana API SDK 3.3.0）；主仓库基线为 `845350d`，并包含本次平台测试脚本与文档改动。

| Ubuntu | 工具链 | 12 个示例构建 | SDK 动态库检查 | `--list-models` |
|---|---|---|---|---|
| 18.04 x64 | GCC 7.5 / CMake 3.10 | 通过 | 通过，无 `not found` | 通过 |
| 20.04 x64 | GCC 9.4 / CMake 3.16 | 通过 | 通过，无 `not found` | 通过 |
| 22.04 x64 | GCC 11.4 / CMake 3.22 | 通过 | 通过，无 `not found` | 通过 |

每个结果都使用 SDK 子模块中的 `DianaAPI.h` 和 `libDianaApi.so` 构建；运行时设置 `LD_LIBRARY_PATH` 为 SDK 的 `lib/` 目录。`ldd` 还确认 SDK 自带的 `libxml2.so` 被加载。

> 此表是 Docker 容器兼容性结论，不是完整真机认证。尚未覆盖机器人网络连接、状态读取、I/O、任何 `--run` 运动、长期稳定性和实时性能。

## Docker 冒烟测试

首次运行前初始化 SDK 子模块：

```bash
git submodule update --init --recursive
```

分别测试：

```bash
./scripts/test-ubuntu.sh 18.04
./scripts/test-ubuntu.sh 20.04
./scripts/test-ubuntu.sh 22.04
```

或一次测试全部版本：

```bash
./scripts/test-ubuntu.sh all
```

每个容器会配置 CMake、编译 12 个示例、检查 `libDianaApi.so` 的动态库依赖，并运行 `01_connection --list-models`。不会连接机器人，也不会执行 `--run`。

Docker 默认在安装构建工具前切换到阿里云 Ubuntu APT 源。Ubuntu 18.04 基础镜像在首次更新前没有证书包，因此默认使用 HTTP；完成构建工具安装后容器已具备 `ca-certificates`。若网络到清华镜像更快，可临时指定：

```bash
APT_MIRROR=https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ ./scripts/test-ubuntu.sh 18.04
```

SDK 自带 `libxml2.so`，因此容器不额外安装系统 `libxml2`。若输出包含 `not found`，表示当前 Ubuntu 版本缺少 SDK 所需的运行时库；应优先使用厂商提供的对应系统版本 SDK。

## 真机验证

Docker 通过只表示程序和 SDK 可以在容器中加载，不能证明实际网络通信或运动控制。

在每个目标 Ubuntu 的实体机或虚拟机上，依次运行：

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j"$(getconf _NPROCESSORS_ONLN)"
cd ..
export LD_LIBRARY_PATH="$PWD/third_party/diana_sdk_cpp/lib:$LD_LIBRARY_PATH"

./build/bin/01_connection --model diana7 --ip <robot-ip>
./build/bin/02_robot_state --model diana7 --ip <robot-ip>
```

接着验证全部默认 dry-run 示例。任何带 `--run` 的测试都必须在现场完成人员、速度、负载、净空与急停确认后手工执行。
