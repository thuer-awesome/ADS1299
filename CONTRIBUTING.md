# Contributing

本仓库同时包含用户自有硬件/固件和第三方库快照。提交前请确认你有权公开所提交的内容；未经授权的 TI、ST、蓝牙模块手册、工具和二进制文件不要复制进仓库。

硬件修改必须同时更新原理图、PCB、BOM、ERC/DRC 记录和 [`docs/traceability/README.md`](docs/traceability/README.md)。固件修改必须附寄存器回读、帧完整性或回归测试证据。没有实测证据的项目保持 `OPEN`，不要用推断结果替换。

新增测量结果请按 `tests/results/README.md` 命名，并把仪器设置、板卡版本、芯片批次和数据手册版本写入同名元数据。蓝牙 MAC、调试器序列号、人体数据和个人信息必须脱敏。

提交前运行：

```text
python scripts/validate_repository.py
git diff --check
git lfs fsck
```
