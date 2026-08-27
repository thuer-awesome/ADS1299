# ADS1292 DV1.7.8 独立参考设计

ADS1292 只有 2 个 ECG 通道，模拟电源/参考/时钟条件与 ADS1298/ADS1299 不同。当前工作区的 `DEMO.SchDoc`、`DEMO.PcbDoc`、`ADS1292 DV1.7.8.1.zip` 和截图只作为原始证据；在没有从 Altium 网络表导出并逐网核对前，不把截图推断成“已连接”。`data/processed/ads1292_schematic_inventory.csv` 记录了从 SchDoc OLE `FileHeader` 提取出的器件、网络标签和值，但它仍然不是正式 netlist。

## 芯片级基线

| 项目 | ADS1292 条件 | 依据 |
|---|---|---|
| AVDD/DVDD | 常用单极 AVDD=3 V、DVDD=1.8 V；允许单极或双极 | DS p.3-6、60-61 |
| 参考 | 内部 VREF 约 2.42 V；VREFP/VREFN 按内部参考图连接 | p.24-26、60 |
| 时钟 | 外部 512 kHz；CLKSEL=0；CLK_DIV=1 时可到约 2.048 MHz | p.10、27 |
| VCAP1/VCAP2 | 图 60/61 中各约 10 µF/1 µF；VCAP1 振动环境优先低微音钽/聚合物，C0G/NP0 仅用于小容量旁路 | p.60-61 |
| 输入 PGA | 增益 1/2/3/4/6/8/12；内部 PGA 输出有 4.7 nF CFILTER 图示 | p.21-23、60 |
| 数据帧 | 24-bit status + 2×24-bit = 9 byte | p.28-39 |
| PWDN/RESET | 共用一个低有效引脚；低于规定时间进入掉电/复位 | p.11-12、31-32 |

## 引脚和无源网络

PGA1N/PGA1P 与 PGA2N/PGA2P 位于 1/2/7/8 脚；RLDIN/RLDREF=29、RLDINV=28、RLDOUT=30；VCAP1=11、VCAP2=27；DIN/DOUT/SCLK/CS=19/21/20/18；DRDY=22；CLKSEL=14；PWDN/RESET=15；START=16（p.11-12）。

输入端采用低漏电保护和可更换串联电阻；跨 PGA 输出的 `4.7 nF` 是数据手册图示的 CFILTER，不是所有输入端都应并联的电容。RLD 网络按 p.24-25、50-56 的 RLD/lead-off 图实现，所有人体连接前必须先用等效电阻电容模型验证。

## 上电、初始化和 MCU

ADS1292 p.62-63 给出明确流程：所有数字/模拟输入先拉低；电源稳定后提供 CLK；等待 `tPOR=212 tMOD`；RESET 低至少 `1 tMOD`；发送 RESET 后等待 18 tCLK；发送 SDATAC；配置 CONFIG1、CONFIG2、CHnSET；START；RDATAC。内部参考模式先置 `PDB_REFBUF` 并等待稳定。

`CONFIG1`（p.40）决定数据率、单次/连续模式和 CLK_DIV；`CONFIG2`（p.41）决定内部参考、测试信号和校准；`LOFF`（p.42）决定导联脱落比较器阈值/电流；`CH1SET/CH2SET`（p.43-44）决定 PGA 增益、关断和输入 MUX；`RLD_SENS`（p.45）决定 RLD 平均输入；`LOFF_SENS` 与 `LOFF_STAT`（p.46-49）负责检测状态。ADS1292 没有 ADS1299 的 8 路 BIAS_SENSP/SENSN 寄存器，固件必须按两通道地址和位域实现。

当前 `DEMO.SchDoc` 的可读器件/网络字符串包含 `STM32L442KCU6`、`SPI1`、`SPI3` 和 `RF-BM-4044B4`；目录名却写有 STM32L432，且蓝牙模块也不是 BG22A。应以 PCB 装配、MCU/蓝牙顶标确认实际料号，再用对应数据手册核对 AF、UART 电平和复位时序，不能根据目录名或 BG22A 手册猜测。测试版应把 SPI SCK/MISO/MOSI、CS、DRDY、PWDN/RESET、START、CLKSEL 全部引出到测试点，并在固件启动日志打印芯片 ID、CONFIG1/2/3、CH1/2SET 和实际 SPI 时钟。

## 2 通道测试门禁

- ID 回读正确且寄存器写后回读一致。
- DRDY 周期与所选数据率一致，9-byte 帧无错位、无丢样。
- CH1/CH2 输入短接噪声按手册条件采集至少 1000 帧；记录均值、峰峰值、RMS 和频谱。
- CONFIG2 内部测试信号的幅度/频率与手册图表一致。
- RLD 和 lead-off 用 51 kΩ+47 nF 等效模型逐脚断开/接入，状态寄存器和 RLDOUT 波形能解释。
- 只有在以上数据、原始波形、仪器设置和板卡序列号均归档后，才把 `ads1292_dv1.7.8` 标记为通过。
