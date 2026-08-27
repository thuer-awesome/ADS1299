# ADS1299 EEG Front-End Research Archive

这是 ADS1292、ADS1298、ADS1299 脑电/生物电前端工程资料的可追溯整理仓库。重点是把原理图、PCB、固件配置、TI 数据手册页码、电阻电容选型、测试点和故障定位放在同一套工程记录中。

## 快速入口

- [项目文档门户](docs/README.md)
- [数据手册追溯](docs/traceability/README.md)
- [三代 AFE 参考设计](docs/design/README.md)
- [MCU、SPI、UART、BLE](docs/interfaces/README.md)
- [测试与故障定位](docs/testing/README.md)
- [硬件测试点和制造门禁](docs/operations/README.md)
- [数据和可审查 CSV](data/README.md)
- [固件和协议](firmware/README.md)
- [验证报告模板](validation/README.md)
- [原始工程快照](hardware/source/README.md)

## 仓库结构

```text
.
├── .github/                 # Issue、PR 和 CI 校验
├── docs/                    # 追溯、设计、接口、测试、制造文档门户
├── hardware/                # 三代 Altium 源文件、参考设计、测试点、制造门禁
├── firmware/                # 三代固件源码快照、板级矩阵、协议说明
├── data/                    # 原始资料索引和处理后证据入口
├── tests/                   # 测试夹具、结果命名和记录规范
├── validation/              # 验证状态、报告模板和结果入口
├── manufacturing/           # ERC/DRC、BOM、Gerber、装配输出门禁
├── scripts/                 # 可复现的仓库/数据校验脚本
├── tools/                   # Altium inventory 等工程工具入口
├── third_party/             # 第三方资料和许可证边界
├── examples/                # 经过审查的寄存器/帧解析示例预留
├── releases/                # 发布和版本标签门禁
└── ADS129x_Research_Project_2026/  # 原始整理包和证据归档，保留兼容路径
```

根级目录是面向协作和 GitHub 浏览的项目门户；`ADS129x_Research_Project_2026/` 保留原始整理时的文件路径和证据表，不作为第二套独立实现。

## 版本范围

| 版本 | 工程背景 | 关键状态 |
|---|---|---|
| ADS1299 DV4.0.2 | 第六代双侧多通道，STM32F103T8 固件，主体 SchDoc 为 STM32F103T8 + RF-BM-BG22A3 | 8 通道、27 byte 帧；VCAP1=100 µF 和 AVDD=5 V 需优先核对 |
| ADS1298 DV2.4.1 | 第七代分体式八通道，固件目录为 STM32L432，主体 SchDoc 元件为 STM32L442KCU6 + RF-BM-BG22A3 | 8 通道、27 byte 帧；RTS/CTS 和 L432/L442 型号需实测确认 |
| ADS1292 DV1.7.8 | 双通道工作区；SchDoc 元件为 ADS1292-QFN（comment=ADS1292R）+ STM32L442KCU6 + RF-BM-4044B4 | 2 通道、9 byte 帧；4044B4 手册和正式 netlist 尚未归档 |

## 重要限制

仓库中的 `data/processed/` 表格区分 `DS-REQ`、`TI-TYP`、`PROJ-REC` 和 `MEAS-REQ`。当前测试状态保持 `OPEN`，因为没有把原理图分析当成实物电气验证。正式测试需要限流电源、示波器、逻辑分析仪、精密信号源、屏蔽输入夹具和 BLE 测试端。

原始 Altium/PDF/固件资料因路径和第三方许可原因没有全部复制到仓库；原始位置、大小和 SHA-256 见 [`data/raw/manifest.csv`](ADS129x_Research_Project_2026/data/raw/manifest.csv)。提交大文件时使用 Git LFS，并先确认许可和隐私要求。

仓库同时包含用户自有的关键工程文件和源码快照：`hardware/source/` 下为 ADS1292/ADS1298/ADS1299 的原理图、PCB、项目和库文件，`firmware/source/` 下为三代固件源码。第三方数据手册不复制到仓库，使用页码引用和 SHA-256 清单。许可证状态见 [`LICENSE_STATUS.md`](LICENSE_STATUS.md)。

## Research use only

本仓库用于科研和工程验证，不代表医疗器械认证、患者安全评估或量产批准。任何人体连接前必须完成输入漏电、RLD/lead-off 电流、ESD、充电隔离和电源故障测试。
