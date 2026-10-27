# Ubuntu Platform Compatibility Test

This repository provides Docker smoke tests to verify that the toolchain, SDK shared libraries, and all C++ examples can build and load correctly on Ubuntu 18.04, 20.04, and 22.04.

## Current Verification Results

Verification date: 2026-08-19. The SDK submodule was `5cd4d5f` (`industry-v3.3.0.260211`, Diana API SDK 3.3.0). The main repository baseline was `845350d`, including the platform test script and documentation changes.

| Ubuntu | Toolchain | 12 example builds | SDK shared library check | `--list-models` |
|---|---|---|---|---|
| 18.04 x64 | GCC 7.5 / CMake 3.10 | Pass | Pass, no `not found` | Pass |
| 20.04 x64 | GCC 9.4 / CMake 3.16 | Pass | Pass, no `not found` | Pass |
| 22.04 x64 | GCC 11.4 / CMake 3.22 | Pass | Pass, no `not found` | Pass |

Each result uses `DianaAPI.h` and `libDianaApi.so` from the SDK submodule. At runtime, `LD_LIBRARY_PATH` is set to the SDK `lib/` directory. `ldd` also confirms that the SDK-provided `libxml2.so` is loaded.

> This table is a Docker container compatibility result, not a complete real-hardware certification. It does not cover robot network connection, state reading, I/O, any `--run` motion, long-term stability, or real-time performance.

## Docker Smoke Test

Initialize the SDK submodule before the first run:

```bash
git submodule update --init --recursive
```

Test each version separately:

```bash
./scripts/test-ubuntu.sh 18.04
./scripts/test-ubuntu.sh 20.04
./scripts/test-ubuntu.sh 22.04
```

Or test all versions at once:

```bash
./scripts/test-ubuntu.sh all
```

Each container configures CMake, builds the 12 examples, checks the shared library dependencies of `libDianaApi.so`, and runs `01_connection --list-models`. It does not connect to a robot and does not execute `--run`.

Docker switches to the Aliyun Ubuntu APT mirror by default before installing build tools. The Ubuntu 18.04 base image has no certificate package before the first update, so HTTP is used by default. After build tools are installed, the container has `ca-certificates`. If the Tsinghua mirror is faster from your network, temporarily specify:

```bash
APT_MIRROR=https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ ./scripts/test-ubuntu.sh 18.04
```

The SDK includes `libxml2.so`, so the container does not install the system `libxml2`. If the output contains `not found`, the current Ubuntu version is missing a runtime library required by the SDK. Prefer the vendor-provided SDK for the corresponding system version.

## Real-Hardware Validation

A passing Docker result only means the program and SDK can load in the container. It does not prove real network communication or motion control.

On each target Ubuntu physical machine or virtual machine, run:

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j"$(getconf _NPROCESSORS_ONLN)"
cd ..
export LD_LIBRARY_PATH="$PWD/third_party/diana_sdk_cpp/lib:$LD_LIBRARY_PATH"

./build/bin/01_connection --model diana7 --ip <robot-ip>
./build/bin/02_robot_state --model diana7 --ip <robot-ip>
```

Then validate all default dry-run examples. Any test with `--run` must be executed manually only after confirming personnel safety, speed, payload, clearance, and emergency stop conditions on site.
