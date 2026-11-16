#!/usr/bin/env python3
"""Writes src/lifted/imports.cpp: a dispatch table entry for each of the
image's import thunks (docs/rexglue-removal.md, "Before E1: the rewiring",
stage 4), read from generated/default/itc_init.cpp's function mappings.

Native code that calls a kernel or XAM import by its thunk's address (the
ports that call `kExCreateThread = 0x825DBBE4` and the like through their
`Calls`) reaches it through lifted::Call like any other function: the
entry runs the import through lifted::Import, the lift code's boundary
with the SDK. Without these, such calls went to the SDK's function table.

usage: import_entries.py
"""

import os
import re
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(TOOLS, "..", ".."))
INIT = os.path.join(ROOT, "itc_recomp", "generated", "default", "itc_init.cpp")
OUT = os.path.join(ROOT, "itc_recomp", "src", "lifted", "imports.cpp")


def main():
    with open(INIT) as f:
        thunks = re.findall(r"\{ 0x([0-9A-F]{8}), (__imp__\w+) \}", f.read())
    thunks = sorted(set(thunks))
    lines = [
        "// The import thunks' dispatch table entries (docs/rexglue-removal.md,",
        "// \"Before E1: the rewiring\", stage 4): written by",
        "// itc_recomp/tools/import_entries.py. Don't edit it by hand: run the tool",
        "// again. A call to an import thunk's address runs the import through the",
        "// SDK boundary (lifted::Import).",
        "",
        '#include "lifted/lifted.h"',
        '#include "lifted/sdk.h"',
        "",
        "#include <iterator>",
        "",
        "namespace lifted {",
        "",
        "namespace {",
        "",
    ]
    for address, name in thunks:
        lines.append(f"Result i{address}(uint8_t* base, const Args& args) {{")
        lines.append(f"  return Import(base, {name}, args);")
        lines.append("}")
    lines += ["", "const bool kOn = true;", "", "const Registration kImports[] = {"]
    for address, _ in thunks:
        lines.append(f"    {{0x{address}, i{address}}},")
    lines += [
        "};",
        "",
        "const bool kRegistered = (Register(kImports, std::size(kImports), &kOn), true);",
        "",
        "}  // namespace",
        "",
        "}  // namespace lifted",
        "",
    ]
    with open(OUT, "w") as f:
        f.write("\n".join(lines))
    print(f"wrote {len(thunks)} import entries")
    return 0


if __name__ == "__main__":
    sys.exit(main())
