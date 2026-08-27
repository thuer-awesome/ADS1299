# 工具与复现

建议把 Altium 导出的网络表（CSV）、BOM（CSV）、逻辑分析仪原始文件和示波器截图放入 `data/processed/`，原始 PDF/SchDoc/PcbDoc 保留在外部资料目录并在 README 中记录绝对路径和 SHA-256。

`extract_altium_inventory.py` 可在安装 `tools/requirements.txt` 后提取 SchDoc 的器件和网络标签，例如：

```text
python tools/extract_altium_inventory.py "E:\ADS1299\2022-09-26 ADS1292 DV1.7.8（调整）STM32L432+4404B 双通道\DEMO.SchDoc" -o data/processed/altium_inventory.csv
```

该脚本明确不会推断导线连通性；签核仍必须使用 Altium 导出的正式 netlist。

推荐的最小复现步骤：

1. 导出三块 PCB 的 netlist 和 BOM，按 `docs/01_datasheet_traceability.md` 检查器件/网络/页码。
2. 用 `register_baseline.csv` 生成固件上电配置和回读日志。
3. 用 `test_matrix.csv` 为每个测试分配板卡序列号、仪器和原始文件名。
4. 用脚本计算帧计数、DRDY 周期、RMS/FFT 和 BLE 丢包率；脚本输出不得覆盖原始数据。
