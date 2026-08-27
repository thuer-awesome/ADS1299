#!/usr/bin/env python3
"""Extract a conservative component/net-label inventory from an Altium SchDoc.

This does not reconstruct electrical connectivity.  It only reads the OLE
FileHeader stream and emits evidence that must still be checked against an
Altium-exported netlist before a schematic is signed off.
"""
from __future__ import annotations

import argparse
import csv
import re
from pathlib import Path

import olefile


def fields(record: str) -> dict[str, str]:
    return {k: v.replace("\x00", "") for k, v in re.findall(r"([A-Z][A-Z0-9_.]*)=([^|]*)", record)}


def extract(path: Path):
    ole = olefile.OleFileIO(str(path))
    try:
        raw = ole.openstream(["FileHeader"]).read()
    finally:
        ole.close()
    text = raw.decode("utf-8", errors="ignore")
    records = re.split(r"\|(?=RECORD=)", text)
    out = []
    for i, record in enumerate(records):
        if not record.startswith("RECORD="):
            continue
        kind = fields(record)
        record_type = record.split("|", 1)[0].split("=", 1)[1]
        if record_type == "1":
            # Altium stores component values in a later RECORD=41 Comment entry.
            comments = []
            for later in records[i + 1 :]:
                if later.startswith("RECORD=1|"):
                    break
                f = fields(later)
                if f.get("NAME") == "Comment" and f.get("TEXT"):
                    comments.append(f["TEXT"])
            if kind.get("LIBREFERENCE") or comments:
                out.append(("component", kind.get("LIBREFERENCE", ""), ";".join(comments), kind.get("INDEXINSHEET", "")))
        elif record_type == "18" and kind.get("NAME"):
            out.append(("net_label", kind["NAME"], "", kind.get("INDEXINSHEET", "")))
    return out


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("schdoc", type=Path)
    parser.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args()
    rows = extract(args.schdoc)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["source_file", "record_type", "item", "value_or_net", "index_in_sheet", "connectivity_status"])
        for record_type, item, value, index in rows:
            writer.writerow([str(args.schdoc), record_type, item, value, index, "not reconstructed; verify with exported netlist"])


if __name__ == "__main__":
    main()
