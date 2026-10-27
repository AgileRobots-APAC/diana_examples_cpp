# 05 Kinematics

[English](README.md) | [中文](README.cn.md)

Demonstrates `forward`, `inverse`, `inverse_ext`, `rpy2Axis`, and `axis2RPY`. `inverse_ext` uses the current joint angles as a reference, so in multi-solution cases it prefers the solution closest to the current posture.

The joint array reserves 7 elements, but output is limited to the actual joint count returned by `getJointCount()`.

```bash
./build/bin/05_kinematics --model diana7 --ip 192.168.10.75
./build/bin/05_kinematics --model thor12 --ip 192.168.10.75
```

Related documentation: [`docs/04_coordinate_pose_and_rotation.md`](../../docs/04_coordinate_pose_and_rotation.md).
