#!/usr/bin/env python3
"""Writes src/lifted/imports.h, the image's kernel and XAM imports declared
(each a record of ours with its name, the key our answers are found by:
src/imports/native_imports.h), and src/lifted/imports.cpp: a dispatch table entry for each of the
image's import thunks (docs/rexglue-removal.md, "Before E1: the rewiring",
stage 4), read from generated/default/itc_init.cpp's function mappings,
whose names the records carry for diagnostics (--log_imports).

Native code that calls a kernel or XAM import by its thunk's address (the
ports that call `kExCreateThread = 0x825DBBE4` and the like through their
`Calls`) reaches it through lifted::Call like any other function: the
entry runs the import through lifted::Import (our answer, or a log and a
stop: E2 and E4 in docs/rexglue-removal.md).

usage: import_entries.py
"""

import os
import re
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(TOOLS, "..", ".."))
INIT = os.path.join(ROOT, "itc_recomp", "generated", "default", "itc_init.cpp")
OUT = os.path.join(ROOT, "itc_recomp", "src", "lifted", "imports.cpp")
HEADER = os.path.join(ROOT, "itc_recomp", "src", "lifted", "imports.h")


def main():
    with open(INIT) as f:
        thunks = re.findall(r"\{ 0x([0-9A-F]{8}), (__imp__\w+) \}", f.read())
    thunks = sorted(set(thunks))
    lines = [
        "// The import thunks' dispatch table entries (docs/rexglue-removal.md,",
        "// \"Before E1: the rewiring\", stage 4): written by",
        "// itc_recomp/tools/import_entries.py. Don't edit it by hand: run the tool",
        "// again. A call to an import thunk's address runs the import through",
        "// lifted::Import (our answer, or a log and a stop).",
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
        lines.append(f"Result i{address}(const Args& args) {{")
        lines.append(f"  return Import({name}, args);")
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
    header = [
        "// The image's kernel and XAM imports: written by",
        "// itc_recomp/tools/import_entries.py. Don't edit it by hand: run the tool",
        "// again. Each is a record of ours with the import's name, known by its",
        "// address (`__imp__X`, the name the generated code gave the SDK's",
        "// function): the key lifted::Import finds our answer by",
        "// (docs/rexglue-removal.md, \"E4: the plan\", step 3).",
        "",
        "#pragma once",
        "",
        "namespace lifted {",
        "",
        "// An import: its name, for diagnostics.",
        "struct ImportFunction {",
        "  const char* name;",
        "};",
        "",
        "namespace import_records {",
        "",
    ]
    names = sorted({n for _, n in thunks})
    for name in names:
        header.append(f'inline ImportFunction {name[len("__imp__"):]}{{"{name[len("__imp__"):]}"}};')
    header += ["", "}  // namespace import_records", "", "}  // namespace lifted", ""]
    for name in names:
        header.append(f"inline lifted::ImportFunction* const {name} = &lifted::import_records::{name[len('__imp__'):]};")
    header.append("")
    with open(HEADER, "w") as f:
        f.write("\n".join(header))
    print(f"wrote {len(thunks)} import entries")
    return 0


if __name__ == "__main__":
    sys.exit(main())
