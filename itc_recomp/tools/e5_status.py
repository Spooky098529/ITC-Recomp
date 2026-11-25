#!/usr/bin/env python3
"""E5's progress counts (docs/e5-native.md, "Tracking"): the 360 shape left
in the code we build, counted in the sources, and the schema's types by
storage. Reads code, changes none.

usage: e5_status.py
"""
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
SRC = os.path.join(ROOT, "src")
sys.path.insert(0, HERE)
import schema_lib  # noqa: E402

GUEST_ACCESS = re.compile(r"\bguest::(Load|Store)(U8|U16|U32|U64|Float)s?\(")
POINTER = re.compile(r"\bguest::Pointer\(")
CALL_ADDR = re.compile(r"\blifted::Call\(0x8")
CALL_PTR = re.compile(r"\blifted::Call\(uint32_t\(")
VTABLE = re.compile(r"\b(lifted|guest)::VtableWord\(")
FRAME = re.compile(r"^\s*// stwu r1,")
IMAGE = re.compile(r"^\s*// lis r\d+,-32\d{3}")


def count(paths, *patterns):
    totals = [0] * len(patterns)
    for p in paths:
        with open(p, errors="replace") as f:
            for line in f:
                for i, pat in enumerate(patterns):
                    if pat.search(line):
                        totals[i] += 1
    return totals


def main():
    lifted = sorted(glob.glob(os.path.join(SRC, "lifted", "batch_*.cpp"))) + \
        [os.path.join(SRC, "lifted", "originals.cpp")]
    ports = [p for p in glob.glob(os.path.join(SRC, "**", "*.cpp"), recursive=True) +
             glob.glob(os.path.join(SRC, "**", "*.h"), recursive=True)
             if not p.startswith(os.path.join(SRC, "lifted")) and
             not p.startswith(os.path.join(SRC, "schema"))]
    la, lp, lc, lv, lvt, lf, li = count(lifted, GUEST_ACCESS, POINTER, CALL_ADDR, CALL_PTR, VTABLE,
                                       FRAME, IMAGE)
    pa, pp, pvt = count(ports, GUEST_ACCESS, POINTER, VTABLE)
    sch = schema_lib.load()
    storage = {}
    for t in sch.types.values():
        storage[t.storage] = storage.get(t.storage, 0) + 1
    print("| What | Lifted | Hand ports |")
    print("| --- | --- | --- |")
    print(f"| Guest loads and stores (`guest::Load*`, `Store*`) | {la:,} | {pa:,} |")
    print(f"| Raw guest pointers (`guest::Pointer`) | {lp:,} | {pp:,} |")
    print(f"| Calls by constant address (`lifted::Call(0x8...)`) | {lc:,} | |")
    print(f"| Calls through a pointer or slot (`lifted::Call(...)`) | {lv:,} | |")
    print(f"| Vtable words read through `VtableWord` | {lvt:,} | {pvt:,} |")
    print(f"| Functions making a guest stack frame (`stwu r1`) | {lf:,} | |")
    print(f"| Image addresses built (`lis rN,-32xxx`) | {li:,} | |")
    print()
    print("Schema types by storage: " + ", ".join(f"{k} {v:,}" for k, v in sorted(storage.items())))
    return 0


if __name__ == "__main__":
    sys.exit(main())
