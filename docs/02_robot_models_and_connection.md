# 02 Robot Models and Connection

Diana API targets multiple robot models. This example repository keeps feature examples and robot models separate:

- `examples/`: shared features such as connection, state, MoveJ, MoveL, kinematics, and I/O.
- `robots/`: registered model profiles for Diana3, Diana7, Diana7 G2, Thor7, Thor12, and Thor20.

## 1. Joint Count Differences

The current documentation uses this convention:

```text
Diana series: 7 axes
Thor series : 6 axes
```

All models use the same Diana API. After connecting, call:

```cpp
int joint_count = getJointCount(ip);
```

to get the current robot joint count.

## 2. Why --model Is Still Needed

The joint count alone cannot identify a specific model. For example, 7 axes only indicates the Diana series, not whether the robot is Diana3, Diana7, or Diana7 G2. Likewise, 6 axes cannot distinguish Thor7, Thor12, and other Thor models.

Therefore, the C++ examples use explicit selection:

```bash
./build/bin/02_robot_state --model diana7 --ip 192.168.10.75
./build/bin/02_robot_state --model thor12 --ip 192.168.10.75
```

The program then calls `getJointCount()` to validate the joint count, avoiding accidental use of a Thor profile with a 7-axis Diana robot or vice versa.

## 3. List Supported Models

```bash
./build/bin/01_connection --list-models
```

Current supported keys:

```text
diana3
diana7
diana7-g2
thor7
thor12
thor20
```

The Thor7 Pro profile is currently not registered, so `--model thor7-pro` cannot be used.

## 4. Where Future Model Differences Belong

If a model later needs its own recommended example poses, speed limits, configuration notes, or similar differences, update only:

```text
robots/diana/<model>.h
robots/thor/<model>.h
```

Do not duplicate the entire `examples/` tree. This keeps the API call patterns maintained in one place.

## 5. Connection Lifecycle

The connection example initializes network information, sets the robot IP, creates the SDK connection, and registers error/state callbacks. Callbacks may run from background threads, so avoid long blocking work inside them.

A typical lifecycle for real motion is:

```text
initSrv -> read state / control -> stop (if needed) -> holdBrake (if needed) -> destroySrv
```

`releaseBrake()` starts the robot arm. After calling it, wait about two seconds before sending motion commands. `01_connection` does not enable the robot by default; it requests that operation only when `--enable` is passed explicitly.
