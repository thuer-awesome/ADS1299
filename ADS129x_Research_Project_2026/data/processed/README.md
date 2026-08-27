# 处理后数据

CSV 表是可审查的基线和记录模板。实验产出应使用以下命名：

`<board>_<chip>_<test-id>_<date>_<instrument>.<ext>`

例如：`DV4_ADS1299_NOISE-01_20260827_MDO34.csv`。不要覆盖原始文件；重复测试使用递增序号，并在表格 `evidence` 字段填入相对路径。每个波形文件旁边放置同名 `.json`，记录采样率、带宽、探头、寄存器快照和板卡序列号。

`altium_inventory_ads1292.csv`、`altium_inventory_ads1298.csv`、`altium_inventory_ads1299.csv` 分别是脚本从三代 SchDoc OLE `FileHeader` 生成的器件/网络标签清单（133、285、251 条）；它们不包含导线连通性，不可替代 Altium 导出的 netlist。

`firmware_baseline.csv` 汇总第六代 ADS1299、第七代 ADS1298、2022-06 ADS1292 历史固件，以及当前 DV1.7.8 原理图中能确认的 MCU/BLE/SPI 信息。
