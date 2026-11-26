"""Ports every game shader it finds and writes the GLSL, a catalog and the
renderer's table of ports.

    python port_all.py --out <glsl folder> --table <header> [--check <glslangValidator>]
                       <inputs...>

Inputs are shader files (.xpu), C headers of shader files (compiled_shaders.h),
part files (.part, whose shader sections
hold the cars' and course objects' shaders) and folders of either. Each
shader is written once, as <stage>_<hash>.vert or .frag, where the hash is
the XXH3-64 of its microcode (as the renderer and the SDK name shaders).

--table writes the C++ table the renderer looks ports up in
(src/native/game_shader_ports.h); compile.sh compiles the GLSL it names.
shaders.json in the output folder lists every shader: where it was found,
its constants, textures and interpolators (docs/shaders.md and
docs/part_shaders.md are written from it by write_doc.py)."""
import argparse, glob, hashlib, json, os, re, struct, subprocess, sys
import xxhash
from xenos_ucode import parse_file
from ucode_to_glsl import Port, Unsupported

HERE = os.path.dirname(os.path.abspath(__file__))

# The part files' shaders by the mod tool's short ids (sbxr_tools'
# sbx_formats/shaders.py), keyed by the first 16 hex digits of the SHA-1 of
# the microcode.
PART_SHADER_IDS = {
    "48439039777d64bf": "vs_00", "b588e1e3ebbe7153": "vs_01", "11af1467d4625931": "vs_02",
    "22d03b7ac226f511": "vs_03", "326929f21ea15378": "vs_04", "1715fed6bae0f464": "vs_05",
    "5eeb3870970de693": "vs_06", "73ed0c4b5395015d": "vs_07", "8f69e6f05f9194e8": "vs_08",
    "67a7abd402f8651c": "vs_09", "0187624dac8d63a9": "vs_10",
    "726f9680a380465d": "ps_00", "7c9395c88b13e506": "ps_01", "bac6ef2d132bfa50": "ps_02",
    "2e86071cec3eefcf": "ps_03", "1d6d7ff0df1998fa": "ps_04", "9a07f93a46cf3503": "ps_05",
    "2cbec30a512a8e95": "ps_06", "0cf4d49cf8ee62bf": "ps_07", "12dd1294caaff2da": "ps_08",
    "1cdea8fcdb4ff9f8": "ps_09", "dd82887ab5efc12c": "ps_10", "69eb0eab5e84cbab": "ps_11",
    "5c084ba8d1529ee1": "ps_12", "8ca17c86e8250e68": "ps_13", "8d2c32389936edc6": "ps_14",
    "3128c3aa698e0847": "ps_15", "27fcd1e65c5b9279": "ps_16", "0362b10ec9572f3e": "ps_17",
    "d245f50399c46e52": "ps_18", "08c38a5c71b568cd": "ps_19", "49c35b2c7e32eeb3": "ps_20",
    "288456f26cbb114d": "ps_21", "7e98c0ddbcdc01c4": "ps_22", "e12ffae501fe1117": "ps_23",
    "f95e96eca4cf7818": "ps_24", "03b87e855809c150": "ps_25", "52541ea75fe7ce80": "ps_26",
    "e2385bcd640ac8b0": "ps_27",
}


def microcode_bytes(words):
    return struct.pack(">%dI" % len(words), *words)


def microcode_hash(words):
    return xxhash.xxh3_64(microcode_bytes(words)).intdigest()


def layout_free_hash(port):
    """A vertex shader's hash with what the game patches for the vertex
    declaration zeroed: each vertex fetch's fetch constant, format fields,
    stride and offset."""
    words = list(port.s.words)
    for f in port.vfetches:
        a = f.address * 3
        words[a] &= ~(0x7F << 20)
        words[a + 1] &= ~(0x3FFFF << 12)
        words[a + 2] = 0
    return microcode_hash(words)


def part_shaders(data):
    """The shaders in a part file: (offset, file bytes) for each shader
    header (4-aligned magic, then a header and microcode that fit)."""
    at = 0
    while True:
        at = data.find(b"\x10\x2a\x0e", at)
        if at < 0:
            return
        if at % 4 == 0 and data[at + 3] in (0, 1) and at + 24 <= len(data):
            header_size, ucode_size = struct.unpack(">II", data[at + 4:at + 12])
            if 0 < header_size < 0x4000 and 0 < ucode_size < 0x10000 and \
                    at + header_size + ucode_size <= len(data):
                yield at, data[at:at + header_size + ucode_size]
        at += 4


def sources(inputs):
    """(name, where, shader file bytes, part shader id) for every shader."""
    files = []
    for path in inputs:
        if os.path.isdir(path):
            files += sorted(glob.glob(os.path.join(path, "*.xpu")))
            files += sorted(glob.glob(os.path.join(path, "*.part")))
        else:
            files.append(path)
    for f in files:
        base = os.path.basename(f)
        data = open(f, "rb").read()
        if f.endswith(".h"):
            # C arrays of shader files (src/graphics/compiled_shaders.h: the
            # shaders the game compiles at boot).
            for name, body in re.findall(r"uint8_t (k\w+)\[\d*\] = \{(.*?)\};", data.decode("latin-1"), re.S):
                yield name, name, bytes(int(x, 16) for x in re.findall(r"0x([0-9A-Fa-f]{2})", body)), ""
        elif f.endswith(".part"):
            for at, shader in part_shaders(data):
                header_size, ucode_size = struct.unpack(">II", shader[4:12])
                sha = hashlib.sha1(shader[header_size:header_size + ucode_size]).hexdigest()[:16]
                yield base, "%s at 0x%X" % (base, at), shader, PART_SHADER_IDS.get(sha, "")
        else:
            yield base, base, data, ""


def table_entry(e, port):
    stage = "kVertex" if port.vertex else "kPixel"
    lines = ["  {ShaderStage::%s, 0x%016Xull, %d, k_game_%s, std::size(k_game_%s), \"%s\"," % (
        stage, e["lookup_hash"], len(port.s.words), e["name"], e["name"], e["title"])]
    lines.append("   {{%s}}, %d," % (", ".join(str(f.address) for f in port.vfetches), len(port.vfetches)))
    lines.append("   {{%s}}, %d," % (", ".join("{%d, %s, %s}" % (fc, "true" if dim == 3 else "false",
                                                                "true" if signed else "false")
                                             for fc, dim, signed in port.textures), len(port.textures)))
    lines.append("   {{%s}}, %d}," % (", ".join("{%d, %d, %d, %d, %d}" % s for s in port.samplers),
                                    len(port.samplers)))
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("inputs", nargs="+")
    ap.add_argument("--out", required=True, help="the folder for the GLSL and shaders.json")
    ap.add_argument("--table", help="the renderer's table of ports to write")
    ap.add_argument("--check", help="glslangValidator, to compile each port")
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)
    interface = open(os.path.join(HERE, "xe_interface.glsl")).read()
    open(os.path.join(args.out, "xe_interface.glsl"), "w", newline="\n").write(interface)
    seen, catalog, problems, ports = {}, [], [], []
    for name, where, data, part_id in sources(args.inputs):
        try:
            shader = parse_file(data)
        except ValueError:
            continue
        h = microcode_hash(shader.words)
        if h in seen:
            entry = seen[h]
            if part_id and not entry["part_id"]:
                entry["part_id"] = part_id
            if len(entry["found_in"]) < 8:
                entry["found_in"].append(where)
            entry["found_count"] += 1
            continue
        stage = "vs" if shader.is_vertex else "ps"
        entry = {"hash": "%016X" % h, "stage": stage, "found_in": [where], "found_count": 1,
                 "part_id": part_id, "target": shader.target,
                 "constants": [{"name": c.name, "set": c.register_set, "register": c.register,
                                "count": c.count} for c in shader.constants],
                 "literals": {str(r): list(v) for r, v in sorted(shader.literals.items())},
                 "microcode_words": len(shader.words)}
        seen[h] = entry
        catalog.append(entry)
        title = "%s shader %016X%s, first found in %s" % (
            "Vertex" if shader.is_vertex else "Pixel", h, " (%s)" % part_id if part_id else "", where)
        try:
            port = Port(shader, title)
            text = port.glsl(title)
        except Unsupported as e:
            entry["port"] = None
            entry["not_ported"] = str(e)
            problems.append((entry["hash"], "not ported: %s" % e))
            continue
        file = "%s_%016X.%s" % (stage, h, "vert" if shader.is_vertex else "frag")
        open(os.path.join(args.out, file), "w", newline="\n").write(text)
        entry.update({
            "port": file,
            "registers": port.register_count,
            "float_constants": sorted(port.float_used),
            "relative_constants": port.dynamic_constants,
            "textures": [{"fetch_constant": fc, "view": "cube" if d == 3 else "2D array",
                          "signed": bool(s)} for fc, d, s in port.textures],
            "samplers": [{"fetch_constant": s[0], "mag": s[1], "min": s[2], "mip": s[3],
                          "aniso": s[4]} for s in port.samplers],
            "vertex_fetches": [{"instruction": f.address, "fetch_constant": f.fetch_constant,
                                "register": f.dest} for f in port.vfetches],
            "interpolators": sorted(e for e in port.exports if e < 16) if shader.is_vertex
            else list(range(min(port.register_count, 16))),
            "color_targets": [] if shader.is_vertex else sorted(port.color_targets),
            "loops": bool(port.labels),
        })
        entry["lookup_hash"] = layout_free_hash(port) if shader.is_vertex else h
        entry["name"] = "%s_%016X" % (stage, h)
        entry["title"] = (part_id + " " if part_id else "") + where.replace("\\", "/")
        ports.append((entry, port))
        if args.check:
            r = subprocess.run([args.check, "-V", "--quiet", "--target-env", "vulkan1.2",
                                "-o", os.devnull, os.path.join(args.out, file)],
                               capture_output=True, text=True)
            entry["compiles"] = r.returncode == 0
            if r.returncode:
                problems.append((entry["hash"], (r.stdout + r.stderr).strip()[:300]))
    if args.table:
        L = ["// Generated by tools/shader_port/port_all.py; don't edit. The game's shaders",
             "// ported to GLSL (src/native/glsl/game), for ShaderCache (shaders.cpp): see",
             "// docs/shaders.md.",
             "#pragma once", "", "#include <iterator>", "", '#include "generated_game_shaders.h"',
             '#include "shader_ports.h"', "", "namespace native {", "",
             "inline constexpr GamePort kGamePorts[] = {"]
        for e, port in sorted(ports, key=lambda x: (x[1].vertex, x[0]["hash"])):
            L.append(table_entry(e, port))
        L += ["};", "", "}  // namespace native", ""]
        open(args.table, "w", newline="\n").write("\n".join(L))
    for e in catalog:
        e.pop("name", None)
        e.pop("title", None)
        if "lookup_hash" in e:
            e["lookup_hash"] = "%016X" % e["lookup_hash"]
    json.dump(catalog, open(os.path.join(args.out, "shaders.json"), "w", newline="\n"), indent=1)
    print("%d shaders (%d vertex, %d pixel), %d ported" % (
        len(catalog), sum(e["stage"] == "vs" for e in catalog),
        sum(e["stage"] == "ps" for e in catalog), len(ports)))
    for h, why in problems:
        print(" ", h, why)
    found = {e["part_id"] for e in catalog if e["part_id"]}
    missing = sorted(set(PART_SHADER_IDS.values()) - found)
    if missing:
        print("part shaders not found in the inputs:", " ".join(missing))


if __name__ == "__main__":
    main()
