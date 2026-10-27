# 01 Getting Started

## Goal

Complete the shortest path from getting the repository to successfully reading robot state.

## Step 1: Prepare the Robot and PC

- Make sure the robot controller is powered on normally.
- Connect the PC to the controller API Ethernet port.
- Configure the PC and robot on the same subnet.
- For motion tests, make sure the emergency stop is reachable and the workspace is clear.

## Step 2: Check the Network

First, use the teach pendant or controller network settings to confirm and record the IP address and subnet mask of the robot's **API network port**. Do not treat an address in this tutorial as the robot's default IP, and do not change a robot's site network configuration just to match this example.

The following values are examples only:

```text
Robot API IP: 192.168.10.75
Subnet mask: 255.255.255.0
```

Based on the configuration confirmed on the teach pendant, configure the PC network adapter with an unused address in the same subnet. For example:

```text
192.168.10.100
255.255.255.0
```

Test connectivity with the actual IP confirmed on the teach pendant:

```bash
ping <robot-api-ip>
```

## Step 3: Prepare the SDK

When cloning for the first time, fetch the official Diana C/C++ SDK with Git submodules:

```bash
git clone --recurse-submodules <your-repo-url>
```

The SDK is located at:

```text
third_party/diana_sdk_cpp/
```

Alternatively, specify an external SDK directory with `-DDIANA_SDK_DIR=`.

If the main repository was already cloned, run:

```bash
git submodule update --init --recursive
```

## Step 4: Build

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

If the SDK is not under `third_party/diana_sdk_cpp/`, use:

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DDIANA_SDK_DIR=/path/to/diana_sdk_cpp
cmake --build . -- -j"$(getconf _NPROCESSORS_ONLN)"
export LD_LIBRARY_PATH=/path/to/diana_sdk_cpp/lib:$LD_LIBRARY_PATH
```

For the SDK layout, dependencies, and version replacement notes, see [`third_party/diana_sdk_cpp/README.md`](../third_party/diana_sdk_cpp/README.md).

## Step 5: Connect

```bash
./build/bin/01_connection --model <model> --ip <robot-api-ip>
```

## Step 6: Read State

```bash
./build/bin/02_robot_state --model <model> --ip <robot-api-ip>
```

If joint positions, TCP pose, and robot state are printed, the basic API communication path is working.

## Next Steps

- Learn math APIs without motion: `05_kinematics`
- Learn joint motion: `03_joint_motion`
- Learn linear motion: `04_cartesian_motion`


## Select the Robot Model

Explicitly passing `--model` is recommended. For details, see [02_robot_models_and_connection.md](02_robot_models_and_connection.md).

```bash
./build/bin/01_connection --list-models
./build/bin/01_connection --model diana7 --ip <robot-api-ip>
./build/bin/01_connection --model thor12 --ip <robot-api-ip>
```
