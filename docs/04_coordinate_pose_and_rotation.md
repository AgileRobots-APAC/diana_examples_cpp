# 04 Frames, Poses, and Rotations

The most common source of confusion is the difference between a target pose in the base frame and moving along the tool's own axes.

## Common Frame Concepts

- Base: robot base frame
- Flange: end flange frame
- TCP / Tool: tool center point frame
- Workpiece: workpiece frame, when used by the project

## `moveLToPose`

The target `pose[6]` is the target TCP pose in the base frame:

```text
[x, y, z, rx, ry, rz]
```

Position is in meters. Rotation uses an axis-angle vector in radians.

## Why "Z + 10 mm" Is Not Always "Move 10 mm Along Tool Z"

If you directly modify:

```cpp
pose[2] += 0.01;
```

that means adding 10 mm along the base-frame Z direction.

If the user needs to move along the current TCP's own axes, use tool-frame APIs such as `moveTCP` / `speedLOnTcp`, or perform the required pose transform first. Do not simply modify the base-frame pose.

## Related Examples

- [`04_cartesian_motion`](../examples/04_cartesian_motion/README.md)
- A dedicated TCP motion example can be added later.

## Axis-Angle and RPY

The `[rx, ry, rz]` values in Diana API poses are axis-angle rotation vectors in radians, not direct Roll/Pitch/Yaw values. `rpy2Axis(double arr[3])` and `axis2RPY(double arr[3])` both modify the array in place. Passing RPY directly to `moveLToPose` will produce an incorrect orientation.

Related examples: [`05_kinematics`](../examples/05_kinematics/README.md), [`07_tcp_and_frames`](../examples/07_tcp_and_frames/README.md).
