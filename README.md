# Diana API C++ Examples

[English](README.md) | [中文](README.cn.md)

C++ examples and learning notes for **Diana / Thor series robots** using the official Diana C/C++ SDK.

This repository is intended to help users clone the project, provide the official SDK, choose a robot model, pass the robot IP address, and learn the common API workflow from connection and state reading through motion, I/O, frames, payloads, paths, and realtime commands.

> This project does not wrap or replace the official robot SDK. The programs in `examples/` call the original Diana API directly where possible. The `common/` directory only contains shared helpers for argument parsing, callbacks, waiting, printing, and robot model validation.

## Supported Models

```text
Diana series (7 axes)
├── Diana3       -> --model diana3
├── Diana7       -> --model diana7
└── Diana7 G2    -> --model diana7-g2

Thor series (6 axes)
├── Thor7        -> --model thor7
├── Thor12       -> --model thor12
└── Thor20       -> --model thor20
```

All models share the same examples. Model-specific data is kept in [`robots/`](robots/), and each connection can be checked with `getJointCount()` to validate Diana 7-axis robots and Thor 6-axis robots.

> The Thor7 Pro profile exists in the source tree but is not registered yet, so `--model thor7-pro` is not currently available.

See [`docs/02_robot_models_and_connection.md`](docs/02_robot_models_and_connection.md) for details.

## Learning Tutorials (recommended order)

New users should follow this order. The tutorials progress from connection and state reading to motion and advanced control. Complete the earlier read-only checks before running examples that write I/O or move the robot.

- [01 Getting started](docs/01_getting_started.md)
- [02 Robot models and connection](docs/02_robot_models_and_connection.md)
- [03 Robot state, errors, and diagnostics](docs/03_robot_state_and_diagnostics.md)
- [04 Frames, poses, and rotations](docs/04_coordinate_pose_and_rotation.md)
- [05 Joint motion](docs/05_joint_motion.md)
- [06 Cartesian motion](docs/06_cartesian_motion.md)
- [07 I/O](docs/07_io.md)
- [08 TCP, frames, and payload](docs/08_tcp_frames_and_payload.md)
- [09 Motion limits](docs/09_motion_limits.md)
- [10 Paths and waypoints](docs/10_path_and_waypoints.md)
- [11 Safety](docs/11_safety.md)
- [12 Advanced motion](docs/12_advanced_motion.md)

The tutorials explain why, when, and how to use each API area. The runnable C++ code for each area is listed below.

- [Ubuntu 18.04 / 20.04 / 22.04 compatibility test](docs/platform_compatibility_test.md)

## Runnable Examples

The following links describe the command, parameters, and usage notes for each runnable example.

- [01 Connection and model validation](examples/01_connection/README.md)
- [02 Robot state and diagnostics](examples/02_robot_state/README.md)
- [03 Joint motion](examples/03_joint_motion/README.md)
- [04 Cartesian motion](examples/04_cartesian_motion/README.md)
- [05 Kinematics](examples/05_kinematics/README.md)
- [06 I/O](examples/06_io/README.md)
- [07 TCP, frames, and payload](examples/07_tcp_and_frames/README.md)
- [08 Motion limits](examples/08_motion_limits/README.md)
- [09 Paths and waypoints](examples/09_path_and_waypoints/README.md)
- [10 Jog motion](examples/10_jog_motion/README.md)
- [11 Force guarded motion](examples/11_force_guarded_motion/README.md)
- [12 Realtime ServoL](examples/12_servoL_realtime/README.md)

### Example Function Overview

Complete the relevant tutorial and read the safety notes before running examples that write I/O or move the robot.

| Example | Description | Moves by default |
|---|---|---|
| [`01_connection`](examples/01_connection/README.md) | Connection lifecycle, version, model / joint-count validation, state, optional enable / disable | No |
| [`02_robot_state`](examples/02_robot_state/README.md) | Robot state, external force, link status, errors, warnings, and collision diagnostics | No, read-only |
| [`03_joint_motion`](examples/03_joint_motion/README.md) | Joint motion with `moveJToTarget` | No, requires `--run` |
| [`04_cartesian_motion`](examples/04_cartesian_motion/README.md) | Linear Cartesian motion with `moveLToPose` | No, requires `--run` |
| [`05_kinematics`](examples/05_kinematics/README.md) | Forward kinematics, inverse kinematics, reference IK, RPY / axis-angle conversion | No |
| [`06_io`](examples/06_io/README.md) | DI / DO / AI / AO and bus electrical data, with optional DO / AO writes | No, writes require explicit arguments |
| [`07_tcp_and_frames`](examples/07_tcp_and_frames/README.md) | TCP, frames, payload, and tool-relative offset calculation | No, read-only |
| [`08_motion_limits`](examples/08_motion_limits/README.md) | Actual motion limits reported by the controller | No, read-only |
| [`09_path_and_waypoints`](examples/09_path_and_waypoints/README.md) | Basic or complex multi-waypoint joint paths with `--complex` | No, requires `--run` |
| [`10_jog_motion`](examples/10_jog_motion/README.md) | Low-speed, short-duration J1 jog example | No, requires `--run` |
| [`11_force_guarded_motion`](examples/11_force_guarded_motion/README.md) | Low-speed Cartesian motion stopped by a force threshold | No, requires `--run` |
| [`12_servoL_realtime`](examples/12_servoL_realtime/README.md) | 10 ms realtime `servoL_ex` command example | No, requires `--run` |

## Runtime Setup

### SDK Setup

This repository references the official DianaApi SDK as a Git submodule. Clone with submodules enabled:

```bash
git clone --recurse-submodules <your-repo-url>
```

If the main repository has already been cloned, initialize the submodule with:

```bash
git submodule update --init --recursive
```

For the SDK version, directory layout, CMake configuration, and Linux runtime library setup, see [`third_party/diana_sdk_cpp/README.md`](third_party/diana_sdk_cpp/README.md).

### Build

Run these commands from the repository root. By default, CMake uses the SDK under `third_party/diana_sdk_cpp/`:

```bash
git clone --recurse-submodules <your-repo-url>
cd diana-api-examples-cpp
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j"$(getconf _NPROCESSORS_ONLN)"
cd ..
```

Executables are generated under:

```text
build/bin/
```

Before running the examples, set the SDK library path and try a command that does not connect to a robot:

```bash
export LD_LIBRARY_PATH="$PWD/third_party/diana_sdk_cpp/lib:$LD_LIBRARY_PATH"
./build/bin/01_connection --list-models
```

If the SDK is located outside this repository, pass its root directory to CMake:

```bash
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release \
  -DDIANA_SDK_DIR=/path/to/diana_sdk_cpp
cmake --build . -- -j"$(getconf _NPROCESSORS_ONLN)"
cd ..
export LD_LIBRARY_PATH=/path/to/diana_sdk_cpp/lib:$LD_LIBRARY_PATH
```

### First Run

List supported models:

```bash
./build/bin/01_connection --list-models
```

Connect to a Diana7:

```bash
./build/bin/01_connection --model diana7 --ip 192.168.10.75
```

Connect to a Thor12:

```bash
./build/bin/01_connection --model thor12 --ip 192.168.10.75
```

You can run non-motion examples without `--model`, but the program can only infer the Diana / Thor series from the joint count. It cannot reliably identify the exact robot model.

Before connecting, make sure the PC and robot are on the same network segment:

```bash
ping 192.168.10.75
```

## Safety

Motion examples such as `03_joint_motion` and `04_cartesian_motion` calculate and print their targets by default. They do not send motion commands unless `--run` is specified.

Starting with v0.2, real motion requires an explicit robot model:

```bash
./build/bin/03_joint_motion --model diana7 --ip 192.168.10.75 --run
./build/bin/04_cartesian_motion --model thor12 --ip 192.168.10.75 --run
```

The examples also call `getJointCount()` after connecting to validate the model family. Read [`docs/11_safety.md`](docs/11_safety.md) before running motion examples.

## Compatibility Tests

Docker smoke tests build all 12 examples, check SDK dynamic library dependencies, and run `--list-models` without connecting to a robot:

```bash
./scripts/test-ubuntu.sh 18.04
./scripts/test-ubuntu.sh 20.04
./scripts/test-ubuntu.sh 22.04
```

See [`docs/platform_compatibility_test.md`](docs/platform_compatibility_test.md) for details and a real-robot validation checklist.

## Repository Layout

| Path | Description |
|---|---|
| [`docs/`](docs/) | Tutorials in learning order, safety notes, FAQ, and compatibility checks. |
| [`examples/`](examples/) | Runnable C++ examples that correspond to the tutorials. |
| [`common/`](common/) | Shared helper code for examples. |
| [`robots/`](robots/) | Robot model profiles and registry. |
| [`third_party/diana_sdk_cpp/`](third_party/diana_sdk_cpp/) | Official DianaApi SDK submodule and accompanying API manual. |
| [`docker/`](docker/) | Docker environment used by compatibility tests. |
| [`scripts/`](scripts/) | Utility scripts, including Ubuntu smoke tests. |

## Documentation Basis

The examples and tutorials are based on the Diana industrial robot API manual for C/C++ and the Diana controller interface documentation. Always use the official API manual that matches the SDK version installed at the customer site as the final reference.


## Compatibility

| Examples version | SDK version | SDK `version` file |
|---|---|---|
| 1.0.0 | 3.3.0 | `industry-v3.3.0.260211` |

The SDK submodule is pinned to the SDK tag listed above. To use another SDK version, see "SDK Setup" and verify the examples against it before use.

## License

The example source in this repository is licensed under the MIT License; see [`LICENSE`](LICENSE). The Diana SDK referenced as a submodule is a separate repository and is not covered by this license.
