# 03 Joint Motion

[English](README.md) | [中文](README.cn.md)

Demonstrates `moveJToTarget`. The target is generated from the robot's current joint position by adding 10 degrees to J1, so the example does not assume that different robot models share the same home posture.

Default dry-run:

```bash
./build/bin/03_joint_motion --model diana7 --ip 192.168.10.75
```

Real motion requires both an explicit model and `--run`:

```bash
./build/bin/03_joint_motion --model diana7 --ip 192.168.10.75 --run
./build/bin/03_joint_motion --model thor12 --ip 192.168.10.75 --run
```

The program validates the joint count with `getJointCount()` before motion. Read [`docs/11_safety.md`](../../docs/11_safety.md) before running.
