# ADS1292 固件索引

当前 DV1.7.8 工作区仅发现 PCB/原理图和 `ADS1292 DV1.7.8.1.zip`，没有可直接确认的完整源码目录。另有可追溯的 2022-06-17 DV1.7 固件目录：`E:\ADS1299\2022-06-17 STM32L432 1.1.6（发货固件）优化蓝牙，降功耗 配套 ADS1292 DV1.7`。

该固件明确设置 `ADS129X_DEVICE=ADS1292`、2 通道、SPI3、CS=PB6、DRDY=PA15/EXTI15_10、9-byte 帧；SPI3 使用 PB3/PB4/PB5、CPOL=0/CPHA=1，SPI1 预分频 8 作为另一配置，BG22 UART2=PA2/PA3 且未启用硬件流控，蓝牙 LINK=PA10、RESET=PA11。PWDN/RESET 仍由 ADS1292 单脚控制，需结合原理图核对。
