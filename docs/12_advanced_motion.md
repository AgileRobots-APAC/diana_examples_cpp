# 12 Advanced Motion

`10_jog_motion`, `11_force_guarded_motion`, and `12_servoL_realtime` are all dry-run by default. Real execution requires both `--model` and `--run`.

- Jog: `10_jog_motion` performs a low-speed, short-duration positive J1 jog by default. `--mode tcp` calls `moveTcp_ex` to translate along base-frame +X, and `--mode rotate` calls `rotationTCP_ex` to rotate around base-frame +Z. All three modes call `stop()` and `holdBrake()` after completion.
- Force-guarded stop: during low-speed Cartesian motion, the example polls TCP external force and stops when the threshold is reached or timeout occurs. Complete site force-control and collision-risk review first.
- ServoL: generates a 10 ms cycle sine trajectory around the current TCP along base-frame Z, with +/-5 cm amplitude and 10 cm total travel. It runs for 20 seconds (2000 cycles) and returns to the start. This is only for understanding the API pattern. Regular Linux does not guarantee hard real-time behavior and should not be used directly as a production real-time control solution.

Before executing any advanced motion, follow [Safety Notes](11_safety.md) and validate at low speed, unloaded, in a workspace with sufficient clearance.
