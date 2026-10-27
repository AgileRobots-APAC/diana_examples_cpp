# 04 Cartesian Motion

[English](README.md) | [中文](README.cn.md)

演示 `moveLToPose`，目标为当前 TCP 在基坐标系 Z 方向 +10 mm。

默认 dry-run：

```bash
./build/bin/04_cartesian_motion --model diana7 --ip 192.168.10.75
```

真正运动：

```bash
./build/bin/04_cartesian_motion --model thor12 --ip 192.168.10.75 --run
```

`--run` 必须配合 `--model`，程序会先校验机器人轴数。运行前阅读 [`docs/11_safety.cn.md`](../../docs/11_safety.cn.md)。
