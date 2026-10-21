#!/usr/bin/env python3
"""Which recompiled functions can be reached at all, from the code alone.
Reports only; changes nothing.

usage: static_reachability.py <image> [--unreachable <out file>]

<image> is the loaded executable image from --dump_image. Starting from the
roots, follows the edges:
  roots  xstart; every word in .rdata/.data that is a function's start
         (vtables, callbacks, thread entries: a data word can't be credited
         to a function, so all count); every guest function src/ names
         outside an override's definition
  edges  direct calls (a function's body in generated/ naming another), and
         lis/addi (or ori) address constants in a function's own code that
         land on a function's start (member pointers, callbacks passed on)
Vtables point at every method of every class, used or not, so this only
finds what nothing can reach; what's reachable but never run takes a
coverage build. Overridden functions keep their generated body's edges,
which overcounts a little where the override no longer calls them.
It also misses addresses built across registers (lis r9,hi, later
addi r28,r28,lo): 23 functions a 20-second attract race ran were
"unreachable" here (2026-10-21). The coverage build is the measure; this
is a quick look without running the game.
"""

import os
import re
import struct
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
GENERATED = os.path.join(ROOT, "itc_recomp", "generated", "default")
SRC = os.path.join(ROOT, "itc_recomp", "src")

# The same ranges as scan_ptrs.py. .pdata (between .rdata and .text) is left
# out on purpose: it lists every function's start.
BASE = 0x82000000
TEXT = (0x82090000, 0x82090000 + 0x54C064)
DATA_RANGES = [(0x82000600, 0x82000600 + 0x76DC4),  # .rdata
               (0x82600000, 0x82600000 + 0x37833C)]  # .data


def main():
    args = sys.argv[1:]
    unreachable_out = None
    if "--unreachable" in args:
        i = args.index("--unreachable")
        unreachable_out = args[i + 1]
        del args[i:i + 2]
    if len(args) != 1:
        sys.exit(__doc__)
    img = open(args[0], "rb").read()

    def word(addr):
        o = addr - BASE
        return struct.unpack(">I", img[o:o + 4])[0]

    # Name <-> address, from the registration table.
    addr_of, name_of = {}, {}
    for m in re.finditer(r"SetFunction\((0x[0-9A-Fa-f]+), (\w+)\)",
                         open(os.path.join(GENERATED, "itc_register.cpp")).read()):
        a, n = int(m.group(1), 16), m.group(2)
        addr_of[n] = a
        name_of.setdefault(a, n)

    # Bodies and their direct calls.
    calls, lines = {}, {}
    define = re.compile(r"^DEFINE_REX_FUNC\((\w+)\)")
    ident = re.compile(r"\b([A-Za-z_]\w*)\(")
    for f in sorted(os.listdir(GENERATED)):
        if not re.match(r"itc_recomp\.\d+\.cpp$", f):
            continue
        cur = None
        for line in open(os.path.join(GENERATED, f)):
            m = define.match(line)
            if m:
                cur = m.group(1)
                calls.setdefault(cur, set())
                lines[cur] = 0
                continue
            if cur is None or line.lstrip().startswith("//"):
                continue
            lines[cur] += 1
            for c in ident.findall(line):
                if c in addr_of and c != cur:
                    calls[cur].add(c)
    defined = set(calls)
    for n in calls:  # kernel imports are registered too: not recompiled
        calls[n] &= defined

    # Address constants in each function's own code.
    starts = sorted(a for a, n in name_of.items() if n in defined)
    start_set = set(starts)
    for k, a0 in enumerate(starts):
        a1 = starts[k + 1] if k + 1 < len(starts) else TEXT[1]
        if not TEXT[0] <= a0 < TEXT[1]:
            continue
        here = name_of[a0]
        for a in range(a0, min(a1, TEXT[1]) - 4, 4):
            i = word(a)
            if (i >> 26) != 15 or ((i >> 16) & 31) != 0:  # lis = addis rD,0,imm
                continue
            rd, hi = (i >> 21) & 31, (i & 0xFFFF) << 16
            for b in range(a + 4, min(a + 4 * 12, a1), 4):
                j = word(b)
                op, ra = j >> 26, (j >> 16) & 31
                if ra == rd and op in (14, 24):  # addi / ori
                    lo = j & 0xFFFF
                    v = (hi + (lo - 0x10000 if op == 14 and lo & 0x8000 else lo)) & 0xFFFFFFFF
                    if v in start_set and name_of[v] != here:
                        calls[here].add(name_of[v])
                    break
                if ((j >> 21) & 31) == rd and op not in (36, 37, 38, 44, 45, 47):  # rD overwritten (not a store)
                    break

    # Roots.
    roots = {"xstart"} & defined
    data_roots = set()
    for lo, hi in DATA_RANGES:
        for a in range(lo, hi - 3, 4):
            v = word(a)
            if v in start_set:
                data_roots.add(name_of[v])
    src_roots = set()
    override = re.compile(r"REX_FUNC\((sub_[0-9A-F]{8})\)")
    for d, _, fs in os.walk(SRC):
        for f in fs:
            if not f.endswith((".cpp", ".h", ".inc")):
                continue
            text = open(os.path.join(d, f), errors="replace").read()
            text = override.sub("", text)
            for n in re.findall(r"\b(?:__imp__)?(sub_[0-9A-F]{8})\b", text):
                if n in defined:
                    src_roots.add(n)
    roots |= data_roots | src_roots

    seen, stack = set(), list(roots)
    while stack:
        n = stack.pop()
        if n in seen:
            continue
        seen.add(n)
        stack.extend(calls[n] - seen)

    unreachable = sorted(defined - seen, key=lambda n: addr_of[n])
    total_lines = sum(lines.values())
    dead_lines = sum(lines[n] for n in unreachable)
    print(f"functions: {len(defined)} ({total_lines} lines of generated code, comments left out)")
    print(f"roots: {len(roots)} (xstart, {len(data_roots)} from data, {len(src_roots)} named in src/)")
    print(f"reachable: {len(seen)}")
    print(f"unreachable: {len(unreachable)} ({dead_lines} lines, "
          f"{100.0 * dead_lines / total_lines:.1f}% of the code)")
    if unreachable_out:
        with open(unreachable_out, "w") as out:
            for n in unreachable:
                out.write(f"0x{addr_of[n]:08X} {n}\n")


if __name__ == "__main__":
    main()
