# FAQ

## 1. CMake 找不到 DianaAPI.h

确认 SDK 路径：

```bash
cmake .. -DDIANA_SDK_DIR=/your/path/to/diana_sdk_cpp
```

并确认存在：

```text
<SDK>/include/DianaAPI.h
```

## 2. 编译成功，运行找不到 libDianaApi.so

```bash
export LD_LIBRARY_PATH=/your/path/to/diana_sdk_cpp/lib:$LD_LIBRARY_PATH
```

## 3. ping 不通机器人

先解决网卡、IP、掩码和路由问题；不要先从 API 代码排查。

## 4. `rx ry rz` 是 RPY 吗？

默认不是。Diana API 常用旋转参数为轴角向量。需要 RPY 时使用 `axis2RPY / rpy2Axis` 转换。

## 5. 为什么运动例程不动？

为了避免客户下载后直接发生机器人运动。显式加：

```bash
--run
```

才会发送运动命令。

## 6. Diana 和 Thor 关节数不同怎么办？

示例使用 SDK 中的 `JOINT_NUM`，不在代码中固定写死 6 或 7。
