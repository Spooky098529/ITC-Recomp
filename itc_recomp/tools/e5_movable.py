#!/usr/bin/env python3
"""E5 step 3's pick list (docs/loops/e5-loop.md, "Step 3"): the schema's
types that can move to host memory by themselves, by area.

A schema type (itc_recomp/schema/*.toml) is movable when, over every
measure name it claims (tools/type_map.py):
  - one area's functions touch it (private);
  - it isn't read from a file in place, read by the GPU or saved (the
    measure's tags), and isn't the image's data (origin "image");
  - the measure saw it allocated (an allocation site to make the host object
    at); an object embedded in another moves with its owner (step 4);
Each movable type lists its allocation sites and the functions touching it
that also make accesses the measure couldn't type (read those first: a
raw access reaching the moved object faults on its handle).

The areas come in the measure's order of fewest ties to other areas first
(coverage/type_map_areas.tsv's shared weight).

usage: e5_movable.py [area]   (writes coverage/e5_movable.tsv, prints the list;
                               with an area, only its types, in detail)
"""
import collections
import csv
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import schema_lib  # noqa: E402
import type_map  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(HERE))
COVERAGE = os.path.join(ROOT, "itc_recomp", "coverage")
BLOCKING_TAGS = {"file", "gpu", "save"}
# Types whose code play never reaches (left out of the pick list, the
# lead's call after step 3's batches, 2026-11-22): the multiplayer modes are
# unreachable (--multiplayer_menu_items, off), and the game has no System
# Link mode.
UNREACHABLE = {
    "ModeType8": "the type 8 (two-player) mode",
    "SysLinkIdSetNode": "System Link",
}


SCHEMA_TYPES = {}


def overlaps(ty):
    spans = []
    for f in ty.fields:
        width = ty.width(f, SCHEMA_TYPES)
        spans.append((f["offset"], f["offset"] + width))
    spans.sort()
    return any(b0 < a1 for (a0, a1), (b0, b1) in zip(spans, spans[1:]))


def main(argv):
    only = argv[1].removesuffix(".md") if len(argv) > 1 else None
    _, m = type_map.typed_accesses()
    sch = schema_lib.load()
    SCHEMA_TYPES.update(sch.types)
    # The measure's types by display name, with what each holds.
    roots = collections.defaultdict(lambda: {"areas": collections.Counter(), "tags": set(),
                                             "allocs": [], "touch": set()})
    for t, fns in m.touch.items():
        d = m.display(t)
        r = roots[d]
        for fn in fns:
            r["areas"][m.area.get(fn, "?")] += 1
        r["touch"] |= fns
    for t, sites in m.allocs.items():
        roots[m.display(t)]["allocs"].extend(sites)
    with open(os.path.join(COVERAGE, "type_map.tsv")) as f:
        for row in csv.DictReader(f, delimiter="\t"):
            if row["tags"]:
                roots[row["type"]]["tags"] |= set(row["tags"].split(","))
    weight = {}
    with open(os.path.join(COVERAGE, "type_map_areas.tsv")) as f:
        for row in csv.DictReader(f, delimiter="\t"):
            weight[row["area"].removesuffix(".md")] = int(row["shared_weight"] or 0)

    rows = []
    for name, ty in sorted(sch.types.items()):
        areas, tags, allocs, touch = collections.Counter(), set(), [], set()
        for meas in ty.measures or [name]:
            r = roots.get(meas)
            if not r:
                continue
            areas += r["areas"]
            tags |= r["tags"]
            allocs += r["allocs"]
            touch |= r["touch"]
        if not touch:
            continue
        origin = getattr(ty, "origin", None)
        reasons = []
        owners = sorted(a.removesuffix(".md") for a in areas if a != "?")
        if len(owners) != 1:
            reasons.append(f"shared ({len(owners)} areas)")
        if tags & BLOCKING_TAGS:
            reasons.append("tags " + ",".join(sorted(tags & BLOCKING_TAGS)))
        if not allocs:
            reasons.append("no allocation seen")
        # Overlapping fields no longer stop a move (step 4: host structs keep
        # the guest offsets; a field over another is read at its offset).
        if name in UNREACHABLE:
            reasons.append(f"unreachable ({UNREACHABLE[name]})")
        risky = sorted(fn for fn in touch if m.unattributed.get(fn))
        rows.append({
            "type": name, "area": ty.area, "movable": "yes" if not reasons else "no",
            "why_not": "; ".join(reasons), "size": ty.size,
            "alloc_sites": " ".join(sorted({m.display_name(fn) for fn, _ in allocs})),
            "functions": len(touch),
            "untyped_functions": " ".join(m.display_name(fn) for fn in risky),
        })
    with open(os.path.join(COVERAGE, "e5_movable.tsv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]), delimiter="\t")
        w.writeheader()
        w.writerows(rows)

    movable = [r for r in rows if r["movable"] == "yes"]
    by_area = collections.defaultdict(list)
    for r in movable:
        by_area[r["area"]].append(r)
    print(f"schema types touched: {len(rows)}; movable now: {len(movable)} in {len(by_area)} areas")
    why = collections.Counter(x for r in rows if r["movable"] == "no" for x in
                              [w.split(" (")[0] for w in r["why_not"].split("; ")])
    print("not movable now: " + ", ".join(f"{k} {v}" for k, v in why.most_common()))
    for area in sorted(by_area, key=lambda a: (weight.get(a, 10**9), a)):
        if only and area != only:
            continue
        rs = by_area[area]
        print(f"{area} (shared weight {weight.get(area, '?')}): "
              + ", ".join(f"{r['type']} ({r['size']} B)" for r in rs))
        if only:
            for r in rs:
                print(f"  {r['type']}: {r['functions']} functions; allocated in {r['alloc_sites'] or '-'}")
                if r["untyped_functions"]:
                    print(f"    with untyped accesses: {r['untyped_functions']}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
