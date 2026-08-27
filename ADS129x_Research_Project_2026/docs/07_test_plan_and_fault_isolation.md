# 测试计划与故障定位

每个测试记录板卡序列号、芯片批次、固件 Git 版本、仪器型号/带宽、探头接地方式、寄存器快照、原始 CSV 和结论。`PASS` 只表示该项在记录条件下通过，不能外推到人体或量产。

## A. 无电检查

1. 用万用表确认 AVDD-AVSS、DVDD-DGND、BLE_VCC-GND 无短路；逐个测量 VCAP1/2/3/4、VREFP/VREFN、RLD/BIAS 输出对地阻抗。
2. 核对 ADS129x 未用脚处理：ADS1299 的 RESV1、DAISY_IN、GPIO；ADS1298 的 PACE/WCT；ADS1292 的复用 PWDN/RESET。任何悬空脚均记为 `OPEN`。
3. 核对电容封装、介质和额定电压，特别是 ADS1299 VCAP1=100 µF、ADS1298 VCAP1=22 µF、ADS1292 VCAP1≈10 µF。

## B. 上电和时钟

1. 用限流电源分别给模拟、数字、BLE rail 供电，记录启动电流、稳态电流和 TX 峰值电流。
2. 示波器同时观察 AVDD/DVDD/VCAP1/RESET/CLK/DRDY；按 ADS1299 p.61-62、ADS1298 p.85-87、ADS1292 p.62-63 的 tPOR 和 RESET 脉宽执行。
3. 检查 AVDD 电压处于器件范围，若 ADS1299 仅有 3.3 V，直接判定供电架构不合格，不进入数据测试。

## C. SPI 和寄存器

1. CS 在整个命令/数据事务保持低；SCLK 空闲低、MSB first；用逻辑分析仪确认 DIN 在下降沿采样、DOUT 在上升沿推出。
2. 上电先发 RESET，再发 SDATAC；写入 `register_baseline.csv` 的寄存器，逐字节 RREG 回读。ID 不匹配、只读位变化或写后回读不一致时，先查 CS、CPHA、时钟和电平。
3. 记录 SCLK 频率。ADS1299 第六代先从 1~4 MHz 开始，确认无误码后逐步提高；不要直接使用旧固件可能产生的 36 MHz。
4. 帧长度检查：ADS1299/1298 每次 DRDY 读取 27 byte；ADS1292 读取 9 byte。统计 10 分钟内帧计数、序号、CRC、DRDY 间隔和 DMA/ISR 超时。

## D. 内部测试信号和输入短接

1. 将所有 CHnSET 设为输入短接（ADS1299 mux=001；ADS1298/1292 对应输入短接码），按器件数据手册噪声条件采集至少 1000 个连续样本。
2. 计算每通道均值、RMS、峰峰值、95% 分位、FFT 50/60 Hz 峰值和通道间相关性；结果与数据手册噪声表比较时，必须注明数据率、增益、参考和带宽完全一致，否则只做相对比较。
3. 切换内部测试信号（ADS1299 CHnSET mux=101；ADS1298/1292 使用各自 CONFIG2/CHnSET），测量码值、频率和幅度。若所有通道同时错位，优先查 SPI 帧对齐；若单通道异常，查该通道 RC/保护和 CHnSET。

### 码值换算和结果记录

对 24-bit 二补码原始码 `code`，先执行符号扩展，再按 `V = code × VREF / (GAIN × 2^23)` 换算输入电压；`VREF` 必须填写实际器件/供电模式值（ADS1299 内部参考约 4.5 V，ADS1298 随 3 V/5 V 模式变化，ADS1292 典型约 2.42 V）。报告中同时保留原始码、换算值和增益，避免把不同参考电压下的 RMS 直接比较。

## E. RLD/BIAS 与 Lead-off

1. 用 51 kΩ+47 nF 等效皮肤模型逐通道连接，先关闭 lead-off，再依次开启 P、N、AC/DC 激励；记录 LOFF_STAT、RLD/BIAS 状态和电流。
2. 断开 P、N、参考和 RLD 电极，确认状态位只在对应脚变化。状态位全为故障通常指向参考/地断路；全部正常但电极开路通常指向 sense 位或激励路径错误。
3. BIAS/RLD 驱动器先接纯电阻模型，观察 BIASOUT/RLDOUT 是否振荡；再接 1.5 nF 稳定性电容。任何振铃都要记录示波器带宽和探头电容，不得用软件滤波掩盖。

## F. BLE 和功耗

1. AT 模式读取版本、MAC、PHY、PACK、UART，保存完整响应；确认实际模块是 BG22A1 还是 BG22A3。
2. 在 115200、460800、921600（若模块支持）分别发送递增序号帧，统计有效吞吐、丢包、重传、CTS/RTS 翻转和 ADC 数据时间戳漂移。
3. 连接/断开、休眠/唤醒、BLE TX 峰值和充电切换时记录 AVDD、VCAP1、DRDY 是否受扰。若蓝牙动作导致 EEG 噪声突增，先查地回流、天线 keepout 和电源去耦。

在任何人体电极连接前，先用假负载测量 lead-off/RLD 的最大电压和电流、输入到保护地的漏电、充电状态下的隔离和 ESD 路径；这些项目没有实测记录时，只允许使用电阻电容模型。

## 故障定位速查

| 现象 | 优先检查 | 证据 |
|---|---|---|
| ID=0x00/0xFF | AVDD/DVDD、RESET/PWDN、CS、SPI 电平和 CPHA | TP 电源/RESET 波形、逻辑分析仪 |
| DRDY 不跳 | CLK/CLKSEL、START、CONFIG1、tPOR/RESET | TP_CLK/START/DRDY、寄存器快照 |
| 帧长正确但码值全零 | CHnSET PD 位、输入短接/参考、PGA 饱和 | CHnSET 回读、输入端电压 |
| 所有通道 50/60 Hz | 地回流、AVDD 纹波、输入屏蔽、RLD 未闭环 | 频谱、电源纹波、布局照片 |
| 单通道高噪声 | 该通道保护/RC/焊点、输入漏电、sense 位 | 通道互换实验、显微照片 |
| BIAS/RLD 振荡 | 输出 RC、负载模型、走线耦合、探头电容 | BIASOUT/RLDOUT 波形 |
| BLE 丢包 | CTS/RTS 方向、UART 时钟、PACK、天线和 VCC 去耦 | UART/BLE 双端原始记录 |
