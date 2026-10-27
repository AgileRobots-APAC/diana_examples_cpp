# 01 Connection

[English](README.md) | [中文](README.cn.md)

Demonstrates the Diana API lifecycle: `initSrvNetInfo`, `initSrv`, version and state reads, and `destroySrv`. It also adds robot model selection and `getJointCount()` validation.

```bash
./build/bin/01_connection --list-models
./build/bin/01_connection --model diana7 --ip 192.168.10.75
./build/bin/01_connection --model thor12 --ip 192.168.10.75
```

By default, this example does not call `releaseBrake()`. To explicitly demonstrate enable / disable behavior:

```bash
./build/bin/01_connection --model diana7 --ip 192.168.10.75 --enable
```

Related documentation: [`docs/02_robot_models_and_connection.md`](../../docs/02_robot_models_and_connection.md).
