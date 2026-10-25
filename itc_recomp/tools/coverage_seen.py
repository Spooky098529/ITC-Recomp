#!/usr/bin/env python3
"""Turns the coverage build's output (itc_coverage writes coverage/seen_<pid>.txt
next to the executable: see src/debug/coverage.cpp) into guest addresses and
adds them to the list of recompiled functions seen in play,
itc_recomp/coverage/seen_in_play.txt (one address a line, sorted; generated,
never edited by hand). Sessions add up: a function is in the list once any
session ran it.

usage: coverage_seen.py <itc_coverage> <seen_<pid>.txt>... [--dry-run]

A function counts as run when its recompiled body ran (__imp__sub_X) or its
entry was called (sub_X, the weak thunk overrides replace). An override that
no longer calls its recompiled body leaves the function out: it's ours.
"""

import os
import re
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
GENERATED = os.path.join(ROOT, "itc_recomp", "generated", "default")
LIST = os.path.join(ROOT, "itc_recomp", "coverage", "seen_in_play.txt")


def main():
    args = sys.argv[1:]
    dry_run = "--dry-run" in args
    args = [a for a in args if a != "--dry-run"]
    if len(args) < 2:
        sys.exit(__doc__)
    binary, seen_files = args[0], args[1:]

    addr_of = {}
    for m in re.finditer(r"SetFunction\((0x[0-9A-Fa-f]+), (\w+)\)",
                         open(os.path.join(GENERATED, "itc_register.cpp")).read()):
        addr_of[m.group(2)] = int(m.group(1), 16)
    defined = set()  # the registration table has the kernel imports too
    for f in os.listdir(GENERATED):
        if re.match(r"itc_recomp\.\d+\.cpp$", f):
            defined.update(re.findall(r"^DEFINE_REX_FUNC\((\w+)\)",
                                      open(os.path.join(GENERATED, f)).read(), re.M))
    addr_of = {n: a for n, a in addr_of.items() if n in defined}

    # Host function address -> guest address, from the symbol table.
    guest_at = {}
    nm = subprocess.run(["nm", "--defined-only", binary], capture_output=True, text=True, check=True)
    for line in nm.stdout.splitlines():
        parts = line.split()
        if len(parts) != 3 or parts[1] not in "tTwW":
            continue
        name = parts[2][len("__imp__"):] if parts[2].startswith("__imp__") else parts[2]
        if name in addr_of:
            guest_at[int(parts[0], 16)] = addr_of[name]

    seen, unknown = set(), 0
    for path in seen_files:
        for line in open(path):
            line = line.strip()
            if not line:
                continue
            g = guest_at.get(int(line, 16))
            if g is None:
                unknown += 1  # instrumented code that isn't a guest function
            else:
                seen.add(g)

    before = set()
    if os.path.exists(LIST):
        before = {int(l, 16) for l in open(LIST) if l.strip()}
    after = before | seen
    total = len(set(addr_of.values()))
    print(f"this run: {len(seen)} functions ran ({unknown} other instrumented functions left out)")
    print(f"seen in play: {len(after)} of {total} recompiled functions "
          f"({len(after) - len(before)} new; {len(before)} before)")
    if not dry_run:
        os.makedirs(os.path.dirname(LIST), exist_ok=True)
        with open(LIST, "w") as out:
            for a in sorted(after):
                out.write(f"0x{a:08X}\n")
        print(f"wrote {os.path.relpath(LIST, ROOT)}")


if __name__ == "__main__":
    main()
