#!/usr/bin/env python3
"""Writes the views of the game's types (docs/e5-native.md, step 2) from the
schema (itc_recomp/schema/*.toml) into itc_recomp/src/schema/: a header per
area (`<area>.h`, the area's name with `_`) and `types.h` (every type
declared, so a pointer field may name a type of another area).

A view is `base` and a guest address (schema/view.h): it holds no storage,
and its accessors read and write guest memory as the code does today
(guest::Load*, guest::Store*), so code switched to views changes nothing
the game sees. For each field `name` at +offset:
  name()              its value (a ptr field: a view of the type it points to)
  set_name(v)         stores it (a ptr field: a view or an address)
  name_address()      the field's own guest address
and `kSize`, the type's size. The `[globals]` table of an area becomes a
struct `<Area>Globals` with the same accessors, static, taking `base`.

usage: schema_gen.py      (writes every header; checks names and pointees)
"""
import glob
import os
import re
import sys
import tomllib

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
SCHEMA = os.path.join(ROOT, "schema")
OUT = os.path.join(ROOT, "src", "schema")

SCALARS = {
    "u8": ("uint8_t", "guest::LoadU8(base, {a})", "guest::StoreU8(base, {a}, v)"),
    "s8": ("int8_t", "int8_t(guest::LoadU8(base, {a}))", "guest::StoreU8(base, {a}, uint8_t(v))"),
    "u16": ("uint16_t", "guest::LoadU16(base, {a})", "guest::StoreU16(base, {a}, v)"),
    "s16": ("int16_t", "int16_t(guest::LoadU16(base, {a}))", "guest::StoreU16(base, {a}, uint16_t(v))"),
    "u32": ("uint32_t", "guest::LoadU32(base, {a})", "guest::StoreU32(base, {a}, v)"),
    "s32": ("int32_t", "int32_t(guest::LoadU32(base, {a}))", "guest::StoreU32(base, {a}, uint32_t(v))"),
    "fn": ("uint32_t", "guest::LoadU32(base, {a})", "guest::StoreU32(base, {a}, v)"),
    "u64": ("uint64_t", "guest::LoadU64(base, {a})", "guest::StoreU64(base, {a}, v)"),
    "f32": ("float", "guest::LoadFloat(base, {a})", "guest::StoreFloat(base, {a}, v)"),
    "f64": ("double", "std::bit_cast<double>(guest::LoadU64(base, {a}))",
            "guest::StoreU64(base, {a}, std::bit_cast<uint64_t>(v))"),
}
IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


def load():
    areas = {}
    for path in sorted(glob.glob(os.path.join(SCHEMA, "*.toml"))):
        area = os.path.basename(path)[:-5]
        with open(path, "rb") as f:
            areas[area] = tomllib.load(f)
    return areas


def check(areas):
    errors = []
    types = {}
    for area, data in areas.items():
        for name, t in data.items():
            if name == "globals":
                continue
            if not IDENT.match(name):
                errors.append(f"{area}: type name {name!r}")
            if name in types:
                errors.append(f"{area}: type {name} also in {types[name]}")
            types[name] = area
    for area, data in areas.items():
        for name, t in data.items():
            fields = (t.items() if name == "globals" else
                      ((f["name"], f) for f in t.get("fields", [])))
            seen = set()
            for fname, f in fields:
                if not IDENT.match(fname) or fname in seen:
                    errors.append(f"{area}: {name}.{fname}: a bad or repeated name")
                seen.add(fname)
                ty = f.get("type")
                if ty == "ptr" and f.get("to") and f["to"] not in types:
                    errors.append(f"{area}: {name}.{fname} points to an unknown type {f['to']}")
                if ty not in SCALARS and ty not in ("ptr", "bytes"):
                    errors.append(f"{area}: {name}.{fname}: an unknown type {ty!r}")
    return types, errors


def accessors(fname, f, at, static):
    """The accessors of a field at the address expression `at`."""
    pre = "static " if static else ""
    cst = "" if static else " const"
    bp = "uint8_t* base" if static else ""
    sep = ", " if static else ""
    ty = f["type"]
    out = []
    if ty == "ptr":
        to = f.get("to")
        if to:
            out.append(f"  template <typename T = {to}>")
            out.append(f"  {pre}T {fname}({bp}){cst} {{ return T(base, guest::LoadU32(base, {at})); }}")
            out.append(f"  {pre}void set_{fname}({bp}{sep}const View& v){cst} {{ guest::StoreU32(base, {at}, v.address); }}")
        else:
            out.append(f"  {pre}View {fname}({bp}){cst} {{ return View(base, guest::LoadU32(base, {at})); }}")
        out.append(f"  {pre}void set_{fname}({bp}{sep}uint32_t v){cst} {{ guest::StoreU32(base, {at}, v); }}")
    elif ty == "bytes":
        pass
    else:
        ctype, load, store = SCALARS[ty]
        out.append(f"  {pre}{ctype} {fname}({bp}){cst} {{ return {load.format(a=at)}; }}")
        out.append(f"  {pre}void set_{fname}({bp}{sep}{ctype} v){cst} {{ {store.format(a=at)}; }}")
    out.append(f"  {pre}uint32_t {fname}_address(){cst} {{ return {at}; }}" if not static else
               f"  static constexpr uint32_t {fname}_address() {{ return {at}; }}")
    return out


def camel(area):
    return "".join(p.capitalize() for p in re.split(r"[-_]", area))


def emit_area(area, data):
    lines = [
        f"// The views of docs/functions/{area}.md's types (docs/e5-native.md, step 2):",
        f"// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/{area}.toml.",
        "// Don't edit it by hand: edit the schema and run the tool again.",
        "",
        "#pragma once",
        "",
        '#include "schema/types.h"',
        "",
        "namespace schema {",
        "",
    ]
    for name, t in data.items():
        if name == "globals":
            continue
        if t.get("measure") or t.get("origin"):
            lines.append(f"// {t.get('origin', '?')}; measure: {t.get('measure', '-')}")
        lines.append(f"struct {name} : View {{")
        lines.append("  using View::View;")
        lines.append(f"  static constexpr uint32_t kSize = {t.get('size', 0)};")
        if "vtable" in t:
            lines.append(f"  static constexpr uint32_t kVtable = 0x{t['vtable']:08X};")
        for f in t.get("fields", []):
            if f.get("note"):
                lines.append(f"  // {f['note']}")
            lines.extend(accessors(f["name"], f, f"address + {f['offset']}", False))
        lines.append("};")
        lines.append("")
    if "globals" in data:
        lines.append(f"// The image words docs/functions/{area}.md's code uses.")
        lines.append(f"struct {camel(area)}Globals {{")
        for gname, g in data["globals"].items():
            if g.get("note"):
                lines.append(f"  // {g['note']}")
            lines.extend(accessors(gname, g, f"0x{g['address']:08X}u", True))
        lines.append("};")
        lines.append("")
    lines += ["}  // namespace schema", ""]
    return "\n".join(lines)


def main(argv):
    areas = load()
    types, errors = check(areas)
    if errors:
        for e in errors:
            print(e, file=sys.stderr)
        return 1
    os.makedirs(OUT, exist_ok=True)
    decl = [
        "// Every type of the schema, declared (docs/e5-native.md, step 2): written by",
        "// itc_recomp/tools/schema_gen.py. Don't edit it by hand.",
        "",
        "#pragma once",
        "",
        '#include "schema/view.h"',
        "",
        "namespace schema {",
        "",
    ] + [f"struct {t};" for t in sorted(types)] + ["", "}  // namespace schema", ""]
    with open(os.path.join(OUT, "types.h"), "w") as f:
        f.write("\n".join(decl))
    written = {"types.h", "view.h"}
    for area, data in areas.items():
        name = area.replace("-", "_") + ".h"
        with open(os.path.join(OUT, name), "w") as f:
            f.write(emit_area(area, data))
        written.add(name)
    for stale in set(os.listdir(OUT)) - written:
        os.remove(os.path.join(OUT, stale))
    print(f"schema: {len(areas)} areas, {len(types)} types")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
