# ADS1299 DV4.0.2 测试版参考设计

## 目标

先做一块只用于电气和数据链路验证的 ADS1299 8 通道测试板，再恢复电池、充电、外壳和低功耗功能。测试板必须能独立给 AVDD=5 V、DVDD=3.3 V、BLE=3.3 V 供电，并能断开 BIAS、lead-off 和人体电极。

## 芯片功能与引脚

| ADS1299 引脚/网络 | 功能 | 测试板连接 | 依据 |
|---|---|---|---|
| IN1P/IN1N … IN8P/IN8N | 8 路差分 PGA 输入 | 每通道串联可更换保护电阻；预留 INP-INN 差分 C、对 AVSS 的共模 C、外部短接跳线 | DS p.6-7、20-23、68-69 图 75 |
| SRB1/SRB2 | 公共参考路由 | 用 0 Ω/跳线选择；默认不把 SRB1 与所有输入硬并联 | DS p.20、59、66-67 |
| BIASIN/BIASINV/BIASOUT/BIASREF | BIAS/RLD 驱动 | BIASOUT 到电极经 1 MΩ + 1.5 nF 典型网络；每路 sense 约 220 kΩ；预留断开和示波器 TP | DS p.28-29、33-37、63 |
| VREFP/VREFN | 内部参考 | VREFP-VREFN 至少 10 µF；VREFN 直接 AVSS；TP_REF | DS p.24、70-71 |
| VCAP1 | 内部电荷泵/参考节点 | `100 µF` 级低微音钽/聚合物优先；紧贴引脚，TP_VCAP1；C0G/NP0 仅适合小容量旁路 | DS p.70-71；SchDoc 清单有 10/22 µF 候选但网络未确认，列为 P0 风险 |
| VCAP2/3/4 | 内部旁路 | 1 µF；VCAP3 再并 0.1 µF；不接任何外部负载 | DS p.70-71 |
| AVDD/AVDD1 | 模拟/电荷泵供电 | 5 V 安静模拟 rail；AVDD1 从 AVDD 星形引出，避免数字回流；每个脚 10 µF+0.1 µF | DS p.8、70-73 |
| AVSS/AVSS1 | 模拟回流 | AVSS1 与 AVSS 星形连接；在 ADC 附近与系统 AGND 单点汇接 | DS p.70-73 |
| DVDD/DGND | 数字供电/地 | 3.3 V、0.1 µF+10 µF；DGND 回流不穿过输入区 | DS p.8、70-73 |
| CLK/CLKSEL | 主时钟 | 测试板默认外部 2.048 MHz，CLKSEL=0；提供可测 TP_CLK 和可切换内部振荡器 | DS p.8、27 |
| DIN/DOUT/SCLK/CS | SPI | 连接 STM32F103 SPI1：PA7/PA6/PA5，CS=PB0；串 22~47 Ω 可选阻尼位 | DS p.6-7、38-43 |
| DRDY/RESET/PWDN/START | 转换控制 | DRDY=PA0/EXTI0；RESET=PA4；PWDN 和 START 必须有明确上拉/下拉及 TP；默认不悬空 | DS p.6-7、34-37、61-62 |
| DAISY_IN/RESV1/GPIO | 未使用数字脚 | DAISY_IN、RESV1、未用 GPIO 接 DGND；GPIO 使用时必须配置方向 | DS p.6-7、27、61 |

## 推荐首版寄存器基线

首版先使用 `data/processed/register_baseline.csv` 中 ADS1299 行的安全配置：HR、250 SPS、内部 4.5 V reference、所有通道输入短接、BIAS 和 lead-off 关闭。完成寄存器回读和噪声测试后，按测试顺序切换内部测试信号、单通道输入、BIAS 和 lead-off。`CHnSET=0x60`（Gain=24）只用于项目原固件兼容性，不作为首个噪声基线；Gain=1 或 12 更容易避免输入/测试信号饱和。

### 主芯片寄存器到引脚的关系

- `CONFIG1`（p.46）：`DAISY_EN` 决定菊花链，`CLK_EN` 决定是否把内部振荡器送到 CLK，`DR[2:0]` 决定数据率；CLKSEL=0 时必须提供外部 2.048 MHz。
- `CONFIG2`（p.47）：`INT_TEST`、测试频率和幅度只在内部测试阶段打开，完成测试后恢复正常输入。
- `CONFIG3`（p.48）：`PD_REFBUF`、`BIASREF_INT`、`PD_BIAS`、`BIAS_MEAS` 和 `BIAS_LOFF_SENS` 直接决定 VREF/BIASREF/BIASOUT 是否有电；先测电源和参考，再打开 BIAS。
- `CHnSET`（p.50）：`PD` 关断通道，`GAIN[2:0]` 设 PGA，`SRB2` 选择公共参考，`MUX[2:0]` 选择正常输入、短接、测试、电源或温度。输入短接码为 `001`，内部测试码为 `101`。
- `BIAS_SENSP/SENSN`（p.51-52）选择参与 BIAS 平均的 P/N 输入；`LOFF_SENSP/SENSN`（p.53-54）选择 lead-off 激励/检测；`GPIO`（p.58）必须把未用脚配置为明确方向并接 DGND；`MISC1.SRB1`（p.59）只在公共参考拓扑需要时置位。

### 一次 SPI 事务的最小时序

上电后保持 CS 高、SCLK 低；RESET 完成后拉低 CS，发送 `SDATAC=0x11`，再发送 `WREG=0x40|addr`、`len-1` 和数据；读回使用 `RREG=0x20|addr`、`len-1`。每次事务 CS 全程保持低，CS 拉高后至少等待 4 tCLK。开始采集时发送 `START=0x08`，再发送 `RDATAC=0x10`；每个 DRDY 下降沿读取 27 byte。停止或改寄存器前发送 `SDATAC`/`STOP`，避免 RDATAC 模式下把寄存器命令当作数据。

## 前端阻容选型

- 输入保护优先使用低漏电、低噪声器件；串联电阻从 1 kΩ 起评估，不得让源阻抗与输入偏置电流产生不可接受的 DC 偏移。
- 跨 INP/INN 的差分电容优先 C0G/NP0；共模电容按差分电容的 1/10~1/20 起步。4.99 kΩ、4.7 nF、952 kΩ、10.3 kΩ 等只作为 TI EEG 典型图的候选值（p.68-69），最终由目标带宽、噪声和输入保护要求决定。
- BIAS 输出的 1 MΩ/1.5 nF、通道 sense 的 220 kΩ 是 TI 典型网络（p.33-37），应保留 0 Ω 或焊盘以便更换，不得在未测量稳定性时直接封死。
- VCAP1 禁止用高压 X7R/X5R 作为唯一器件；振动测试中压电噪声会调制参考。优先使用额定电压足够的低微音钽/聚合物；若采用陶瓷替代，必须用目标偏置和机械激励实测有效容量、噪声与稳定性，并在 BOM 中记录 DC bias 条件。

## 必测点

`TP_AVDD`、`TP_AVSS`、`TP_DVDD`、`TP_DGND`、`TP_AVDD1`、`TP_AVSS1`、`TP_VREFP`、`TP_VREFN`、`TP_VCAP1`、`TP_VCAP2`、`TP_VCAP3`、`TP_VCAP4`、`TP_CLK`、`TP_DRDY`、`TP_SCLK`、`TP_CS`、`TP_DIN`、`TP_DOUT`、`TP_RESET`、`TP_PWDN`、`TP_START`、`TP_BIASOUT`。测试点必须能用探头接触而不需要拆下芯片或 BLE 模块。

## 版本门禁

只有同时满足以下条件，才能把测试板标记为“电气基线通过”：所有电源在负载阶跃时无越界；上电顺序和 RESET/VCAP1 达标；ID 和全部可写寄存器回读一致；216-bit 帧无丢样；输入短接噪声按 ADS1299 p.16-17 的条件采集至少 1000 个连续样本；内部测试信号频率/幅度与 CONFIG2/数据手册一致；BIAS/lead-off 在断开和接入 51 kΩ+47 nF 模型时状态可解释。任何一项缺证据都保持 `OPEN`。
