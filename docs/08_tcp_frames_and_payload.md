# 08 TCP, Frames, and Payload

`07_tcp_and_frames` only reads the current TCP, default active TCP, default TCP/workpiece frame names, and active TCP payload. Confirm these settings before discussing tool-frame motion. Do not modify TCP, payload, or workpiece frames in introductory examples.

When querying the default TCP or workpiece frame name, the SDK requires the caller to provide a name buffer of at least 256 bytes. The example follows that requirement.

Payload includes mass, center of mass, and inertia. It affects trajectory planning, dynamics compensation, and force-control APIs.

Tool-relative offsets are converted to base-frame targets with "current TCP homogeneous matrix x relative transform in the tool frame". For `--offset=x,y,z,roll,pitch,yaw`, position is in meters and angles are in degrees. The example only computes and prints the target; it does not execute motion.
