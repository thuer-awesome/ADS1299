"""Validate the public repository structure and reviewed CSV tables."""

from __future__ import annotations

import csv
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CSV_ROOTS = (ROOT / "ADS129x_Research_Project_2026" / "data", ROOT / "data")
REQUIRED = (
    ROOT / "README.md",
    ROOT / "docs" / "README.md",
    ROOT / "hardware" / "README.md",
    ROOT / "firmware" / "README.md",
    ROOT / "tests" / "README.md",
    ROOT / "validation" / "README.md",
    ROOT / "scripts" / "README.md",
    ROOT / "ADS129x_Research_Project_2026" / "docs" / "01_datasheet_traceability.md",
    ROOT / "ADS129x_Research_Project_2026" / "docs" / "02_ads1299_test_reference_design.md",
)


def check_csv(path: Path) -> tuple[bool, str]:
    with path.open("r", encoding="utf-8-sig", newline="") as handle:
        rows = list(csv.reader(handle))
    if not rows:
        return False, "empty"
    width = len(rows[0])
    bad = [line for line, row in enumerate(rows[1:], 2) if len(row) != width]
    return (not bad, f"rows={len(rows) - 1} columns={width}" if not bad else f"bad_lines={bad}")


def main() -> int:
    failed = False
    for path in REQUIRED:
        ok = path.is_file()
        print(f"{'PASS' if ok else 'FAIL'} required {path.relative_to(ROOT)}")
        failed |= not ok

    seen: set[Path] = set()
    for root in CSV_ROOTS:
        if not root.exists():
            continue
        for path in sorted(root.rglob("*.csv")):
            if path in seen:
                continue
            seen.add(path)
            try:
                ok, detail = check_csv(path)
            except (OSError, UnicodeError, csv.Error) as exc:
                ok, detail = False, str(exc)
            print(f"{'PASS' if ok else 'FAIL'} csv {path.relative_to(ROOT)} {detail}")
            failed |= not ok

    sensitive = re.compile(r"(?i:(?:[0-9a-f]{2}:){5}[0-9a-f]{2})")
    for path in (ROOT / "docs", ROOT / "data", ROOT / "validation"):
        for candidate in path.rglob("*"):
            if not candidate.is_file() or candidate.suffix.lower() not in {".md", ".py", ".csv", ".yml", ".yaml"}:
                continue
            text = candidate.read_text(encoding="utf-8", errors="ignore")
            if sensitive.search(text):
                print(f"FAIL sensitive marker {candidate.relative_to(ROOT)}")
                failed = True
    print("repository_validation=" + ("FAIL" if failed else "PASS"))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
