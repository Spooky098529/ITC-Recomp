"""Compiles the game's shaders into ../generated_game_shaders.h, checked in so
the build doesn't need a GLSL compiler. Run from this directory after
porting or after changing a shader:

    py compile_game.py [--emit <folder>]

Each game shader is a port in game/ (made by tools/shader_port), unless a
variants.txt lists it (in materials/, post/, boot/ or vertex/): then it's made from
a hand-written file in that folder with the defines on its line, and the
port stays as the reference. A hand-written shader gets a header from game/shaders.json: its
descriptors where the translator has them, its constants by their D3D
names, and its textures by their sampler names.

--emit writes every shader's full source to a folder (with the files they
include), for tools/shader_port/check to compile and test.

Needs glslangValidator and spirv-opt (both in the Vulkan SDK). The SPIR-V
isn't optimized (the driver does that; inlining every helper made the
modules several times bigger), only stripped of its debug names and unused
functions. tools/shader_port/check compiles them the same way."""
import argparse
import glob
import json
import os
import shutil
import struct
import subprocess
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = os.path.join(HERE, "game")
# The folders of hand-written shaders. Their files are compiled side by
# side, so a file name is used once across them.
FOLDERS = ["materials", "post", "boot", "vertex"]
INCLUDES = [os.path.join(GAME, "xe_interface.glsl")]
for _folder in FOLDERS:
    for _pattern in ("*.glsl", "*.frag", "*.vert"):
        INCLUDES += sorted(glob.glob(os.path.join(HERE, _folder, _pattern)))


def read_variants():
    """Shader name -> (folder/file, defines)."""
    variants = {}
    for folder in FOLDERS:
        path = os.path.join(HERE, folder, "variants.txt")
        if not os.path.exists(path):
            continue
        for line in open(path):
            line = line.split("#")[0].split()
            if line:
                if line[0] in variants:
                    raise SystemExit("%s is listed twice in variants.txt" % line[0])
                variants[line[0]] = ("%s/%s" % (folder, line[1]), line[2:])
    return variants


def header(info, source, defines):
    """The lines in front of a hand-written shader: the interface, the
    constants and textures by name, and the variant's defines."""
    pixel = info["stage"] == "ps"
    lines = ["#version 450",
             "#extension GL_GOOGLE_include_directive : require",
             "// Shader %s (%s), made from %s by compile_game.py." % (
                 info["hash"], info["part_id"] or "game", source),
             "#define XE_PIXEL" if pixel else "#define XE_VERTEX"]
    # Float constants: in register order, all 256 when the shader indexes
    # them by the loop counter (as the translator packs them).
    packed = list(range(256)) if info["relative_constants"] else info["float_constants"]
    lines.append("#define XE_FLOAT_CONSTANTS %d" % max(len(packed), 1))
    lines.append('#include "xe_interface.glsl"')
    textures = {}  # fetch constant -> sampler name
    aliases = []  # other sampler names for the same fetch constant
    for c in info["constants"]:
        if c["set"] == "float4":
            span = list(range(c["register"], c["register"] + c["count"]))
            used = [r for r in span if r in packed]
            if not used:
                continue  # declared but unused
            lines.append("#define %s_COUNT %d" % (c["name"], c["count"]))
            if c["count"] == 1:
                lines.append("#define %s xe_fc[%d]  // c%d" % (
                    c["name"], packed.index(c["register"]), c["register"]))
            elif used == span and packed[packed.index(span[0]):][:len(span)] == span:
                lines.append("#define %s(i) xe_fc[%d + (i)]  // c%d-c%d" % (
                    c["name"], packed.index(span[0]), span[0], span[-1]))
            else:
                # Only some elements are used: a table of where each is (0 for
                # the unused ones, which the shader doesn't read).
                slots = [packed.index(r) if r in packed else 0 for r in span]
                lines.append("const int %s_slot[%d] = int[](%s);  // c%d-c%d, some unused" % (
                    c["name"], len(span), ", ".join(map(str, slots)), span[0], span[-1]))
                lines.append("#define %s(i) xe_fc[%s_slot[i]]" % (c["name"], c["name"]))
        elif c["set"] == "bool":
            # A pixel shader's bool constants are after the vertex shader's.
            lines.append("#define %s xe_bool(%du)  // b%d" % (
                c["name"], c["register"] + (128 if pixel else 0), c["register"]))
        elif c["set"] == "sampler":
            if c["register"] in textures:
                aliases.append((c["name"], textures[c["register"]]))
            else:
                textures[c["register"]] = c["name"]
    # Textures: the images (unsigned, then signed) and the samplers, at the
    # translator's bindings.
    texture_set = 3 if pixel else 2
    names = {}
    for binding, t in enumerate(info["textures"]):
        name = textures[t["fetch_constant"]]
        kind = "textureCube" if t["view"] == "cube" else "texture2DArray"
        view = "signed" if t["signed"] else "unsigned"
        lines.append("layout(set = %d, binding = %d) uniform %s %s_%s;" % (
            texture_set, binding, kind, name, view))
        names[t["fetch_constant"]] = name
    for n, s in enumerate(info["samplers"]):
        fc = s["fetch_constant"]
        lines.append("layout(set = %d, binding = %d) uniform sampler %s_sampler;" % (
            texture_set, len(info["textures"]) + n, names[fc]))
        lines.append("#define %s %s_unsigned, %s_signed, %s_sampler, %du" % (
            names[fc], names[fc], names[fc], names[fc], fc))
    for name, same in aliases:
        lines.append("#define %s %s" % (name, same))
    if not pixel:
        # Each vertex fetch's fields: the renderer sets them from the patched
        # instruction (src/native/shader_ports.cpp), as for the ports.
        for n, f in enumerate(info["vertex_fetches"]):
            base = 100 + 10 * n
            lines.append("layout(constant_id = %d) const uint XE_VF%d_CONSTANT = %du;" % (
                base, n, f["fetch_constant"]))
            lines.append("layout(constant_id = %d) const int XE_VF%d_OFFSET = 0;" % (base + 1, n))
            lines.append("layout(constant_id = %d) const uint XE_VF%d_STRIDE = 0u;" % (base + 2, n))
            lines.append("layout(constant_id = %d) const uint XE_VF%d_FORMAT = 0u;" % (base + 3, n))
            lines.append("layout(constant_id = %d) const uint XE_VF%d_FLAGS = 0u;" % (base + 4, n))
            lines.append("layout(constant_id = %d) const int XE_VF%d_EXP_ADJUST = 0;" % (base + 5, n))
            lines.append("layout(constant_id = %d) const uint XE_VF%d_SWIZZLE = 0x688u;" % (base + 6, n))
            for g in range(3):
                lines.append("layout(location = %d) in uvec4 xe_vf_in_%d_%d;" % (3 * n + g, n, g))
        lines.append("layout(constant_id = 2) const bool XE_RECTANGLE_LIST = false;")
    for d in defines:
        lines.append("#define %s" % d.replace("=", " ", 1))
    lines.append('#include "%s"' % os.path.basename(source))
    return "\n".join(lines) + "\n"


def write_sources(folder):
    """Every shader's full source in folder, by name; returns the names."""
    infos = {os.path.splitext(i["port"])[0]: i
             for i in json.load(open(os.path.join(GAME, "shaders.json")))}
    variants = read_variants()
    file_names = [os.path.basename(path) for path in INCLUDES]
    for name in set(file_names):
        if file_names.count(name) > 1:
            raise SystemExit("two hand-written files are named %s" % name)
    for path in INCLUDES:
        shutil.copy(path, folder)
    names = []
    ports = sorted(glob.glob(os.path.join(GAME, "*.vert"))) + \
        sorted(glob.glob(os.path.join(GAME, "*.frag")))
    for port in ports:
        name, extension = os.path.splitext(os.path.basename(port))
        if name in variants:
            source, defines = variants[name]
            text = header(infos[name], source, defines)
        else:
            text = open(port).read()
        open(os.path.join(folder, name + extension), "w", newline="\n").write(text)
        names.append(name + extension)
    missing = set(variants) - set(os.path.splitext(n)[0] for n in names)
    if missing:
        raise SystemExit("variants.txt names shaders that aren't in game/: %s" % sorted(missing))
    return names


def compile_shader(source, folder):
    raw, out = os.path.join(folder, "raw.spv"), os.path.join(folder, "out.spv")
    subprocess.run(["glslangValidator", "-V", "-g0", "--target-env", "vulkan1.3", "-o", raw,
                    source], check=True, stdout=subprocess.DEVNULL)
    subprocess.run(["spirv-opt", "--eliminate-dead-functions", raw, "-o", out], check=True)
    data = open(out, "rb").read()
    return struct.unpack("<%dI" % (len(data) // 4), data)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--emit", help="write the full sources to this folder instead")
    args = parser.parse_args()
    if args.emit:
        os.makedirs(args.emit, exist_ok=True)
        print("%d shaders written" % len(write_sources(args.emit)))
        return
    lines = ["// Generated by src/native/glsl/compile_game.py from game/ and the hand-written",
             "// shaders (materials/, post/, boot/, vertex/);",
             "// don't edit.", "#pragma once", "#include <cstdint>"]
    with tempfile.TemporaryDirectory() as folder:
        sources = os.path.join(folder, "sources")
        os.makedirs(sources)
        names = write_sources(sources)
        for file_name in names:
            name = os.path.splitext(file_name)[0]
            words = compile_shader(os.path.join(sources, file_name), folder)
            rows = [",".join("0x%x" % w for w in words[i:i + 12]) for i in range(0, len(words), 12)]
            lines.append("inline constexpr uint32_t k_game_%s[] = {\n%s};" % (name, ",\n".join(rows)))
    open(os.path.join(HERE, "..", "generated_game_shaders.h"), "w", newline="\n").write(
        "\n".join(lines) + "\n")
    print("%d shaders compiled (%d written by hand)" % (len(names), len(read_variants())))


if __name__ == "__main__":
    main()
