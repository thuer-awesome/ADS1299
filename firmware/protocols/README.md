# Data Protocols

ADS1299/ADS1298 数据帧为 24-bit status + 8×24-bit 通道数据，共 27 byte；ADS1292 为 24-bit status + 2×24-bit，共 9 byte。BLE PACK、UART 波特率、CRC/序号和丢包统计必须结合对应模块手册和实测日志确认。

寄存器与帧长基线见 [`register_baseline.csv`](../../ADS129x_Research_Project_2026/data/processed/register_baseline.csv) 和 [`test_matrix.csv`](../../ADS129x_Research_Project_2026/data/processed/test_matrix.csv)。
