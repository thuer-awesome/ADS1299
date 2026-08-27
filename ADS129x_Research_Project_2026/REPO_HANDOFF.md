# ADS1299 仓库同步说明

目标远程仓库：`https://github.com/thuer-awesome/ADS1299`

本目录已经准备好作为目标仓库的研究资料子目录提交。推送前仍需确认 GitHub 凭据、默认分支和远程是否有并行更新；不得覆盖远程已有提交。

## 建议同步步骤

1. 在网络可用且已登录的机器上克隆仓库，并确认默认分支、已有目录和许可证：

   ```text
   git clone https://github.com/thuer-awesome/ADS1299.git
   git -C ADS1299 branch --show-current
   git -C ADS1299 log -1 --oneline
   ```

2. 将 `ADS129x_Research_Project_2026/` 放到仓库约定的研究目录，先运行 `git diff --no-index` 或人工审查，确认没有覆盖已有文件。
3. 需要提交原始 Altium/PDF/波形时，先确认许可证和 Git LFS 配额；默认只提交本目录中的索引、哈希、CSV、Markdown 和提取工具。
4. 提交前运行 CSV 校验、`python -B tools/extract_altium_inventory.py ...` 和 Markdown 检查，把测试仪器和板卡序列号写入 `data/processed/`。
5. 通过 Pull Request 合并；不要在未完成实测时把 `OPEN` 改为 `PASS`。
