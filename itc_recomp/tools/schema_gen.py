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
    alltypes = {n: t for data in areas.values() for n, t in data.items() if n != "globals"}
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
            if name == "globals" or not {"host_alloc", "host_alloc_array", "host_free"} & set(t):
                continue
            if t.get("storage") not in ("host", "dual"):
                errors.append(f"{area}: {name}: host_alloc, host_alloc_array and host_free are for host types only")
                continue
            calls = []
            alloc = t.get("host_alloc", [])
            if not isinstance(alloc, list) or not all(isinstance(a, int) for a in alloc):
                errors.append(f"{area}: {name}: host_alloc is a list of call return addresses")
            else:
                calls += alloc
            # An array allocated at once (new[] without a count header): the
            # call's size register, divided by the type's size for NewArray.
            arrays = t.get("host_alloc_array", [])
            if not isinstance(arrays, list):
                arrays = [None]
            for ar in arrays:
                if (not isinstance(ar, dict) or set(ar) != {"call", "reg"} or not isinstance(ar["call"], int)
                        or not isinstance(ar["reg"], str) or not re.fullmatch(r"r([3-9]|10)", ar["reg"])):
                    errors.append(f"{area}: {name}: host_alloc_array entry {ar!r}: "
                                  "{ call = <return address>, reg = <the size's register, \"r3\"..\"r10\"> }")
                    continue
                calls.append(ar["call"])
            frees = t.get("host_free", [])
            if not isinstance(frees, list):
                errors.append(f"{area}: {name}: host_free is a list of {{ call, reg, returns }}")
                frees = []
            for fr in frees:
                if (not isinstance(fr, dict) or not {"call", "reg"} <= set(fr) <= {"call", "reg", "returns"}
                        or not isinstance(fr["call"], int) or not isinstance(fr.get("returns", 0), int)
                        or not isinstance(fr["reg"], str) or not re.fullmatch(r"r([3-9]|10)", fr["reg"])):
                    errors.append(f"{area}: {name}: host_free entry {fr!r}: "
                                  "{ call = <return address>, reg = \"r3\"..\"r10\", returns = <int> (left out: r3 kept) }")
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
            if t.get("storage") not in ("host", "dual"):
                errors.append(f"{area}: {name}: storage {t.get('storage')!r} (guest, host or dual)")
                continue
            spans = []
            for f in t.get("fields", []):
                spans.append((f["offset"], f["offset"] + field_width(f, alltypes), f["name"]))
                to = alltypes.get(f.get("to"))
                if f.get("type") == "embed" and to and to.get("storage", "guest") not in ("host", "dual"):
                    errors.append(f"{area}: {name}.{f['name']}: embeds {f['to']}, a guest type, "
                                  "in a host type (its address is the owner's handle + offset)")
            # Overlapping fields are allowed: the struct keeps the guest
            # offsets, and a field over an earlier one is read at its offset
            # (host_type).
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
                if ty == "embed" and (f.get("to") not in types or not alltypes[f["to"]].get("size")):
                    errors.append(f"{area}: {name}.{fname} embeds {f.get('to')!r}: a known type with a size")
                if ty not in SCALARS and ty not in ("ptr", "bytes", "embed"):
                    errors.append(f"{area}: {name}.{fname}: an unknown type {ty!r}")
    return types, errors


def field_width(f, alltypes):
    """A field's width in bytes; an embed's is its type's size times its count."""
    if f.get("type") == "bytes":
        return f.get("count", 0)
    if f.get("type") == "embed":
        return alltypes.get(f.get("to"), {}).get("size", 0) * f.get("count", 1)
    return WIDTHS.get(f.get("type"), 0)


ALLTYPES = {}
TYPE_AREA = {}


def embed_accessors(f, view_address):
    """An embedded object's view: the owner's address (a guest address, or a
    handle, whose interior values name the object inside) plus its offset;
    an array of them takes an index."""
    to, off, n = f["to"], f["offset"], f["name"]
    # A template, as a pointer field's: the type may be defined further on.
    if f.get("count", 1) > 1:
        return [f"  template <typename T = {to}>",
                f"  T {n}(uint32_t i) const {{ return T(base, {view_address} + {off} + i * T::kSize); }}"]
    return [f"  template <typename T = {to}>",
            f"  T {n}() const {{ return T(base, {view_address} + {off}); }}"]


def accessors(fname, f, at, static):
    """The accessors of a field at the address expression `at`."""
    pre = "static " if static else ""
    cst = "" if static else " const"
    bp = "uint8_t* base" if static else ""
    sep = ", " if static else ""
    ty = f["type"]
    out = []
    if ty == "embed":
        out.extend(embed_accessors(f, "address"))
    elif ty == "ptr":
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


def members(t):
    """A host type's fields that are struct members: the others overlap one
    (a union, or a measure's `fault_` field) and are read at their offset.
    Real fields first, by offset; then `fault_` fields where they fit."""
    taken, chosen = [], set()
    # Scalars before bytes fields: a bytes field over scalars (a record kept
    # whole) would leave the scalars as unswapped overlays.
    for f in sorted(t.get("fields", []), key=lambda f: (f["name"].startswith("fault_"),
                                                         f["type"] == "bytes", f["offset"])):
        lo, hi = f["offset"], f["offset"] + field_width(f, ALLTYPES)
        if f["type"] == "embed" or any(lo < b and a < hi for a, b in taken):
            continue
        taken.append((lo, hi))
        chosen.add(f["name"])
    return chosen


def layout_fields(t, at=0, depth=0):
    """A host type's scalar fields wider than a byte, embedded objects'
    included (their own fields at the owner's offset), for copies between it
    and guest memory (memory/host_copy.h); a field over an earlier one is
    left out (the earlier one decides the bytes' order)."""
    out = []
    mem = members(t)
    for f in sorted(t.get("fields", []), key=lambda f: f["offset"]):
        if f["type"] != "embed" and f["name"] not in mem:
            continue
        if f["type"] == "embed" and depth < 8:
            inner = ALLTYPES.get(f["to"], {})
            for i in range(f.get("count", 1)):
                out += layout_fields(inner, at + f["offset"] + i * inner.get("size", 0), depth + 1)
        elif f["type"] in WIDTHS and WIDTHS[f["type"]] > 1:
            out.append((at + f["offset"], WIDTHS[f["type"]]))
    return sorted(out)


def host_type(name, t):
    """A host type (`storage = "host"`, docs/loops/e5-loop.md step 3) or a
    dual one (`storage = "dual"`, step 4): its members in `<name>Data`, laid
    out at the guest offsets (packed, the gaps padding, in host byte order
    but for bytes fields, which keep guest order), so a value inside a
    handle's span names the member there (memory::HandleObject) and an
    embedded object is a member where the guest code expects it. Its view
    holds a handle (a dual view: a handle or a guest address, told apart at
    each access) with the same accessors as a guest view's (`_address()` an
    interior handle on the host), plus `<field>_bits()` and `set_<field>_bits()` (the raw
    bits of the field's width, what lifted code loads and stores), `New`,
    `NewArray`, `at` and `Delete`."""
    dual = t.get("storage") == "dual"
    fields = sorted(t.get("fields", []), key=lambda f: f["offset"])
    size = max([t.get("size", 0)] + [f["offset"] + field_width(f, ALLTYPES) for f in fields])
    out = ["#pragma pack(push, 1)", f"struct {name}Data {{"]
    at = 0
    mem = members(t)
    overlaid = {f["name"] for f in fields if f["name"] not in mem and f["type"] != "embed"}
    # Embedded objects whose bytes no member covers get bytes of their own.
    cover = [f for f in fields if f["name"] in mem or
             (f["type"] == "embed" and not any(
                 g["name"] in mem and g["offset"] < f["offset"] + field_width(f, ALLTYPES)
                 and f["offset"] < g["offset"] + field_width(g, ALLTYPES) for g in fields))]
    for f in sorted(cover, key=lambda f: f["offset"]):
        if f["offset"] < at:
            continue  # an embed over another (left to its members' bytes)
        if f["offset"] > at:
            out.append(f"  uint8_t pad_{at}[{f['offset'] - at}] = {{}};")
        if f["type"] == "bytes":
            out.append(f"  uint8_t {f['name']}[{f['count']}] = {{}};  // +{f['offset']}, guest order")
        elif f["type"] == "embed":
            # The embedded type's members, reached through its own view (an
            # interior handle): bytes here, so headers need no order.
            out.append(f"  uint8_t {f['name']}[{field_width(f, ALLTYPES)}] = {{}};  "
                       f"// +{f['offset']}, {f.get('count', 1)} x {f['to']}")
        else:
            out.append(f"  {HOST[f['type']][0]} {f['name']} = 0;  // +{f['offset']}")
        at = f["offset"] + field_width(f, ALLTYPES)
    if size > at:
        out.append(f"  uint8_t pad_{at}[{size - at}] = {{}};")
    out.append("};")
    out.append("#pragma pack(pop)")
    out.append(f"static_assert(sizeof({name}Data) == {size});")
    lay = layout_fields(t)
    if lay:
        out.append(f"inline constexpr memory::LayoutField k{name}Fields[] = {{"
                   + ", ".join(f"{{{o}, {w}}}" for o, w in lay) + "};")
        out.append(f"inline constexpr memory::Layout k{name}Layout = {{k{name}Fields, {len(lay)}}};")
    else:
        out.append(f"inline constexpr memory::Layout k{name}Layout = {{nullptr, 0}};")
    out.append("")
    out.append(f"struct {name} : View {{")
    out.append("  using View::View;")
    out.append(f"  static constexpr uint32_t kSize = {t.get('size', 0)};")
    out.append(f"  static constexpr bool kHost = {'false' if dual else 'true'};")
    out.append(f"  static constexpr bool kDual = {'true' if dual else 'false'};")
    if "vtable" in t:
        out.append(f"  static constexpr uint32_t kVtable = 0x{t['vtable']:08X};")
    out.append(f"  {name}Data* data() const {{ return static_cast<{name}Data*>(memory::HandleObject(address)); }}")
    if dual:
        out.append("  bool on_host() const { return memory::IsHandle(address); }")
    out.append(f"  static {name} New(uint8_t* base) {{")
    out.append(f"    return {name}(base, memory::MakeHandle(new {name}Data(), sizeof({name}Data), \"{name}\", &k{name}Layout));")
    out.append("  }")
    # An array allocated at once (new[] without a count header, one block of
    # `count` objects): one handle; element i is the handle + i * kSize, and
    # copies and raw accesses use the element's layout at offset % kSize.
    out.append(f"  template <uint32_t Stride = kSize>")
    out.append(f"  static {name} NewArray(uint8_t* base, uint32_t count) {{")
    out.append(f"    static_assert(sizeof({name}Data) == Stride, \"an array's stride is the guest size\");")
    out.append(f"    auto* p = static_cast<{name}Data*>(::operator new(sizeof({name}Data) * std::max<uint32_t>(count, 1)));")
    out.append(f"    for (uint32_t i = 0; i < count; i++) new (p + i) {name}Data();")
    out.append(f"    return {name}(base, memory::MakeHandle(p, sizeof({name}Data) * count, \"{name}\", &k{name}Layout, sizeof({name}Data)));")
    out.append("  }")
    out.append(f"  {name} at(uint32_t i) const {{ return {name}(base, address + i * kSize); }}")
    # One object or an array's first: the block freed whole (0 does nothing).
    out.append(f"  void Delete() const {{ if (address) memory::FreeHandle(address); }}")

    # Any offset, any width (1, 2, 4 or 8 bytes), as the guest's bytes give it:
    # for accesses inside a field (a byte of a word) and code reading by offset.
    hl = f"memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), k{name}Layout, offset, width)"
    hs = f"memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), k{name}Layout, offset, width, v)"
    gl = ("width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? "
          "guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) "
          ": guest::LoadU64(base, address + offset)")
    gs = ("if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) "
          "guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) "
          "guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v);")
    if dual:
        out.append(f"  uint64_t load_at(uint32_t offset, uint32_t width) const {{ if (on_host()) return {hl}; return {gl}; }}")
        out.append(f"  void store_at(uint32_t offset, uint32_t width, uint64_t v) const {{ if (on_host()) {{ {hs}; return; }} {gs} }}")
    else:
        out.append(f"  uint64_t load_at(uint32_t offset, uint32_t width) const {{ return {hl}; }}")
        out.append(f"  void store_at(uint32_t offset, uint32_t width, uint64_t v) const {{ {hs}; }}")

    def pick(host, guest):
        return f"on_host() ? {host} : {guest}" if dual else host

    for f in t.get("fields", []):
        n, ty, off = f["name"], f["type"], f["offset"]
        if f.get("note"):
            out.append(f"  // {f['note']}")
        if ty == "embed":
            out.extend(embed_accessors(f, "address"))
            continue
        # The field's address: a guest address, or an interior handle (the
        # handle + offset), which every guest load and store resolves
        # (memory::RawHandleLoad and Store; logged once per type and offset).
        out.append(f"  uint32_t {n}_address() const {{ return address + {off}; }}")
        if ty == "bytes":
            # At its offset (a member, or bytes over scalar members).
            out.append(f"  uint8_t* {n}() const {{ return "
                       + pick(f"reinterpret_cast<uint8_t*>(data()) + {off}",
                              f"guest::Pointer(base, address + {off})") + "; }")
            continue
        member, bits = HOST[ty]
        if n in overlaid:
            # A field over an earlier one (a union, or a measure fault): its
            # bytes at its offset, in host order.
            # Its value as the guest's bytes give it (memory::LoadOverlay).
            p = "reinterpret_cast<uint8_t*>(data())"
            w = {1: "U8", 2: "U16", 4: "U32", 8: "U64"}[WIDTHS[ty]]
            hget = f"{bits}(memory::LoadOverlay({p}, k{name}Layout, {off}, {WIDTHS[ty]}))"
            gbits = f"guest::Load{w}(base, address + {off})"
            getbits = f"on_host() ? {hget} : {gbits}" if dual else hget
            out.append(f"  {bits} {n}_bits() const {{ return {getbits}; }}")
            hput = f"memory::StoreOverlay({p}, k{name}Layout, {off}, {WIDTHS[ty]}, v)"
            gput = f"guest::Store{w}(base, address + {off}, v)"
            putbits = f"if (on_host()) {hput}; else {gput};" if dual else f"{hput};"
            out.append(f"  void set_{n}_bits({bits} v) const {{ {putbits} }}")
            if ty == "ptr":
                out.append(f"  View {n}() const {{ return View(base, {n}_bits()); }}")
                out.append(f"  void set_{n}(uint32_t v) const {{ set_{n}_bits(v); }}")
            else:
                cv = f"std::bit_cast<{member}>({n}_bits())" if member != bits and ty in ("f32", "f64") else f"{member}({n}_bits())"
                out.append(f"  {member} {n}() const {{ return {cv}; }}")
                sv = f"std::bit_cast<{bits}>(v)" if ty in ("f32", "f64") else f"{bits}(v)"
                out.append(f"  void set_{n}({member} v) const {{ set_{n}_bits({sv}); }}")
            continue
        if ty in SCALARS:
            ctype, gload, gstore = SCALARS[ty]
        else:  # ptr
            ctype, gload, gstore = SCALARS["u32"]
        ga = f"address + {off}"
        gl = gload.format(a=ga)
        gs = gstore.format(a=ga)
        if dual:
            get = f"return on_host() ? {member}(data()->{n}) : {member}({gl});"
            put = f"if (on_host()) data()->{n} = v; else {gs};"
        else:
            get = f"return data()->{n};"
            put = f"data()->{n} = v;"
        if ty == "ptr":
            to = f.get("to")
            pget = get.replace("return ", "return T(base, ").rstrip(";") + ");" if not dual else \
                f"return T(base, on_host() ? data()->{n} : {gl});"
            if to:
                out.append(f"  template <typename T = {to}>")
                out.append(f"  T {n}() const {{ {pget} }}")
                out.append(f"  void set_{n}(const View& w) const {{ const uint32_t v = w.address; {put} }}")
            else:
                vget = pget.replace("T(base", "View(base")
                out.append(f"  View {n}() const {{ {vget} }}")
            out.append(f"  void set_{n}(uint32_t v) const {{ {put} }}")
        else:
            out.append(f"  {member} {n}() const {{ {get} }}")
            out.append(f"  void set_{n}({member} v) const {{ {put} }}")
        # The raw bits of the field's width, what lifted code loads and stores.
        if member == bits:
            bget, bput = f"data()->{n}", f"data()->{n} = v"
        else:
            bget, bput = f"std::bit_cast<{bits}>(data()->{n})", f"data()->{n} = std::bit_cast<{member}>(v)"
        if dual:
            w = {1: "U8", 2: "U16", 4: "U32", 8: "U64"}[WIDTHS[ty]]
            out.append(f"  {bits} {n}_bits() const {{ return on_host() ? {bget} : guest::Load{w}(base, {ga}); }}")
            out.append(f"  void set_{n}_bits({bits} v) const {{ if (on_host()) {bput}; else guest::Store{w}(base, {ga}, v); }}")
        else:
            out.append(f"  {bits} {n}_bits() const {{ return {bget}; }}")
            out.append(f"  void set_{n}_bits({bits} v) const {{ {bput}; }}")
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
        if t.get("storage") in ("host", "dual"):
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
    # The areas whose types this one embeds: their views, for the embed
    # accessors (templates, instantiated at use: at the end, so a cycle of
    # areas resolves under #pragma once).
    owners = set()
    for name, t in data.items():
        if name == "globals":
            continue
        for f in t.get("fields", []):
            if f.get("type") == "embed" and f.get("to") in TYPE_AREA and TYPE_AREA[f["to"]] != area:
                owners.add(TYPE_AREA[f["to"]])
    for a in sorted(owners):
        lines.append(f'#include "schema/{a.replace("-", "_")}.h"')
    if owners:
        lines.append("")
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
        TYPE_AREA.update({n: a for a, d in areas.items() for n in d if n != "globals"})
        ALLTYPES.update({n: t for d in areas.values() for n, t in d.items() if n != "globals"})
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
    TYPE_AREA.update({n: a for a, d in areas.items() for n in d if n != "globals"})
    ALLTYPES.update({n: t for d in areas.values() for n, t in d.items() if n != "globals"})
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
