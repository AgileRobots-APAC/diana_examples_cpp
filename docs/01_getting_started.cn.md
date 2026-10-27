# 01 快速开始

## 目标

完成从“拿到仓库”到“成功读取机器人状态”的最短路径。

## 步骤 1：准备机器人与 PC

- 机器人控制箱正常上电。
- PC 使用以太网连接控制箱 API 网络口。
- PC 与机器人配置到同一子网。
- 运动测试时确保急停可触及、工作空间无人且无障碍物。

## 步骤 2：确认网络

先在示教器或控制器的网络设置中，确认并记录机器人 **API 网络口**的 IP 地址和子网掩码。不要把本教程中的地址当成机器人默认 IP，也不要仅因教程示例而修改现场机器人网络配置。

以下仅为示例：

```text
机器人 API IP：192.168.10.75
子网掩码：255.255.255.0
```

根据示教器确认的网络配置，将 PC 网卡设置为同一子网内且未被占用的地址。例如：

```text
192.168.10.100
255.255.255.0
```

使用示教器确认的实际 IP 测试连通性：

```bash
ping <robot-api-ip>
```

## 步骤 3：准备 SDK

首次克隆时，使用 Git submodule 一并获取官方 Diana C/C++ SDK：

```bash
git clone --recurse-submodules <your-repo-url>
```

SDK 位于：

```text
third_party/diana_sdk_cpp/
```

或通过 `-DDIANA_SDK_DIR=` 指定外部目录。

如果已经克隆主仓库，再执行：

```bash
git submodule update --init --recursive
```

## 步骤 4：编译

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

如果 SDK 不在 `third_party/diana_sdk_cpp/`，可改用：

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DDIANA_SDK_DIR=/path/to/diana_sdk_cpp
cmake --build . -- -j"$(getconf _NPROCESSORS_ONLN)"
export LD_LIBRARY_PATH=/path/to/diana_sdk_cpp/lib:$LD_LIBRARY_PATH
```

SDK 的目录结构、依赖库和版本替换规则见 [`third_party/diana_sdk_cpp/README.cn.md`](../third_party/diana_sdk_cpp/README.cn.md)。

## 步骤 5：连接

```bash
./build/bin/01_connection --model <model> --ip <robot-api-ip>
```

## 步骤 6：读取状态

```bash
./build/bin/02_robot_state --model <model> --ip <robot-api-ip>
```

如果能看到关节角、TCP 位姿和机器人状态，最基础的 API 通信已经打通。

## 下一步

- 不运动学习数学接口：`05_kinematics`
- 学习关节运动：`03_joint_motion`
- 学习直线运动：`04_cartesian_motion`


## 选择机器人型号

推荐显式使用 `--model`。详细说明见 [02_robot_models_and_connection.cn.md](02_robot_models_and_connection.cn.md)。

```bash
./build/bin/01_connection --list-models
./build/bin/01_connection --model diana7 --ip <robot-api-ip>
./build/bin/01_connection --model thor12 --ip <robot-api-ip>
```
