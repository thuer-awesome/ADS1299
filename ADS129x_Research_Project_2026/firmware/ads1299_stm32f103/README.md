# ADS1299 固件索引

原始代码目录包含 `ADS129x/ADS129x.H/.C`、`Hardware/spi.c`、`Hardware/usart.c`、`Hardware/BG22AX.c/.h`。当前基线为 8 通道、27 byte、SPI1、DRDY=PA0、RESET=PA4、CS=PB0、BG22 UART2。

代码设置 CPOL=0、CPHA=1、SPI 预分频 2（STM32F103 72 MHz 时约 36 MHz）；测试版必须先降速并用逻辑分析仪验证时序和寄存器回读。
