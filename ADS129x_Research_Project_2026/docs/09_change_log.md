# 整理记录

## 2026-08-27

- 建立 `ADS129x_Research_Project_2026` 目录，保留原始三代工程文件不变。
- 从 ADS1299、ADS1298、ADS1292 数据手册和 BG22A/STM32 资料提取供电、VCAP、时序、SPI、RLD/BIAS、lead-off、布局和 UART 约束，并在 `01_datasheet_traceability.md` 标出页码。
- 纳入第六代 ADS1299 固件实际映射：SPI1 PA5/PA6/PA7、CS PB0、DRDY PA0、RESET PA4、BG22 LINK PB7、RESET PA1、UART2 PA2/PA3。
- 纳入第七代 ADS1298 固件实际映射：SPI3 PB3/PB4/PB5、CS PA15、DRDY PA11、RESET PB6、BG22 LINK PA12、RESET PB7、UART2 PA2/PA3、CTS PA0。
- 新增 ADS1292 独立参考设计，明确其 2 通道 9-byte 帧、PWDN/RESET 复用、512 kHz 时钟和 VCAP1 与其他两代不同。
- 解码 ADS1292 工作区 `修改内容.txt`（GBK）：去掉蓝牙天线、旋转芯片方向、调整焊盘位置；这些修改尚未完成电气/射频复核。
- 解析 `DEMO.SchDoc` OLE `FileHeader`：确认符号 `ADS1292-QFN`（comment=ADS1292R）、`STM32L442KxU6`（comment=STM32L442KCU6）、`RF-BM-4044B4`，以及 `ADS129x_DIN/DOUT/SCLK/CS/DRDY`、`SPI3-*`、`4404B4-*` 网络标签；元件值包含 0.1u、1u、1.5n、4.7n、1M、100K、22.1K、10K、3K、4.7K 等，已写入 `ads1292_schematic_inventory.csv`。
- 使用 `tools/extract_altium_inventory.py` 生成 `data/processed/altium_inventory_ads1292.csv`（133 条记录）。该文件仅是 OLE 字符串清单，`connectivity_status` 明确标记为未重建，正式签核仍需 Altium netlist。
- 对第六代/第七代主体 SchDoc 运行同一工具：分别生成 `altium_inventory_ads1299.csv`（251 条）和 `altium_inventory_ads1298.csv`（285 条）。第七代主体同样显示 STM32L442KCU6 和 RF-BM-BG22A3；第六代显示 RF-BM-BG22A（comment=BG22A3）。

## 未决事项

- 需要从 Altium 原理图导出正式网络表，逐网确认 PWDN、START、CLKSEL、DAISY_IN、RESV1、RLD/BIAS 和 AGND/DGND 的实际连接。
- 需要核对 ADS1299 VCAP1 是否确为 100 µF，有无只在截图中显示 10 µF 的旧版本。
- 需要确认第七代工程实际芯片是 STM32L432 的哪一个封装，并用该料号数据手册复核 AF 复用。
- 需要确认蓝牙模块丝印/BOM 是 BG22A1 还是 BG22A3，并取得对应硬件规格书。
- ADS1292 原理图实际显示 `RF-BM-4044B4`；在取得 4044B4 数据手册前，不能把 BG22A1/A3 的电源、UART 和天线规则套用到该版本。
- 需要补录所有实验原始 CSV、示波器截图、逻辑分析仪文件、功耗曲线和 PCB 层叠/阻抗信息。
