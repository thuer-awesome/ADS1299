# 原始硬件文件快照

这些文件是从本地工程复制的用户自有设计文件，未对二进制内容做修改。PDF 数据手册仍只保留外部路径和哈希，避免重新分发第三方文档。

| 目录 | 来源和内容 |
|---|---|
| `ads1299_dv4.0.2/` | 第六代 `mode.SchDoc`、库文件和预览文件；原始资料未发现同目录 `PcbDoc` |
| `ads1298_dv2.4.1/` | 第七代主体 `DEMO.SchDoc`、`DEMO.PcbDoc`、`DEMO.PrjPCB` 和工程 ZIP |
| `ads1292_dv1.7.8/` | 当前工作区 `DEMO.SchDoc`、`DEMO.PcbDoc`、`DEMO.PrjPCB`、ZIP、修改记录 |
| `ads1292_dv1.7.4/` | 历史双通道样板原理图、PCB、项目和 ZIP |

导入后必须在 Altium 中运行 ERC/DRC，并重新导出 netlist、BOM、层叠和 Gerber；本仓库的 OLE inventory 只记录器件/网络标签，不等于电气连通性。
