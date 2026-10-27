# 06 Cartesian Motion

When the TCP must reach a target along a straight line, use `moveLToPose`, `moveLToTarget`, or `moveL`. The target pose is the TCP pose in the base frame. Position is in meters, and rotation is an axis-angle vector in radians.

`04_cartesian_motion` only computes and prints the target by default. It sends motion commands only when both `--model` and `--run` are specified. Before execution, confirm TCP, payload, speed, clearance, and singularity risks.

Moving along the tool's own axes is different from moving along the base frame. The former requires tool-frame APIs or a pose transform first; see [08 TCP, frames, and payload](08_tcp_frames_and_payload.md).
