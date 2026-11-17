#!/usr/bin/env python3
"""Lifts the originals our C++ still calls (`__imp__sub_X`) into
src/lifted/originals.{cpp,h} (docs/loops/lift-sweep.md): each function's
plain C++ translation and its uniform entry `lifted::original::eX`, which
our code calls through `lifted::Enter` in place of `__imp__sub_X`. They
aren't registered in the dispatch table and have no REX_FUNC of their own
(the function's override, a port, keeps its address); their own calls go
through the dispatch table, as the originals' calls go through the
function table, except to functions lift.py lifted.

usage: lift_originals.py sub_X ...   (writes both files for exactly these)
       lift_originals.py --all          (the originals there now, and every
                                         address lifted code or a hand port
                                         calls with no entry of its own)
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lift


def calls_without_entry():
    """Addresses lifted code or a hand port calls by constant with no entry
    of its own (lifted/entries.h, functions.h): their originals are the
    fallbacks."""
    import glob
    import re
    src = os.path.join(lift.SRC)
    entries = set(re.findall(r"Result e(8[0-9A-F]{7})\(", open(os.path.join(lift.OUT, "entries.h")).read()))
    lifted = {n[4:] for n in lift.load_signatures()}
    targets = set()
    for path in glob.glob(os.path.join(src, "**", "*.cpp"), recursive=True) + \
            glob.glob(os.path.join(src, "**", "*.h"), recursive=True):
        if os.sep + "harnesses" + os.sep in path:
            continue
        text = open(path, errors="ignore").read()
        targets |= {a.upper() for a in re.findall(r"lifted::Call\(base, 0x(8[0-9A-Fa-f]{7})", text)}
        targets |= {a.upper() for a in re.findall(
            r"CallFromContext\([^,()]+, [^,()]+, 0x(8[0-9A-Fa-f]{7})", text)}
    return {"sub_" + a for a in targets - entries - lifted}


def main(argv):
    if argv[1:] == ["--all"]:
        with open(os.path.join(lift.OUT, "originals.h")) as f:
            current = set(re.findall(r"// (sub_8[0-9A-F]{7})'s original", f.read()))
        argv = argv[:1] + sorted(current | calls_without_entry())
    wanted = sorted(set(argv[1:]))
    bodies = lift.load_bodies()
    skip = lift.overridden() - set(wanted)
    lifted_before = lift.load_signatures()
    infos = lift.build(sorted(set(wanted) | set(lifted_before)), bodies, skip)
    refused = [n for n in wanted if not infos[n].lifted]
    for n in refused:
        print(f"{n}: refused: {infos[n].error}")
    names = [n for n in wanted if infos[n].lifted]
    emitting = set(lifted_before)  # direct calls only to lift.py's functions
    lift.mark_needs_lr(infos, emitting)
    cpp = [
        "// The originals our C++ still called (docs/loops/lift-sweep.md): written by",
        "// itc_recomp/tools/lift_originals.py from generated/. Don't edit it by hand:",
        "// run the tool again.",
        "",
        '#include "lifted/originals.h"',
        '#include "lifted/entries.h"',
        '#include "lifted/functions.h"',
        '#include "lifted/names.h"',
        '#include "lifted/sdk.h"',
        "",
        '#include "base/log.h"',
        '#include "helpers/clock.h"',
        "",
        "#include <bit>",
        "#include <cstdlib>",
        "",
        "// A call by address or an import passes every argument register and its",
        "// return address and r31 (the per-object recorder's pairing key reads it).",
        "#define LIFTED_ARGS(lr, r31) \\",
        "  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \\",
        "                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \\",
        "                 uint32_t(r31) }",
        "",
        "namespace lifted::original {",
        "",
    ]
    lift.VIEW_HEADERS.clear()
    emitted = []
    for n in names:
        try:
            body = lift.emit_function(infos[n], infos, emitting)
        except lift.Refused as e:
            print(f"{n}: refused at emission: {e}")
            continue
        cpp.extend(body)
        cpp.append("")
        emitted.append(n)
    names = emitted
    # The schema's views the functions name (docs/e5-native.md, step 2).
    at = cpp.index('#include "lifted/sdk.h"') + 1
    cpp[at:at] = [f'#include "{h}"' for h in sorted(lift.VIEW_HEADERS)]
    for n in names:
        cpp.extend(lift.emit_entry(infos[n]))
        cpp.append("")
    # Every original, the function an address with no entry runs
    # (lifted::Call's fallback, docs/e5-native.md, step 0).
    cpp += ["namespace {", "", "const Registration kFallbacks[] = {"]
    cpp += [f"    {{0x{n[4:]}, {lift.ename(n)}}}," for n in names]
    cpp += ["};", "",
            "const bool kRegistered = (RegisterFallbacks(kFallbacks, std::size(kFallbacks)), true);",
            "", "}  // namespace", ""]
    cpp += ["", "}  // namespace lifted::original", ""]
    h = [
        "// The originals our C++ still called, lifted (lifted/originals.cpp):",
        "// written by itc_recomp/tools/lift_originals.py.",
        "",
        "#pragma once",
        "",
        '#include "lifted/lifted.h"',
        '#include "lifted/regs.h"',
        '#include "lifted/sdk.h"',
        "",
        "namespace lifted::original {",
        "",
    ]
    for n in names:
        h.append(f"// {n}'s original.")
        h.append(f"Result {lift.ename(n)}(uint8_t* base, const Args& a);")
        h.append(f"// The same on a register view (in place of __imp__{n}).")
        h.append(f"inline void c{n[4:]}(Regs& regs, uint8_t* base) {{ regs.Apply({lift.ename(n)}(base, regs.ToArgs())); }}")
    h += ["", "}  // namespace lifted::original", ""]
    with open(os.path.join(lift.OUT, "originals.cpp"), "w") as f:
        f.write(lift.own_spelling("\n".join(cpp)))
    with open(os.path.join(lift.OUT, "originals.h"), "w") as f:
        f.write("\n".join(h))
    print(f"originals: {len(names)} lifted, {len(refused)} refused")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
