"""The schema (itc_recomp/schema/*.toml, docs/e5-native.md, step 2) for the
tools: its types by name and by the measure's name (tools/type_map.py), and
their fields by offset."""
import glob
import os
import tomllib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCHEMA = os.path.join(ROOT, "schema")

WIDTHS = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "fn": 4, "ptr": 4,
          "f32": 4, "u64": 8, "f64": 8}


class Type:
    def __init__(self, area, name, data):
        self.area = area
        self.name = name
        self.measure = data.get("measure")
        self.measures = ([self.measure] if self.measure else []) + data.get("measures", [])
        self.size = data.get("size", 0)
        self.storage = data.get("storage", "guest")
        self.fields = data.get("fields", [])
        # Moved to host memory (step 3: "host"), or with instances in both
        # shapes (step 4: "dual", the view telling them apart by the handle).
        self.host = self.storage in ("host", "dual")
        self.data = data
        self.by_offset = {}
        for f in self.fields:
            self.by_offset.setdefault(f["offset"], []).append(f)

    def header(self):
        return "schema/" + self.area.replace("-", "_") + ".h"

    def width(self, f, types=None):
        """A field's width in bytes (an embed: its type's size times its count)."""
        if f["type"] == "bytes":
            return f.get("count", 0)
        if f["type"] == "embed":
            to = (types or {}).get(f.get("to"))
            return (to.size if to else 0) * f.get("count", 1)
        return WIDTHS.get(f["type"], 0)

    def field_at(self, offset, width):
        """The field at `offset` read or written `width` bytes wide: one of
        that width, else None (a byte of a bytes field counts)."""
        for f in self.by_offset.get(offset, []):
            if WIDTHS.get(f["type"]) == width:
                return f
        for f in self.fields:
            if f["type"] == "bytes" and f["offset"] <= offset < f["offset"] + f.get("count", 0):
                return f
        return None


class Schema:
    def __init__(self):
        self.types = {}
        self.by_measure = {}
        for path in sorted(glob.glob(os.path.join(SCHEMA, "*.toml"))):
            area = os.path.basename(path)[:-5]
            try:
                with open(path, "rb") as f:
                    data = tomllib.load(f)
            except tomllib.TOMLDecodeError as e:
                print(f"{area}: skipped (doesn't parse: {e})")
                continue
            for name, t in data.items():
                if name == "globals":
                    continue
                ty = Type(area, name, t)
                self.types[name] = ty
        for ty in self.types.values():
            for m in ty.measures or [ty.name]:
                self.by_measure[m] = ty
        # Lifted allocation and free sites of host types (schema_gen.py
        # checks their form), by the call's return address:
        # ("alloc", type) or ("free", type, register, returned value).
        self.host_sites = {}
        for ty in self.types.values():
            if not ty.host:
                continue
            for a in ty.data.get("host_alloc", []):
                self.host_sites[a] = ("alloc", ty)
            for fr in ty.data.get("host_free", []):
                self.host_sites[fr["call"]] = ("free", ty, fr["reg"], fr.get("returns"))

    def of_measure(self, measure_name):
        return self.by_measure.get(measure_name)


def load():
    return Schema()
