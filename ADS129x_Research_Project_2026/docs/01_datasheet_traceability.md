# 数据手册逐页追溯

本文中的页码按随项目保存的 PDF 页码（封面计为第 1 页）记录。若供应商重新发布修订版，必须重新核对页码和图号，并在变更日志登记。

## 1. 供电、参考和内部节点

| 项目 | ADS1299 | ADS1298 | ADS1292 | 设计结论 |
|---|---|---|---|---|
| 模拟电源 | AVDD-AVSS `4.75~5.25 V`，典型 5 V（DS-REQ，p.8） | `2.7~5.25 V`（DS-REQ，p.12） | 单极典型 AVDD 3 V，DVDD 1.8 V（DS-REQ/TI-TYP，p.60） | 三代不能共用同一 AVDD rail；ADS1299 必须单独生成约 5 V 模拟电源 |
| 数字电源 | DVDD `1.8~3.6 V`，典型 3.3 V（p.8） | DVDD `1.65~3.6 V`（p.12） | DVDD 典型 1.8 V（p.60） | MCU 电平须落在对应 DVDD 范围内 |
| 参考 | 内部 4.5 V，VREFN=AVSS，VREFP=AVSS+4.5 V（p.8、24） | 3 V 供电约 2.5 V、5 V 供电约 4 V（p.12、29） | VREF 约 2.42 V，外部参考可选（p.24-26） | 在寄存器和换算公式中使用实际 VREF，不要套用 ADS1299 的 4.5 V |
| VCAP1 | `100 uF` 典型；振动环境优先低微音钽/聚合物（p.70-71） | `22 uF` 典型（p.96-97） | `10 uF` 图示；振动环境优先低微音钽/聚合物（p.60-61） | VCAP1 是最高优先级 BOM 审核项，三代值不同；C0G/NP0 仅适合小容量旁路 |
| VCAP2/3/4 | VCAP2=1 uF，VCAP3=1 uF+0.1 uF，VCAP4=1 uF（p.70-71） | 约 1 uF（p.96-97） | VCAP2=1 uF（p.60-61） | 贴近引脚，先按各自图纸布置 |
| 参考去耦 | VREFP 至少 10 uF 到 VREFN（p.70-71） | 见典型电源图，按实际 VREF 模式配置（p.96） | 10 uF + 1 uF 图示（p.60） | 参考电容使用低漏电、低介质吸收器件并预留测试点 |
| 电源去耦 | 每个 device supply `10 uF + 0.1 uF` 固态陶瓷（p.70） | 同类本地去耦（p.96-97） | AVDD/DVDD 均 `10 uF + 0.1 uF`（p.60） | 0.1 uF 放在最靠近电源脚的位置，10 uF 放在同一电源岛内 |

## 2. 关键引脚和未使用引脚

ADS1299 64-pin TQFP 引脚总图和 Pin Functions 在第 5-7 页。模拟输入顺序为 IN8N/IN8P=1/2、IN7N/IN7P=3/4、…、IN1N/IN1P=15/16；SRB1=17、SRB2=18；VREFP/VREFN=24/25；VCAP4=26、VCAP1=28、VCAP2=30、VCAP3=55；BIASREF/BIASINV/BIASIN/BIASOUT=60/61/62/63；数字接口 DIN/PWDN/RESET/CLK/START/CS/SCLK/DAISY_IN/DOUT/DRDY=34/35/36/37/38/39/40/41/43/47。RESV1=31 接 DGND，未用 DAISY_IN 接 DGND，未用 GPIO 接 DGND，未用模拟输入接 AVDD（p.6-7、61）。

ADS1298 TQFP 的数字引脚编号与 ADS1299 相近，但 17/18 为 TESTP_PACE_OUT1/2，60/61/62/63 为 RLDREF/RLDINV/RLDIN/RLDOUT，64 为 WCT（p.10-11）。不可把 ADS1299 的 BIAS 网络直接接到 ADS1298 的 RLD/WCT 引脚。

ADS1292 的引脚表在第 11-12 页：PGA1N/PGA1P=1/2、PGA2N/PGA2P=7/8、RLDIN/RLDREF=29、RLDINV=28、RLDOUT=30、VCAP1=11、VCAP2=27、DIN/DOUT/SCLK/CS=19/21/20/18、DRDY=22、CLKSEL=14、PWDN/RESET=15、START=16（p.11-12）。ADS1292 将 PWDN 与 RESET 复用为单脚，不能照搬 ADS1298/1299 的独立 PWDN 和 RESET。

## 3. 输入、PGA、RLD/BIAS 与无源网络

| 功能 | 数据手册定位 | 选型和测试要求 |
|---|---|---|
| 输入多路复用器 | ADS1299 p.20-22；ADS1298 p.25-29；ADS1292 p.17-20 | 首次上电把所有 CHnSET 置输入短接；再切内部测试信号。电源/温度/RLD 测量模式要把 PGA 置低增益防止饱和。 |
| PGA | ADS1299 p.23（增益 1/2/4/6/8/12/24，表 5）；ADS1298 p.27-29；ADS1292 p.21-23 | 先 Gain=1 或 6 做范围和噪声基线，再逐档验证增益。PGA 输出离 AVSS/AVDD 至少约 200 mV（ADS1299 p.23）。 |
| 差分 RC | ADS1299 p.68-69 图 75；ADS1298 p.33；ADS1292 p.21-22、图 23 | 优先在 INP/INN 之间放差分电容；共模电容约为差分电容的 1/10~1/20。4.99 kOhm、4.7 nF 等是 TI 典型应用值，不是通用硬性值。 |
| BIAS/右腿驱动 | ADS1299 p.28-29、33-37；ADS1298 p.34-35、39-43；ADS1292 p.24-25、50-56 | 典型网络含 220 kOhm 通道电阻、1 MOhm/1.5 nF 输出网络；ADS1298/1292 使用 RLD 命名和不同内部结构。所有阻容值要在原理图中标注来源等级。 |
| Lead-off | ADS1299 p.30-32；ADS1298 p.39-43；ADS1292 p.50-56 | 用 51 kOhm + 47 nF 皮肤模型和手册给出的电流/阈值，分别测试 P/N、AC/DC 和状态寄存器。 |

## 4. 时钟、SPI 和数据帧

| 项目 | ADS1299 | ADS1298 | ADS1292 |
|---|---|---|---|
| 外部 CLK | 约 2.048 MHz，CLKSEL=0（p.8、27） | 约 2.048 MHz（p.12、48） | 512 kHz；CLK_DIV 可到 2.048 MHz（p.27） |
| SPI 采样边沿 | DIN 在 SCLK 下降沿采样，DOUT 在上升沿推出；CPOL=0、CPHA=1 等效时序（p.38-43） | 同类 SPI 时序（p.51-63） | PDF 明确注释 CPOL=0、CPHA=1（p.10、28-39） |
| 帧长度 | 24-bit status + 8×24-bit = 216 bit = 27 byte（p.38-43） | 27 byte（8 通道，p.51-63） | 24-bit status + 2×24-bit = 9 byte（p.28-39） |
| CS | 一个事务全程保持低；拉高后至少等待 4 tCLK（ADS1299 p.38-43） | 同类要求（p.51-63） | CS 事务要求见 p.28-39 |
| DRDY/START | DRDY 低有效；START/STOP 时序见表 7/8（p.34-37） | p.51-63 | p.31-35，START 低时停止转换 |

## 5. 上电和 PCB

ADS1299 的推荐顺序在第 61-62、70 页：数字/模拟输入先低；电源稳定；CLK 有效；等待 `tPOR=218 tCLK` 且 VCAP1 达到要求；RESET 低至少 2 tCLK；写寄存器；START；RDATAC。ADS1298 的对应流程在第 85-87 页；ADS1292 在第 62-63 页（`tPOR=212 tMOD`，RESET 低宽度至少 1 tMOD）。

PCB 约束见 ADS1299 第 72-73 页图 80、ADS1298 第 98-99 页、ADS1292 第 60-61 页：模拟和数字区域分区；高阻输入短且有护环/屏蔽；数字回流不得穿越模拟回路；AGND/DGND 可分区但在 ADC 处单点汇接；旁路电容同层靠近引脚，芯片和电容之间不插过孔；差分输入电容选 C0G/NP0；BLE 天线保持至少 10 mm keepout，并在模块下方避免高速线和电源线。

## 6. 页码审查规则

任何 BOM 或原理图审查意见至少包含“器件、网络、数值、来源等级、数据手册页码/图号、实测方法、通过阈值、证据文件名”。页码只支持“为什么这样设计”，不等于已经完成样板验证。
