# 03 Robot State, Errors, and Diagnostics

## Error Callback

`initSrv` registers an error callback. The example callback calls:

```cpp
formatError(error_code, ip)
```

to print the error code and string description.

## API Return Values

Many common APIs use:

```text
0  success
-1 or <0  failure
```

After a failed call, do not continue assuming that motion was executed.

## Common Motion Errors

The control API documentation lists errors such as:

- IK_SINGULAR: inverse kinematics singularity
- IK_POSITION_LIMIT: inverse kinematics joint position limit exceeded
- IK_FAILURE: inverse kinematics failure
- PLAN_JOINTS: joint planning failure
- SPEED_LIMIT_CART: Cartesian speed limit exceeded
- TORQUE_LIMIT: torque limit exceeded

In real handling, combine `formatError`, robot state, and teach pendant alarm information.

## Callback Notes

Error callbacks may run on background threads:

- Do not `sleep` for a long time.
- Do not perform complex blocking I/O.
- Only record, set state flags, or send quick notifications.

## Diagnostic Data

In addition to joints, TCP, speed, current, and torque, `02_robot_state` reads link state, recent errors/warnings, collision flags, TCP six-axis external force, and joint external forces/torques. The example is read-only and does not send motion or configuration write commands.

Recent errors and warnings are the latest codes stored by the SDK. Interpret them together with teach pendant alarms and timing. TCP external force may be invalid near singular poses; do not move the robot merely to obtain a reading.
