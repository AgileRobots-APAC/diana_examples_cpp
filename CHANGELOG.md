# Changelog

[English](CHANGELOG.md) | [中文](CHANGELOG.cn.md)

## 1.0.0

- Initial release: 12 C++ examples covering connection, robot state and diagnostics, joint and Cartesian motion, kinematics, I/O, TCP and frames, motion limits, and path / waypoints.
- Support robot model profiles for Diana / Thor series: `--model diana3|diana7|diana7-g2|thor7|thor7-pro|thor12|thor20`, `--list-models`, and `--ip <address>`.
- Validate the joint count after connection; handle the actual joint count instead of a fixed value.
- Keep motion examples dry-run by default; real motion requires an explicit `--model` and `--run`.
- Add bilingual README and tutorial documents.
- Add Docker build and SDK loading smoke tests for Ubuntu 18.04, 20.04, and 22.04, a one-command test script, and a platform compatibility / real-hardware validation checklist.
- Compatible with SDK 3.3.0; the `third_party/diana_sdk_cpp` submodule is pinned to its `v3.3.0` tag.
- Add the MIT License (`LICENSE`) for the example source in this repository.
