# ADS1299 EEG Front-End Research Archive

这是 ADS1292、ADS1298、ADS1299 脑电/生物电前端工程资料的可追溯整理仓库。重点是把原理图、PCB、固件配置、TI 数据手册页码、电阻电容选型、测试点和故障定位放在同一套工程记录中。

## 快速入口

- [整理项目总览](ADS129x_Research_Project_2026/README.md)
- [数据手册逐页追溯](ADS129x_Research_Project_2026/docs/01_datasheet_traceability.md)
- [ADS1299 测试版参考设计](ADS129x_Research_Project_2026/docs/02_ads1299_test_reference_design.md)
- [ADS1298 参考设计](ADS129x_Research_Project_2026/docs/03_ads1298_reference_design.md)
- [ADS1292 参考设计](ADS129x_Research_Project_2026/docs/04_ads1292_reference_design.md)
- [MCU/BLE 接口](ADS129x_Research_Project_2026/docs/05_mcu_ble_interface.md)
- [测试计划与故障定位](ADS129x_Research_Project_2026/docs/07_test_plan_and_fault_isolation.md)
- [可审查 CSV](ADS129x_Research_Project_2026/data/processed/)
- [原始硬件与固件源码索引](ADS129x_Research_Project_2026/REPO_HANDOFF.md)

## 版本范围

| 版本 | 工程背景 | 关键状态 |
|---|---|---|
| ADS1299 DV4.0.2 | 第六代双侧多通道，STM32F103T8 固件，主体 SchDoc 为 STM32F103T8 + RF-BM-BG22A3 | 8 通道、27 byte 帧；VCAP1=100 µF 和 AVDD=5 V 需优先核对 |
| ADS1298 DV2.4.1 | 第七代分体式八通道，固件目录为 STM32L432，主体 SchDoc 元件为 STM32L442KCU6 + RF-BM-BG22A3 | 8 通道、27 byte 帧；RTS/CTS 和 L432/L442 型号需实测确认 |
| ADS1292 DV1.7.8 | 双通道工作区；SchDoc 元件为 ADS1292-QFN（comment=ADS1292R）+ STM32L442KCU6 + RF-BM-4044B4 | 2 通道、9 byte 帧；4044B4 手册和正式 netlist 尚未归档 |

## 重要限制

仓库中的 `data/processed/` 表格区分 `DS-REQ`、`TI-TYP`、`PROJ-REC` 和 `MEAS-REQ`。当前测试状态保持 `OPEN`，因为没有把原理图分析当成实物电气验证。正式测试需要限流电源、示波器、逻辑分析仪、精密信号源、屏蔽输入夹具和 BLE 测试端。

原始 Altium/PDF/固件资料因路径和第三方许可原因没有全部复制到仓库；原始位置、大小和 SHA-256 见 [`data/raw/manifest.csv`](ADS129x_Research_Project_2026/data/raw/manifest.csv)。提交大文件时使用 Git LFS，并先确认许可和隐私要求。

仓库同时包含用户自有的关键工程文件和源码快照：`hardware/source/` 下为 ADS1292/ADS1298/ADS1299 的原理图、PCB、项目和库文件，`firmware/source/` 下为三代固件源码。第三方数据手册不复制到仓库，使用页码引用和 SHA-256 清单。

## Research use only

本仓库用于科研和工程验证，不代表医疗器械认证、患者安全评估或量产批准。任何人体连接前必须完成输入漏电、RLD/lead-off 电流、ESD、充电隔离和电源故障测试。
