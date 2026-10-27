# 机器人型号配置

[English](README.md) | [中文](README.cn.md)

本目录只保存“型号差异”，不复制整套 example。

```text
robots/
├── robot_profile.h
├── robot_registry.h/.cpp
├── diana/
│   ├── diana3.h
│   ├── diana7.h
│   └── diana7_g2.h
└── thor/
    ├── thor7.h
    ├── thor7_pro.h
    ├── thor12.h
    └── thor20.h
```

当前资料明确支持的核心差异：

- Diana 系列：7 轴
- Thor 系列：6 轴
- 所有型号共用 Diana API

示例通过 `--model` 选择具体型号，并在连接后调用 `getJointCount()` 校验轴数。

`thor7_pro.h` 保留在目录中，但其 profile 当前暂未注册，不能通过 `--model thor7-pro` 选择。

例如：

```bash
./build/bin/02_robot_state --model diana7 --ip 192.168.10.75
./build/bin/02_robot_state --model thor12 --ip 192.168.10.75
```

> C++ API 当前资料没有提供一个通用接口用于可靠地区分 Diana3 / Diana7 / Thor7 / Thor12 等“具体型号”。因此具体型号由用户显式选择；`getJointCount()` 只用于校验 7 轴 Diana / 6 轴 Thor 是否匹配。

以后如果某个型号存在独有的示例姿态、限制或参数，只修改对应 profile，不需要复制 `examples/`。
