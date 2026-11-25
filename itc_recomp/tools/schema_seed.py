#!/usr/bin/env python3
"""Seeds an area's schema file (docs/e5-native.md, step 2) from the measure
(tools/type_map.py's out/type_map.json): itc_recomp/schema/<area>.toml with
the types the area owns (private to it, or shared with it the area touching
them most), each field as the measure saw it, named `unknown_<offset>`
until read. The file is then edited by hand (names, types, merges); the
seed never overwrites an existing file.

usage: schema_seed.py <area>      e.g. schema_seed.py type2-dialog
"""
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
SCHEMA = os.path.join(ROOT, "schema")
MEASURE = os.path.join(ROOT, "out", "type_map.json")

WIDTH_TYPES = {"1i": "u8", "2i": "u16", "4i": "u32", "8i": "u64", "4f": "f32", "8d": "f64"}


def cpp_name(measure):
    """A C++ name for a measure's type name (edited by hand after)."""
    t = measure
    t = re.sub(r"^new<(\w+|\?)>@", lambda m: f"New{m.group(1).replace('?', 'Any')}_", t)
    t = re.sub(r"\[([0-9A-F]{8})\]", r"At\1", t)
    t = t.replace(".+", "_").replace("::", "_").replace(".ret", "_ret").replace(".", "_")
    t = re.sub(r"[^A-Za-z0-9_]", "", t)
    return t if t[0].isalpha() else "T" + t


def owner(entry):
    areas = entry["areas"]
    return max(areas, key=lambda a: (areas[a], a)) if areas else None


def field_type(f):
    widths = f["widths"]
    if f["pointee"] and widths == ["4i"]:
        return "ptr"
    if f["fn"] and widths == ["4i"]:
        return "fn"
    if len(widths) == 1:
        return WIDTH_TYPES.get(widths[0], "u32")
    return None


def main(argv):
    if len(argv) != 2:
        print(__doc__)
        return 1
    area = argv[1].removesuffix(".md")
    path = os.path.join(SCHEMA, f"{area}.toml")
    if os.path.exists(path):
        print(f"{path} exists: not overwritten")
        return 1
    with open(MEASURE) as f:
        measure = json.load(f)
    mine = {t: e for t, e in measure.items() if owner(e) == f"{area}.md" and not t.startswith("[")}
    names = {t: cpp_name(t) for t in measure}
    lines = [f"# The schema of docs/functions/{area}.md's objects (docs/e5-native.md, step 2):",
             "# seeded by tools/schema_seed.py from the measure, then edited by hand.",
             "# Field types: u8 u16 u32 u64 s8 s16 s32 f32 f64, ptr (with `to`), fn",
             "# (a function's address), bytes (with `count`). `measure` is the type's",
             "# name in tools/type_map.py's tables.", ""]
    for t in sorted(mine, key=lambda t: (-mine[t]["functions"], t)):
        e = mine[t]
        offs = sorted((int(o), f) for o, f in e["fields"].items() if o.lstrip("-").isdigit())
        if not offs:
            continue
        size = max([o + int(f["widths"][-1][:-1]) for o, f in offs if f["widths"]] + e["alloc_sizes"])
        origin = ("allocated" if e["alloc_sizes"] else
                  "file" if "file" in e["tags"] else "gpu" if "gpu" in e["tags"] else "embedded")
        lines.append(f"[{names[t]}]")
        lines.append(f"measure = {json.dumps(t)}")
        lines.append(f"size = {size}")
        lines.append(f"origin = \"{origin}\"")
        if e["vtables"]:
            lines.append(f"vtable = 0x{e['vtables'][0]}")
        lines.append(f"# {e['functions']} functions; areas: {', '.join(e['areas'])}")
        lines.append("fields = [")
        for o, f in offs:
            ty = field_type(f)
            extra = ""
            if ty == "ptr" and len(f["pointee"]) == 1 and f["pointee"][0] in names:
                extra = f", to = \"{names[f['pointee'][0]]}\""
            elif ty == "ptr":
                ty = "u32"
            note = []
            if ty is None:
                ty = WIDTH_TYPES.get(max(f["widths"], key=lambda w: int(w[:-1])), "u32")
                note.append("also read as " + "/".join(f["widths"]))
            if f["fn"]:
                note.append("fn " + ",".join(f["fn"][:3]))
            lines.append(f"  {{ name = \"unknown_{o}\", offset = {o}, type = \"{ty}\"{extra} }},"
                         + (f"  # {'; '.join(note)}" if note else ""))
        lines.append("]")
        lines.append("")
    os.makedirs(SCHEMA, exist_ok=True)
    with open(path, "w") as f:
        f.write("\n".join(lines))
    print(f"{path}: {sum(1 for l in lines if l.startswith('['))} types")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
