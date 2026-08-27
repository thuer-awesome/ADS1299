# ADS1298 DV2.4.1 独立参考设计

ADS1298 是 8 通道 ECG/生物电 AFE，虽然数字接口与 ADS1299 相似，但模拟专用引脚是 RLDREF/RLDINV/RLDIN/RLDOUT/WCT，17/18 为 pace test 输出。第七代原理图必须按 ADS1298 引脚表重画审核，不能复用 ADS1299 的 BIAS 网络。

## 供电和内部电容

| 网络 | 建议值/连接 | 数据手册 |
|---|---|---|
| AVDD/AVSS | 2.7~5.25 V；选择 3 V 或 5 V 模式后，按对应 VREF 重新计算满量程 | p.12、96 |
| DVDD/DGND | 1.65~3.6 V；与 STM32L432 同为 3.3 V 时无需电平转换 | p.12 |
| VCAP1 | `22 µF` 典型，靠近引脚；振动环境优先低微音钽/聚合物，C0G/NP0 仅用于小容量旁路 | p.96-97 |
| VCAP2/VCAP3/VCAP4 | 约 1 µF；VCAP3 可再并 0.1 µF | p.96-97 |
| VREFP/VREFN | 按内部/外部 reference 模式布置，VREFN 回 AVSS，预留 TP | p.29、96 |
| 本地去耦 | 每路 supply 至少 10 µF+0.1 µF，0.1 µF 同层贴脚 | p.96-99 |

## 第七代实际 MCU 映射

固件 `ADS129x.H/.C` 的基线为：SPI3，PB3=SCK、PB4=MISO、PB5=MOSI；CS=PA15；RESET=PB6；DRDY=PA11/EXTI15_10；27-byte 帧；CONFIG1=0xC6（HR、外部时钟、500 SPS 注释）；CONFIG2=0x10；CONFIG3=0xCC；LOFF=0x02；CH1~CH8=0x00；LOFF_SENSP/SENSN=0xFF。实际上电后必须执行 SDATAC，再写寄存器并逐字节回读，不能只依赖 RAM 中的默认数组。

`CONFIG1`（p.69-70）负责 HR/LP、时钟和 DR；`CONFIG2`（p.69-70）负责测试信号；`CONFIG3`（p.69）负责内部参考和 RLD；`CHnSET`（p.71）负责 PD、GAIN、MUX；`RLD_SENSP/SENSN`（p.72-73）负责 RLD 输入平均；`LOFF_SENS/FLIP/STAT`（p.74-78）负责导联脱落；`PACE/RESP/WCT1/WCT2`（p.79-81）是 ADS1298 专用功能。任何把 ADS1299 的 BIAS_SENS 地址或位定义写进这些寄存器的代码都应拒绝合并。

## RLD、PACE、WCT

- RLDREF/RLDINV/RLDIN/RLDOUT 按 ADS1298 p.34-35、39-43 的 RLD 图布置；典型外部网络中的 220 kΩ、50 kΩ、20 kΩ 需标记为 TI-TYP。
- PACE 输出和 WCT 仅在 ECG/pace/Wilson central terminal 功能启用时连接；不用时按 p.10-11、85-87 的未使用引脚要求处理，不能悬空。
- Lead-off 采用 51 kΩ+47 nF 负载模型，分别验证 P/N 状态寄存器和 LOFF_FLIP；不要把 ADS1299 的 BIAS_SENSP/SENSN 地址或位定义写入 ADS1298 工程。

## 验证顺序

1. 无电检查 AVDD、AVSS、DVDD、DGND、RLD/WCT 和数字地无短路；SchDoc 库存清单显示 `C171=22 µF` 且位置接近 `1-VCAP1` 标签，仍需正式 netlist 确认其网络归属后再签核。
2. 外部 2.048 MHz 时钟、RESET、START、DRDY 用示波器同时观察；检查 DRDY 周期与 500 SPS/配置一致。
3. 读 ID=0x92；写/读 CONFIG1~CONFIG4、CHnSET、RLD_SENS、LOFF_SENS，记录寄存器差异。
4. 所有通道输入短接，按数据手册噪声测试条件采集至少 1000 帧；再启用内部测试波形，确认 27-byte 帧的 status 和 8×24-bit 数据对齐。
5. 连接 RLD 模型和 lead-off 模型，分别断开 P、N、电极参考，记录状态位和 RLDOUT 波形。
