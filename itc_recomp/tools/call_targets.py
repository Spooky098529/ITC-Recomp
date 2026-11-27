#!/usr/bin/env python3
"""Lists every address our code calls by a constant through the dispatch
table (lifted::Call(0x...) in src/), one a line, for the game's
--check_call_targets: a lifted function or original calling a hand port
whose entry was removed with its adapter stops the program on that path
(two found by pad replays, 2026-11-26).

  python3 itc_recomp/tools/call_targets.py > <file>
  itc --check_call_targets=<file>"""
import pathlib, re, sys

root = pathlib.Path(__file__).resolve().parents[1] / "src"
found = set()
for path in root.rglob("*.cpp"):
    for m in re.finditer(r"lifted::Call\(0x([0-9A-Fa-f]{8})", path.read_text(errors="replace")):
        found.add(m.group(1).upper())
for address in sorted(found):
    print(address)
print("%d addresses" % len(found), file=sys.stderr)
