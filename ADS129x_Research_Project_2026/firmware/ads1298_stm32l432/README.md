# ADS1298 固件索引

原始代码目录包含 `ADS129x/ADS129x.H/.C`、`Hardware/spi.c`、`Hardware/usart.c`、`Hardware/BG22AX.c/.h`。当前基线为 SPI3 PB3/PB4/PB5、DRDY=PA11、CS=PA15、RESET=PB6、8 通道 27 byte；BG22 UART2=PA2/PA3，CTS=PA0，RTS 尚未得到完整证据。主体原理图元件记录是 STM32L442KCU6，代码工程说明为 STM32L432，必须核对实际装配型号后再锁定 AF 表。

代码默认寄存器数组必须在上电后通过 SDATAC/WREG/RREG 流程验证，不能仅凭 RAM 数组判定芯片已配置。
