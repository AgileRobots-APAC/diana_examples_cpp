# 06 IO

[English](README.md) | [中文](README.cn.md)

Demonstrates `readDI`, `readDO`, `readAI`, `readAO`, bus current / voltage reads, and optional `writeDO` and `writeAO`. The I/O API is not model-specific, but `--model` is still supported for post-connection joint-count validation.

```bash
./build/bin/06_io --model diana7 --ip 192.168.10.75
./build/bin/06_io --model thor12 --ip 192.168.10.75
```

Write `board/do0`:

```bash
./build/bin/06_io --model thor12 --ip 192.168.10.75 --set-do=1
```

Write analog output `board/ao0`:

```bash
./build/bin/06_io --model thor12 --ip 192.168.10.75 --set-ao=0:0.0
```

The `--set-ao` format is `<mode>:<value>`. Confirm the teach pendant AO configuration, wiring, and allowed output range before writing analog output.

Related documentation: [`docs/07_io.md`](../../docs/07_io.md).
