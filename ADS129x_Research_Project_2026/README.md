# ADS129x EEG Front-End Research Project

本目录是对 ADS1292、ADS1298、ADS1299 三个硬件版本的工程化整理。它把现有 Altium 原理图/PCB、固件、测试记录和 TI 数据手册建立成可追溯的研究项目目录；原始资料仍保存在工作区原位置，本目录只保存索引、核对表和可复现实验记录模板。

## 项目边界

- `ADS1299 DV4.0.2`：第六代双侧多通道脑电，原工程为 STM32F103T8 + BG22A 系列，8 通道，SPI1。
- `ADS1298 DV2.4.1`：第七代分体式八通道，原工程为 STM32L432 + BG22A 系列，8 通道，SPI3。
- `ADS1292 DV1.7.8`：当前工作区双通道版本，工程目录为 `2022-09-26 ADS1292 DV1.7.8（调整）STM32L432+4404B 双通道`。

本项目是研究和工程验证资料，不是医疗器械认证文件。没有实物、示波器、电源分析仪、精密信号源和人体安全评估时，不能把“设计已通过”写成“已验证”。

## 目录

```text
ADS129x_Research_Project_2026/
├── README.md
├── CONTRIBUTING.md
├── REPO_HANDOFF.md
├── .github/               # issue/PR 模板
├── docs/
│   ├── 01_datasheet_traceability.md       # 数据手册页码、图表与设计规则
│   ├── 02_ads1299_test_reference_design.md # ADS1299 严谨测试版基线
│   ├── 03_ads1298_reference_design.md      # ADS1298 独立参考设计
│   ├── 04_ads1292_reference_design.md      # ADS1292 独立参考设计
│   ├── 05_mcu_ble_interface.md              # MCU、SPI、UART、BG22A 接口
│   ├── 06_pcb_layout_and_grounding.md       # 分区、回流、射频与测试点
│   ├── 07_test_plan_and_fault_isolation.md  # 测试步骤、判据、故障定位
│   ├── 08_bom_selection_rules.md            # 电阻电容、电源和保护器件
│   └── 09_change_log.md                     # 整理记录和未决事项
├── hardware/
│   ├── ads1299_dv4.0.2/README.md
│   ├── ads1298_dv2.4.1/README.md
│   └── ads1292_dv1.7.8/README.md
├── firmware/
│   ├── ads1299_stm32f103/README.md
│   ├── ads1298_stm32l432/README.md
│   └── ads1292_stm32l432/README.md
├── data/
│   ├── raw/        # 原始资料索引、manifest.csv，不复制大文件
│   └── processed/  # 引脚/BOM/寄存器/测试矩阵和实测结果
└── tools/README.md
```

## 原始资料索引

| 版本 | 原理图/PCB/固件 | 数据手册或用户指南 |
|---|---|---|
| ADS1299 DV4.0.2 | `E:\ADS1299\脑机图设备开发汇总20260706\第六代（双侧多通道脑电）\ADS1299 DV4.0.2 资料 2022-03-15\ADS1299 DV4.0.2 资料 2022-03-15` | 同目录 `ads1299.pdf`、`slau443b 1299用户手册.pdf` |
| ADS1298 DV2.4.1 | `E:\ADS1299\脑电资料\脑机图设备开发汇总20260706\第七代 （分体式八通道）\2023-06-14 ADS1298 DV2.4.1 资料整合(1)\2023-06-14 ADS1298 DV2.4.1 资料整合` | 同目录 `ads1298中文.pdf` |
| ADS1292 DV1.7.8 | `E:\ADS1299\2022-09-26 ADS1292 DV1.7.8（调整）STM32L432+4404B 双通道` | 工作区资料或 ADS1292/ADS1292R 数据手册 `ads1292.pdf` |
| BLE | 六、七代资料目录中的 `RF-BM-BG22A1 模块硬件规格书_210121.pdf`、EFR32BG22 协议 PDF | 见 `docs/05_mcu_ble_interface.md` |

## 证据等级

每条设计结论必须标记为以下之一：

1. **DS-REQ**：数据手册明确的工作范围、时序、连接或禁用条件。
2. **TI-TYP**：数据手册典型应用图或推荐值，需说明适用条件，不能冒充硬性规格。
3. **PROJ-REC**：本项目为可制造性、EMI、调试便利性提出的建议值。
4. **MEAS-REQ**：必须在样板上测量的项目，不能由原理图推断。

## 可审查表格

- `data/processed/pinout.csv`：三代芯片、MCU、BLE 引脚映射和数据手册页码。
- `data/processed/power_bom.csv`：VCAP、VREF、供电去耦、RLD/BIAS 和电源器件选型。
- `data/processed/register_baseline.csv`：寄存器默认值、项目旧固件值、回读方法。
- `data/processed/reference_netlist.csv`：测试版目标网络连接，明确哪些仍待 netlist/实测。
- `data/processed/test_matrix.csv`：上电、时钟、SPI、噪声、RLD、lead-off、BLE 和功耗测试。
- `data/processed/firmware_baseline.csv`：历史固件和当前原理图的 MCU/SPI/帧长/BLE 配置对照。
- `data/processed/altium_inventory_*.csv`：从三代 SchDoc OLE `FileHeader` 提取的器件/网络标签清单，不替代正式 netlist。

## 当前必须先处理的问题

- ADS1299 数据手册第 70-71 页给出的 VCAP1 典型值为 `100 uF`；第六代 SchDoc 库存清单观察到 10 uF/22 uF 候选值，但尚未由正式 netlist 证明哪一个连接到 VCAP1，须核对 BOM、网络和封装后再下板。
- ADS1299 需要 `AVDD-AVSS=4.75~5.25 V`，不能把 3.3 V XC6206 输出直接当作 AVDD；数字 `DVDD`、蓝牙电源和模拟 AVDD 必须分开核算。
- 三代芯片的 VCAP1、参考电压和 BIAS/RLD 命名不同，不能复制同一张“129x 通用”原理图。
- 现有固件 SPI 设置为 CPOL=0、CPHA=1（第二边沿），且预分频可能达到数十 MHz；必须按对应数据手册时序和示波器实测重新设定。
- 第七代 BG22A 使用 UART2，但现有代码只显式配置 CTS，RTS 接线和方向仍需按模块手册实物核对。
- 原理图中的模块标注出现 `BG22A3`，资料目录同时有 `BG22A1` 手册；BOM、丝印和手册型号必须统一。
- ADS1292 当前原理图实际出现 `RF-BM-4044B4`，必须取得 4044B4 专用数据手册后才能确定供电、UART、流控和天线规则。

## 建议的提交节奏

先按 `docs/07_test_plan_and_fault_isolation.md` 完成无电、上电、SPI 寄存器回读、内部测试信号、输入短接噪声、导联脱落、RLD/BIAS、UART/BLE 和功耗测试；每次测试把原始波形或 CSV 放入 `data/processed/`，并在对应 CSV 的 `evidence`、`status` 字段填写实测证据。`reference_netlist.csv` 是测试版目标连接，不是已签核的 Altium 原理图；未测项目保持 `OPEN`，不要用推测值替代。
