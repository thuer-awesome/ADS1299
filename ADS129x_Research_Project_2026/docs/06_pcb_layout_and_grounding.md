# PCB 布局、地和测试点

## 分区原则

建议从板边到芯片依次为“电池/充电和开关电源 → 数字电源与 MCU → BLE 模块及天线 → ADS129x 模拟电源/参考 → 电极连接器”。电极和 ADS129x 输入区域只允许低速、低漏电网络；SPI、SWD、晶振、PWM、LED 和开关节点不得穿过该区域。

ADS1299 第 72-73 页、ADS1298 第 98-99 页和 ADS1292 第 60-61 页都强调：模拟输入和 AVSS 是高阻敏感节点，走线短、直、对称，避免杂散电容；数字回流不能穿越模拟回流；电容同层贴近引脚；差分输入电容使用 C0G/NP0。

## 地和电源

- 将 `AGND/AVSS`、`DGND`、`BLE_GND`、`USB_GND` 和 `POWER_GND` 在原理图中分别命名，布局上形成明确回流路径。AGND/DGND 可以分区，但在 ADS129x 附近按数据手册单点汇接，不要让高速数字回流经过输入保护器件。
- ADS1299 的 AVDD1/AVSS1 由于 charge-pump 在 fCLK 有瞬态，必须从 AVDD/AVSS 星形引出；ADS1298/1292 也要把 AVDD 作为安静电源，开关电源远离 AFE。
- 每个供电脚先放 0.1 µF，再放 10 µF；VCAP、VREF 电容直接落在芯片同层，回路最短。VCAP1 使用低微音钽/聚合物或经实测合格的低噪声陶瓷，并在 BOM 记录介质、额定电压和直流偏置下有效容量；100 µF 级不应假定能用 C0G/NP0 实现。
- XC6206P33 是约 200 mA 级 3.3 V LDO，只能作为经过负载和热耗核算后的数字/BLE rail；它不能生成 ADS1299 所需 5 V AVDD。ADS1299 需要另一个低噪声 5 V 模拟 rail 或升压后再滤波。

## 模拟输入和 RLD/BIAS

- 每路输入保护器件漏电必须显著低于 ADS1299 约 300 pA 典型输入偏置电流（电气特性页），并验证保护器件在目标共模范围内的寄生电容。
- 差分电容优先放在 ADC 引脚之间；共模电容对称放置且值约为差分电容 1/10~1/20。任何 TVS/ESD 阵列要以漏电和电容为第一筛选条件，不能只按额定电压选型。
- BIASOUT/RLDOUT 走线远离输入，输出串联电阻和稳定性电容靠近驱动器；预留跳线以便在没有人体时使用电阻电容模型。

## BLE 天线和高速线

BG22A 手册要求天线附近和模块底部保持 keepout（建议大于 10 mm），不要铺铜或走高速数字/开关电源线；板载天线通过 0 Ω 选择，外接天线时断开该 0 Ω。SPI SCLK、CLK 和 SWD 线应短且有连续数字地回流，进入模拟区前用地隔离或改变走线层。

## 必须预留的测试点

电源：`TP_AVDD`、`TP_AVSS`、`TP_AVDD1`、`TP_AVSS1`、`TP_DVDD`、`TP_DGND`、`TP_VREFP`、`TP_VREFN`、`TP_VCAP1/2/3/4`；时序：`TP_CLK`、`TP_SCLK`、`TP_CS`、`TP_DIN`、`TP_DOUT`、`TP_DRDY`、`TP_RESET`、`TP_PWDN`、`TP_START`、`TP_CLKSEL`；模拟：每通道输入连接器前后、`TP_BIASOUT/RLDOUT`、lead-off 注入点；BLE：`TP_VCC`、`TP_TX`、`TP_RX`、`TP_CTS`、`TP_RTS`、`TP_RESET`、`TP_LINK`。

测试点不要做成长 stub；优先使用小环形焊盘或 1.27 mm 排针，并在 PCB 丝印标注地参考点和测量方向。
