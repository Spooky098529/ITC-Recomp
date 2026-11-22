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
and `kSize`, the type's size. A type with `storage = "host"` (step 3) lives
in host memory instead: see host_type. The `[globals]` table of an area becomes a
struct `<Area>Globals` with the same accessors, static, taking `base`.

usage: schema_gen.py              (writes every header; checks names and pointees)
       schema_gen.py --area <a>   (only <a>'s header; other files that don't
                                   parse are skipped: agents side by side)
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
WIDTHS = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "fn": 4, "ptr": 4,
          "f32": 4, "u64": 8, "f64": 8}
# A host type's members (C++ type) and the raw bits of the field's width.
HOST = {
    "u8": ("uint8_t", "uint8_t"), "s8": ("int8_t", "uint8_t"),
    "u16": ("uint16_t", "uint16_t"), "s16": ("int16_t", "uint16_t"),
    "u32": ("uint32_t", "uint32_t"), "s32": ("int32_t", "uint32_t"),
    "fn": ("uint32_t", "uint32_t"), "ptr": ("uint32_t", "uint32_t"),
    "u64": ("uint64_t", "uint64_t"), "f32": ("float", "uint32_t"), "f64": ("double", "uint64_t"),
}
IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
# C++ keywords a field can't be named (its accessor would be one).
CPP_KEYWORDS = set("""alignas alignof and and_eq asm auto bitand bitor bool break case catch char
char8_t char16_t char32_t class compl concept const consteval constexpr constinit const_cast
continue co_await co_return co_yield decltype default delete do double dynamic_cast else enum
explicit export extern false float for friend goto if inline int long mutable namespace new
noexcept not not_eq nullptr operator or or_eq private protected public register
reinterpret_cast requires return short signed sizeof static static_assert static_cast struct
switch template this thread_local throw true try typedef typeid typename union unsigned using
virtual void volatile wchar_t while xor xor_eq""".split())


def load(tolerant=False):
    """Every area's schema; with `tolerant`, a file that doesn't parse (an
    agent mid-edit) is skipped with a warning."""
    areas = {}
    for path in sorted(glob.glob(os.path.join(SCHEMA, "*.toml"))):
        area = os.path.basename(path)[:-5]
        try:
            with open(path, "rb") as f:
                areas[area] = tomllib.load(f)
        except tomllib.TOMLDecodeError as e:
            if not tolerant:
                raise
            print(f"{area}: skipped (doesn't parse: {e})", file=sys.stderr)
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
    claimed = {}
    for area, data in areas.items():
        for name, t in data.items():
            if name == "globals":
                continue
            for m in ([t["measure"]] if t.get("measure") else []) + t.get("measures", []):
                if m in claimed and claimed[m] != (area, name):
                    errors.append(f"{area}: {name} claims the measure's {m!r}, "
                                  f"also {claimed[m][1]}'s ({claimed[m][0]})")
                claimed[m] = (area, name)
    # Lifted allocation and free sites (docs/loops/e5-loop.md, "lift.py's
    # host_alloc and host_free"): host types only, return addresses in the
    # image, each listed once.
    sites = {}
    for area, data in areas.items():
        for name, t in data.items():
            if name == "globals" or ("host_alloc" not in t and "host_free" not in t):
                continue
            if t.get("storage") != "host":
                errors.append(f"{area}: {name}: host_alloc and host_free are for host types only")
                continue
            calls = []
            alloc = t.get("host_alloc", [])
            if not isinstance(alloc, list) or not all(isinstance(a, int) for a in alloc):
                errors.append(f"{area}: {name}: host_alloc is a list of call return addresses")
            else:
                calls += alloc
            frees = t.get("host_free", [])
            if not isinstance(frees, list):
                errors.append(f"{area}: {name}: host_free is a list of {{ call, reg, returns }}")
                frees = []
            for fr in frees:
                if (not isinstance(fr, dict) or set(fr) != {"call", "reg", "returns"}
                        or not isinstance(fr["call"], int) or not isinstance(fr["returns"], int)
                        or not isinstance(fr["reg"], str) or not re.fullmatch(r"r([3-9]|10)", fr["reg"])):
                    errors.append(f"{area}: {name}: host_free entry {fr!r}: "
                                  "{ call = <return address>, reg = \"r3\"..\"r10\", returns = <int> }")
                    continue
                calls.append(fr["call"])
            for a in calls:
                if not 0x82000000 <= a < 0x83000000 or a % 4:
                    errors.append(f"{area}: {name}: 0x{a:08X} isn't a return address in the image")
                if a in sites:
                    errors.append(f"{area}: {name}: 0x{a:08X} also listed by {sites[a]}")
                sites[a] = name
    for area, data in areas.items():
        for name, t in data.items():
            if name == "globals" or t.get("storage", "guest") == "guest":
                continue
            if t.get("storage") != "host":
                errors.append(f"{area}: {name}: storage {t.get('storage')!r} (guest or host)")
                continue
            spans = []
            for f in t.get("fields", []):
                width = f.get("count", 0) if f.get("type") == "bytes" else WIDTHS.get(f.get("type"), 0)
                spans.append((f["offset"], f["offset"] + width, f["name"]))
            spans.sort()
            for (a0, a1, an), (b0, b1, bn) in zip(spans, spans[1:]):
                if b0 < a1:
                    errors.append(f"{area}: {name} (host): {an} and {bn} overlap; a host type's "
                                  "fields are members")
    for area, data in areas.items():
        for name, t in data.items():
            fields = (t.items() if name == "globals" else
                      ((f["name"], f) for f in t.get("fields", [])))
            seen = set()
            for fname, f in fields:
                if not IDENT.match(fname) or fname in CPP_KEYWORDS or fname in seen:
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


def host_type(name, t):
    """A host type (`storage = "host"`, docs/loops/e5-loop.md step 3): its
    members in `<name>Data`, and a view over a handle (memory/handles.h) with
    the same accessors as a guest view's but no `_address()`, plus
    `<field>_bits()` and `set_<field>_bits()` (the raw bits of the field's
    width, what lifted code loads and stores) and `New` and `Delete`."""
    out = [f"struct {name}Data {{"]
    for f in sorted(t.get("fields", []), key=lambda f: f["offset"]):
        if f["type"] == "bytes":
            out.append(f"  uint8_t {f['name']}[{f['count']}] = {{}};  // +{f['offset']}, guest order")
        else:
            out.append(f"  {HOST[f['type']][0]} {f['name']} = 0;  // +{f['offset']}")
    out.append("};")
    out.append("")
    out.append(f"struct {name} : View {{")
    out.append("  using View::View;")
    out.append(f"  static constexpr uint32_t kSize = {t.get('size', 0)};")
    out.append("  static constexpr bool kHost = true;")
    if "vtable" in t:
        out.append(f"  static constexpr uint32_t kVtable = 0x{t['vtable']:08X};")
    out.append(f"  {name}Data* data() const {{ return static_cast<{name}Data*>(memory::HandleObject(address)); }}")
    out.append(f"  static {name} New(uint8_t* base) {{")
    out.append(f"    return {name}(base, memory::MakeHandle(new {name}Data(), kSize, \"{name}\"));")
    out.append("  }")
    out.append(f"  void Delete() const {{ delete data(); memory::ReleaseHandle(address); }}")
    for f in t.get("fields", []):
        n, ty = f["name"], f["type"]
        if f.get("note"):
            out.append(f"  // {f['note']}")
        if ty == "bytes":
            out.append(f"  uint8_t* {n}() const {{ return data()->{n}; }}")
            continue
        member, bits = HOST[ty]
        if ty == "ptr":
            to = f.get("to")
            if to:
                out.append(f"  template <typename T = {to}>")
                out.append(f"  T {n}() const {{ return T(base, data()->{n}); }}")
                out.append(f"  void set_{n}(const View& v) const {{ data()->{n} = v.address; }}")
            else:
                out.append(f"  View {n}() const {{ return View(base, data()->{n}); }}")
            out.append(f"  void set_{n}(uint32_t v) const {{ data()->{n} = v; }}")
        else:
            out.append(f"  {member} {n}() const {{ return data()->{n}; }}")
            out.append(f"  void set_{n}({member} v) const {{ data()->{n} = v; }}")
        if member == bits:
            out.append(f"  {bits} {n}_bits() const {{ return data()->{n}; }}")
            out.append(f"  void set_{n}_bits({bits} v) const {{ data()->{n} = v; }}")
        else:
            out.append(f"  {bits} {n}_bits() const {{ return std::bit_cast<{bits}>(data()->{n}); }}")
            out.append(f"  void set_{n}_bits({bits} v) const {{ data()->{n} = std::bit_cast<{member}>(v); }}")
    out.append("};")
    out.append("")
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
        measures = ([t["measure"]] if t.get("measure") else []) + t.get("measures", [])
        if measures or t.get("origin"):
            lines.append(f"// {t.get('origin', '?')}; measure: {', '.join(measures) or '-'}")
        if t.get("storage") == "host":
            lines.extend(host_type(name, t))
            continue
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
    if len(argv) == 3 and argv[1] == "--area":
        # One area's header only, from its file, checked against every file
        # that parses: for agents working on areas side by side (the other
        # headers and types.h are written by a full run).
        area = argv[2].removesuffix(".toml").removesuffix(".md")
        areas = load(tolerant=True)
        if area not in areas:
            print(f"{area}: no schema file (or it doesn't parse)", file=sys.stderr)
            return 1
        types, errors = check(areas)
        errors = [e for e in errors if e.startswith(f"{area}:")]
        if errors:
            for e in errors:
                print(e, file=sys.stderr)
            return 1
        os.makedirs(OUT, exist_ok=True)
        with open(os.path.join(OUT, area.replace("-", "_") + ".h"), "w") as f:
            f.write(emit_area(area, areas[area]))
        print(f"schema: {area}: {sum(1 for k in areas[area] if k != 'globals')} types")
        return 0
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
