# Robot model profiles

[English](README.md) | [中文](README.cn.md)

This directory stores only model-specific differences. It does not duplicate the full example set.

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

The core differences explicitly supported by current materials are:

- Diana series: 7 axes
- Thor series: 6 axes
- All models share the Diana API

Examples select the specific model with `--model` and call `getJointCount()` after connection to validate the joint count.

`thor7_pro.h` remains in the directory, but its profile is currently not registered and cannot be selected with `--model thor7-pro`.

For example:

```bash
./build/bin/02_robot_state --model diana7 --ip 192.168.10.75
./build/bin/02_robot_state --model thor12 --ip 192.168.10.75
```

> The current C++ API materials do not provide a general interface for reliably distinguishing specific models such as Diana3, Diana7, Thor7, or Thor12. Therefore, the user selects the specific model explicitly; `getJointCount()` is only used to validate whether the robot matches a 7-axis Diana or 6-axis Thor profile.

If a model later needs unique example poses, limits, or parameters, update only the corresponding profile. Do not duplicate `examples/`.
