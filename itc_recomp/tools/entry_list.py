#!/usr/bin/env python3
"""Writes src/lifted/entries.h (docs/e5-native.md, step 0): a declaration of
every hand port's entry, `lifted::entry::e<address>`, so lifted code and the
hand ports call an entry directly instead of through the dispatch table by
its address.

The entries are made by ITC_NATIVE_ENTRY and the wrapper macros around it
(src/hooks/native_entry.h), whose forms vary too much to read from the
sources, so the list comes from a build's symbols (`nm`). Run it after a
build that adds or removes an entry; a call to an entry that's gone fails
the link, and a new entry is simply not called directly until the list is
written again (lift.py reads the same list: --entries).

usage: entry_list.py [path to the itc executable]
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
DEFAULT_EXE = os.path.join(ROOT, "out", "build", "linux-amd64-release", "itc")
HEADER = os.path.join(ROOT, "src", "lifted", "entries.h")

SYMBOL = re.compile(r"\blifted::entry::e(8[0-9A-F]{7})\(")


def entries_in(exe):
    out = subprocess.run(["nm", "-C", exe], check=True, capture_output=True, text=True).stdout
    return sorted(set(SYMBOL.findall(out)))


def entries_in_header(path=HEADER):
    """The addresses the current header declares (for lift.py)."""
    with open(path) as f:
        return sorted(set(re.findall(r"Result e(8[0-9A-F]{7})\(", f.read())))


def main(argv):
    exe = argv[1] if len(argv) > 1 else DEFAULT_EXE
    addresses = entries_in(exe)
    if not addresses:
        print(f"no lifted::entry symbols in {exe}", file=sys.stderr)
        return 1
    lines = [
        "// Every hand port's entry (src/hooks/native_entry.h), declared so code",
        "// that knows its callee calls it directly (docs/e5-native.md, step 0):",
        "// written by itc_recomp/tools/entry_list.py from a build's symbols.",
        "// Don't edit it by hand: run the tool again.",
        "",
        "#pragma once",
        "",
        '#include "lifted/lifted.h"',
        "",
        "namespace lifted::entry {",
        "",
    ]
    lines += [f"Result e{a}(const Args& args);" for a in addresses]
    lines += ["", "}  // namespace lifted::entry", ""]
    with open(HEADER, "w") as f:
        f.write("\n".join(lines))
    print(f"entries.h: {len(addresses)} entries")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
