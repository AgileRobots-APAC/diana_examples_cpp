# 更新日志

[English](CHANGELOG.md) | [中文](CHANGELOG.cn.md)

## 1.0.0

- 首个版本：12 个 C++ 示例，涵盖连接、状态与诊断、关节运动、笛卡尔运动、运动学、IO、TCP 与坐标系、运动限制、路径与路点。
- 支持 Diana / Thor 系列机器人型号 profile：`--model diana3|diana7|diana7-g2|thor7|thor7-pro|thor12|thor20`、`--list-models` 与 `--ip <address>`。
- 连接后校验轴数，按实际轴数处理，不使用固定值。
- 运动示例默认 dry-run；真实运动需要显式指定 `--model` 与 `--run`。
- 提供中英文 README 与教程文档。
- 新增 Ubuntu 18.04、20.04、22.04 的 Docker 构建和 SDK 加载冒烟测试、一键测试脚本，以及平台兼容性/真机验证清单。
- 适配 SDK 3.3.0；`third_party/diana_sdk_cpp` 子模块固定到其 `v3.3.0` tag。
- 为本仓库的示例源码添加 MIT 许可证（`LICENSE`）。
