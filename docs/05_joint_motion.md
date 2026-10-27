# 05 Joint Motion

## Move to a Target Without Constraining the TCP Path

Prefer joint-space motion:

```text
moveJToTarget / moveJToPose / moveJ
```

Repository example: `03_joint_motion`

## Model Selection

When sending real motion commands, the example requires an explicit `--model` and validates the joint count with `getJointCount()`. For example:

```bash
./build/bin/03_joint_motion --model diana7 --ip 192.168.10.75 --run
./build/bin/03_joint_motion --model thor12 --ip 192.168.10.75 --run
```
