# 11 Safety Notes

> All motion examples are API call references only. Users must run them only after assessing the actual robot model, installation, tool, payload, and site risks.

## Before the First Run

- Make sure the emergency stop button is always reachable.
- Make sure no people or obstacles are inside the robot motion range.
- Confirm that the tool is mounted securely and that TCP and payload are configured correctly.
- Start with low speed and low acceleration.
- Do not directly copy absolute joint positions or absolute poses from another robot site.

## Why Motion Examples Do Not Move by Default

By default, `03_joint_motion` and `04_cartesian_motion`:

1. Connect to the robot.
2. Read the current position.
3. Compute a small example target.
4. Print the target.
5. **Do not send motion commands.**

They execute only when `--run` is explicitly added.

## `releaseBrake`

The API manual says to wait about two seconds after calling `releaseBrake` before doing other operations. The examples follow this.

## `stop`

`stop` stops the current task. Do not treat the example program's software stop logic as a replacement for emergency stop functionality.

## Site Release

Before releasing to customers, have a robot application or safety owner validate every motion-capable example on real hardware and define recommended speed ranges.
