# 固件源码快照

`ads1299_dv4.0.2/`、`ads1298_dv2.4.1/` 和 `ads1292_dv1.7_history/` 是从本地工程复制的源码快照，保留 Keil/CMSIS/HAL 目录结构，排除了常见编译产物（`.hex`、`.axf`、`.map`、`.o`、`.obj`、`.d`、`.crf`、`.Peiki`、`Objects/`、`Listings/`）。

编译前请在对应工程中确认 MCU 料号、启动文件、时钟树、SPI AF、UART 流控和链接脚本；源码中的注释和默认寄存器数组不能代替实物回读。
