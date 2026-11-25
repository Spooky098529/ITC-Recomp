#!/usr/bin/env python3
"""Checks the schema (itc_recomp/schema/*.toml, docs/e5-native.md, step 2)
against the game's code: every load and store the measure (tools/type_map.py)
attributes to a type the schema describes must be a field of it, at its
offset and width. It reads the original instructions of every function the
static map reaches (hand-ported ones too: their originals are the game's),
so a field the schema gets wrong, or one it lacks, shows up whatever code
runs it today.

Also listed: the schema's types the measure never saw (a `measure` name to
fix), and with -v, each type's fields no access reaches.

usage: schema_check.py [-v] [area ...]    (exit 1 when an access fits no field)
"""
import collections
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import schema_lib  # noqa: E402
import type_map  # noqa: E402


def main(argv):
    verbose = "-v" in argv
    areas = {a.removesuffix(".toml").removesuffix(".md") for a in argv[1:] if a != "-v"}
    schema = schema_lib.load()
    accesses, m = type_map.typed_accesses()
    misses = collections.defaultdict(collections.Counter)
    seen_fields = collections.defaultdict(set)
    seen_types = set()
    for (fn, insn), (t, off, disp, width, store) in accesses.items():
        ty = schema.of_measure(t)
        if ty is None or (areas and ty.area not in areas) or off is None:
            continue
        seen_types.add(ty.name)
        at = off + disp
        f = ty.field_at(at, width)
        if f is None:
            name = m.names.get(fn, fn)
            misses[ty.name][(at, width, "store" if store else "load")] += 1
            misses[ty.name][("fn", name)] += 0
        else:
            seen_fields[ty.name].add(f["name"])
    bad = 0
    for name in sorted(misses):
        c = misses[name]
        print(f"{name} ({schema.types[name].area}):")
        for key, n in sorted(((k, v) for k, v in c.items() if k[0] != "fn"), key=lambda kv: kv[0]):
            at, width, kind = key
            print(f"  +{at}: {n} {kind}s {width} bytes wide fit no field")
            bad += n
        fns = sorted(k[1] for k in c if k[0] == "fn")
        print(f"  in {', '.join(fns[:8])}{' ...' if len(fns) > 8 else ''}")
    for name, ty in sorted(schema.types.items()):
        if areas and ty.area not in areas:
            continue
        if name not in seen_types:
            print(f"{name} ({ty.area}): the measure never saw it (measure names: "
                  f"{', '.join(ty.measures) or name})")
        elif verbose:
            unused = [f["name"] for f in ty.fields if f["name"] not in seen_fields[name]]
            if unused:
                print(f"{name}: no access reaches {', '.join(unused)}")
    print(f"schema_check: {len(schema.types)} types, {bad} accesses fit no field")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
