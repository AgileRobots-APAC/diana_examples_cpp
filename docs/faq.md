# FAQ

## 1. CMake Cannot Find DianaAPI.h

Check the SDK path:

```bash
cmake .. -DDIANA_SDK_DIR=/your/path/to/diana_sdk_cpp
```

and confirm that this file exists:

```text
<SDK>/include/DianaAPI.h
```

## 2. Build Succeeds but Runtime Cannot Find libDianaApi.so

```bash
export LD_LIBRARY_PATH=/your/path/to/diana_sdk_cpp/lib:$LD_LIBRARY_PATH
```

## 3. Cannot ping the robot

Resolve network adapter, IP, mask, and routing issues first. Do not start by debugging the API code.

## 4. Are `rx ry rz` RPY?

Not by default. Diana API commonly uses axis-angle vectors for rotation parameters. Use `axis2RPY / rpy2Axis` when RPY conversion is needed.

## 5. Why Do Motion Examples Not Move?

To avoid unexpected robot motion immediately after download. Add explicitly:

```bash
--run
```

to send motion commands.

## 6. What About Different Joint Counts Between Diana and Thor?

The examples use `JOINT_NUM` from the SDK and do not hard-code 6 or 7 in the code.
