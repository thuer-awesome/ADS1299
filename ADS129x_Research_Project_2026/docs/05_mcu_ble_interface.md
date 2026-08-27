# MCU、SPI、UART 与 BLE 接口追溯

## STM32 与 ADS129x

| 版本 | MCU 工程证据 | ADS129x 信号映射 | 需要核实的点 |
|---|---|---|---|
| ADS1299 DV4.0.2 | STM32F103T8 固件 `Hardware/spi.c`、`ADS129x.H` | SPI1: PA5=SCLK、PA6=MISO、PA7=MOSI；CS=PB0；DRDY=PA0/EXTI0；RESET=PA4；8 通道 27 byte | 现代码 CPOL=0/CPHA=1、预分频 2 在 72 MHz 下约 36 MHz，先降至 1~4 MHz 验证，再按 ADS1299 SPI 时序提高；PWDN、START、CLKSEL 需核对原理图默认电平 |
| ADS1298 DV2.4.1 | STM32L432 HAL 固件 `ADS129x.H/.C`、`Hardware/spi.c`；主体 SchDoc 元件记录为 STM32L442KCU6 | SPI3: PB3=SCK、PB4=MISO、PB5=MOSI；CS=PA15；DRDY=PA11/EXTI15_10；RESET=PB6；8 通道 27 byte | `SPI3` AF6 与实际装配 L432/L442 封装复核；代码初始化时禁用 DRDY IRQ，需确认何处重新使能；CONFIG1 注释与实际数据率一致性需测量 |
| ADS1292 DV1.7.8 | 当前工作区 `DEMO.*`；SchDoc 字符串可读为 STM32L442KCU6 + RF-BM-4044B4 | 双通道 ADS1292/1292R 符号，SPI1/SPI3 标签同时存在 | 以 PCB 装配和芯片顶标确认 L442/L432、ADS1292/1292R、4044B4 型号；再从 Altium 网络表确认 SPI/CS/DRDY/START/CLKSEL；PWDN/RESET 是复用脚 |
| ADS1292 DV1.7 历史固件 | 2022-06-17 STM32L432 1.1.6 | SPI3 PB3/PB4/PB5；CS=PB6；DRDY=PA15；BG22 UART2 PA2/PA3；LINK=PA10；RESET=PA11；9 byte | 这是可复现的旧固件映射，不能假定与 DV1.7.8 SchDoc 相同；需做版本差异测试 |

STM32L442KC 数据手册第 52-57、139-141 页可用来核对 L4 系列 AF 复用与 SPI 时序，但第七代固件实际标为 STM32L432。最终生产 BOM 必须以实际 L432 料号数据手册和封装页为准，不能仅凭 L442 手册替代。

## BG22A1/BG22A3 模块

资料中的 RF-BM-BG22A1 硬件规格书第 7 页定义：VCC=3.3 V 推荐（1.71~3.8 V），RESET 低有效，PA03=CTS（模块输入），PA04=RTS（模块输出），PA05=TX，PA06=RX；协议手册第 12、46-47、68-70 页给出 UART、PHY、PACK 和流控要求。模块电源就近放置 2.2 µF + 0.1 µF（协议手册第 69-70 页），天线 keepout 至少 10 mm（硬件规格书第 9-10 页）。

### 第六代实际工程

- `BG22AX.h/.c`：LINK=PBin(7)、RESET=PAout(1)、RTS 读取 PB4（但代码注释掉 RTS 初始化），UART2=PA2/PA3，无硬件流控。
- AT 配置：460800 baud、PHY=2、PACK=195/1（匿名协议）、关闭看门狗；这些是项目配置，不等于所有模块默认值。
- 必须实物核对模块丝印是 BG22A3 还是 BG22A1。BG22A3 若与 A1 引脚或协议不同，应取得对应数据手册。

### 第七代实际工程

- `BG22AX.h/.c`：LINK=PA12 输入、RESET=PB7 输出、UART2=PA2/PA3；代码使用 460800 baud、PHY=2（2M）、PACK=266/1、连接间隔 16。
- `usart.c` 的 UART2 仅配置 CTS（PA0，AF7），`HwFlowCtl=UART_HWCONTROL_CTS`；L4 系列通常可用 PA1/AF7 作为 RTS，但必须以实际 STM32L432 封装 AF 表和原理图为准。若模块 PA04/RTS 已接出，必须同时配置 MCU RTS 输出并确认常态高、低有效时序；若未接出，应关闭 CTS/RTS 硬件流控并通过吞吐/丢包测试证明。
- BG22AX 的 UART 速率支持范围和高于 512000 baud 的吞吐条件见协议 PDF 第 46 页。460800 是工程选择，需要用逻辑分析仪统计丢包率和有效吞吐，不得只看 AT 命令返回 OK。

### ADS1292 当前工程的蓝牙器件

`DEMO.SchDoc` 的元件记录显示 `RF-BM-4044B4`，并不是六、七代资料中的 BG22A1/BG22A3。当前只能确认有 `TX/RX/RTS/CTS/LINK/EN/RST` 网络标签，不能把 BG22A 的 1.71~3.8 V、2.2 µF+0.1 µF 去耦和 AT 指令直接套用到 4044B4。必须取得 4044B4 硬件规格书，核对供电范围、UART 电平、流控极性、天线 keepout 和复位时序后，才能更新 BOM 和测试阈值。

## UART/BLE 测试

1. 断开 ADS129x 数据流，使用 USB-UART 发送 `AT+VERSION`、`AT+MAC?`、`AT+PHY?`、`AT+PACK?`，保存完整响应和模块丝印照片。
2. 示波器检查 VCC 在 TX 峰值时的纹波；若低于规格或复位，先增加就近 0.1 µF/2.2 µF 或改善电源阻抗。
3. 分别启用/关闭 CTS/RTS，发送固定长度递增帧，记录 UART 原始字节、BLE 接收字节、CRC/序号错误和重传次数。
4. 用 27-byte ADS1299/ADS1298 和 9-byte ADS1292 帧做端到端吞吐测试，确认 PACK 帧长不会把 ADC 帧截断或拼接。
5. 天线区域按硬件规格书检查 keepout、地平面和外接天线 0 Ω 选择；不要在模块底部穿越 SPI、时钟或开关电源线。
