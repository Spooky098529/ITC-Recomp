"""Writes the shader tables in the docs from shaders.json (port_all.py's
catalog).

    python write_doc.py catalog <folder with shaders.json>   > (docs/shaders.md's catalog)
    python write_doc.py parts <folder with shaders.json>     > (docs/part_shaders.md's table)"""
import json, sys

KNOWN = {
    "7EB16456AAD56CBA": "Motion blur", "E0D070FB58A4EA03": "Glow history cross-fade",
    "9750599DF5ED3B39": "Bloom combine", "61DB6B3F91E0ED87": "Blur (sample offsets and weights)",
    "D8928E2407A68256": "Depth of field", "C5EDA6DC536A2095": "Final color (gamma, HSV, calibration)",
    "6E9AB15C8DAAAB83": "Lighting",
}


def constants(e, with_registers=False):
    out = []
    for c in e["constants"]:
        if c["set"] == "sampler":
            continue
        if with_registers:
            kind = {"float4": "c", "bool": "b", "int4": "i"}.get(c["set"], "?")
            reg = "%s%d" % (kind, c["register"]) if c["count"] == 1 else \
                "%s%d-%d" % (kind, c["register"], c["register"] + c["count"] - 1)
            out.append("`%s` %s" % (c["name"], reg))
        else:
            out.append("`%s`" % c["name"])
    return ", ".join(out)


def textures(e):
    names = {c["register"]: c["name"] for c in e["constants"] if c["set"] == "sampler"}
    seen = []
    for t in e.get("textures", []):
        if t["signed"]:
            continue
        name = names.get(t["fetch_constant"])
        seen.append("%d%s%s" % (t["fetch_constant"], " `%s`" % name if name else "",
                                " (cube)" if t["view"] == "cube" else ""))
    return ", ".join(seen)


def where(e):
    found = list(e["found_in"])
    more = e["found_count"] - len(found)
    return ", ".join(f.replace(".xpu", "") for f in found) + (" and %d more" % more if more > 0 else "")


def catalog(entries):
    print("| Shader | Found in | Constants | Textures | Notes |")
    print("|---|---|---|---|---|")
    for e in sorted(entries, key=lambda e: (e["stage"], e["found_in"][0])):
        notes = [KNOWN[e["hash"]]] if e["hash"] in KNOWN else []
        if e["part_id"]:
            notes.append("part shader `%s`" % e["part_id"])
        if e.get("loops"):
            notes.append("loops")
        if e["literals"]:
            notes.append("%d literal constants" % len(e["literals"]))
        if e["stage"] == "vs":
            notes.append("%d vertex fetches; layout-free hash `%s`" % (
                len(e.get("vertex_fetches", [])), e.get("lookup_hash", "")))
        if not e.get("port"):
            notes.append("not ported: %s" % e.get("not_ported"))
        print("| `%s_%s` | %s | %s | %s | %s |" % (e["stage"], e["hash"], where(e), constants(e),
                                                  textures(e), "; ".join(notes)))


def parts(entries):
    print("| Id | Shader | Microcode words | Constants | Textures | Interpolators |")
    print("|---|---|---|---|---|---|")
    for e in sorted((e for e in entries if e["part_id"]), key=lambda e: e["part_id"]):
        interpolators = e.get("interpolators", [])
        if e["stage"] == "ps":
            interpolators = "r0-r%d" % (len(interpolators) - 1) if interpolators else "none"
        else:
            interpolators = ", ".join("o%d" % i for i in interpolators)
        print("| `%s` | `%s_%s` | %d | %s | %s | %s |" % (
            e["part_id"], e["stage"], e["hash"], e["microcode_words"], constants(e, True),
            textures(e), interpolators))


def main():
    entries = json.load(open(sys.argv[2] + "/shaders.json"))
    {"catalog": catalog, "parts": parts}[sys.argv[1]](entries)


if __name__ == "__main__":
    main()
