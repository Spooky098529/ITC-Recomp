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

Every shader is also written as HLSL (Shader Model 5) for the D3D11 backend,
into ../generated_game_shaders_hlsl.h (docs/d3d11-backend.md, "Shaders for
D3D11"): SPIRV-Cross from the same GLSL (compiled again with its names, for
readable HLSL), vertex shaders with Y flipped to D3D's clip space. The run-time
fit becomes defines: each specialization constant SPIRV_CROSS_CONSTANT_ID_n,
and interpolator n's semantic XE_INTERPOLATOR_n (TEXCOORDn unless defined).
--check-hlsl also compiles every HLSL shader with D3DCompile under Wine
(tools/hlsl_check, Microsoft's d3dcompiler_47.dll in the prefix).

Needs glslangValidator and spirv-opt (both in the Vulkan SDK) and
spirv-cross (tools/build_spirv_cross.sh builds it into out/tools; $SPIRV_CROSS
or the PATH otherwise). The SPIR-V
isn't optimized (the driver does that; inlining every helper made the
modules several times bigger), only stripped of its debug names and unused
functions. tools/shader_port/check compiles them the same way."""
import argparse
import glob
import json
import os
import re
import shutil
import struct
import subprocess
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.normpath(os.path.join(HERE, "..", "..", "..", ".."))
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
    # Float constants: the registers from the lowest the shader reads to the
    # highest (all 256 when it indexes them by the loop counter), as the
    # renderer gives them; a constant's place is its register less the lowest.
    used = info["float_constants"]
    packed = list(range(256)) if info["relative_constants"] else \
        (list(range(used[0], used[-1] + 1)) if used else [])
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
    # Textures: a view each, then the samplers (the renderer's bindings:
    # shader_ports.cpp, in the table's order).
    texture_set = 3 if pixel else 2
    names = {}
    views = info["textures"]
    for binding, t in enumerate(views):
        name = textures[t["fetch_constant"]]
        kind = "textureCube" if t["view"] == "cube" else "texture2DArray"
        lines.append("layout(set = %d, binding = %d) uniform %s %s_view;" % (
            texture_set, binding, kind, name))
        names[t["fetch_constant"]] = name
    for n, s in enumerate(info["samplers"]):
        fc = s["fetch_constant"]
        lines.append("layout(set = %d, binding = %d) uniform sampler %s_sampler;" % (
            texture_set, len(views) + n, names[fc]))
        lines.append("#define %s %s_view, %s_sampler, %du" % (
            names[fc], names[fc], names[fc], fc))
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


def compile_spirv(source, folder, names=False):
    """The shader's SPIR-V file, without debug names unless names."""
    raw, out = os.path.join(folder, "raw.spv"), os.path.join(folder, "out.spv")
    subprocess.run(["glslangValidator", "-V"] + ([] if names else ["-g0"]) +
                   ["--target-env", "vulkan1.3", "-o", raw, source],
                   check=True, stdout=subprocess.DEVNULL)
    subprocess.run(["spirv-opt", "--eliminate-dead-functions", raw, "-o", out], check=True)
    return out


def compile_shader(source, folder):
    data = open(compile_spirv(source, folder), "rb").read()
    return struct.unpack("<%dI" % (len(data) // 4), data)


def spirv_cross():
    if os.environ.get("SPIRV_CROSS"):
        return os.environ["SPIRV_CROSS"]
    built = os.path.join(REPO, "itc_recomp", "out", "tools", "spirv-cross", "spirv-cross")
    return built if os.path.exists(built) else "spirv-cross"


def to_hlsl(spirv, vertex, interpolators=True):
    """The SPIR-V file as Shader Model 5 HLSL. With interpolators, the
    interpolators' semantics (the vertex shader's outputs, the pixel shader's
    inputs) become XE_INTERPOLATOR_n, which the backend defines as the places
    it gives them (as port_spirv.cpp moves their locations for Vulkan)."""
    text = subprocess.run([spirv_cross(), "--hlsl", "--shader-model", "50"] +
                          (["--flip-vert-y"] if vertex else []) + [spirv],
                          check=True, stdout=subprocess.PIPE, text=True).stdout
    if not interpolators:
        return text
    struct_name = "SPIRV_Cross_Output" if vertex else "SPIRV_Cross_Input"
    match = re.search(r"^struct %s\n\{\n.*?^\};\n" % struct_name, text, re.M | re.S)
    if not match:
        return text  # no interpolators
    used = []

    def semantic(m):
        used.append(int(m.group(1)))
        return ": XE_INTERPOLATOR_%s;" % m.group(1)
    block = re.sub(r": TEXCOORD(\d+);", semantic, match.group(0))
    prelude = ["// Interpolator n's semantic: the place the renderer gives it, TEXCOORDn",
               "// unless it defines XE_INTERPOLATOR_n."]
    for n in sorted(used):
        prelude += ["#ifndef XE_INTERPOLATOR_%d" % n, "#define XE_INTERPOLATOR_%d TEXCOORD%d" % (n, n),
                    "#endif"]
    return "\n".join(prelude) + "\n" + text[:match.start()] + block + text[match.end():]


def check_hlsl(files):
    """Compiles HLSL files ((profile, path) pairs) with D3DCompile under Wine;
    False if any fails."""
    exe = os.path.join(REPO, "itc_recomp", "out", "tools", "hlsl_check.exe")
    if not os.path.exists(exe):
        raise SystemExit("build tools/hlsl_check first (its build.sh)")
    env = dict(os.environ, WINEDEBUG="-all", WINEDLLOVERRIDES="mscoree,mshtml=")
    env.setdefault("WINEPREFIX", os.path.join(REPO, "itc_recomp", "out", "wineprefix"))
    result = subprocess.run(["wine", exe] + ["%s:%s" % (p, f) for p, f in files], env=env,
                            stdout=subprocess.PIPE, text=True)
    print(result.stdout.strip().splitlines()[-1] if result.returncode == 0 else result.stdout)
    return result.returncode == 0


def hlsl_header(comment, shaders):
    """A header of raw string literals: shaders is (name, text) pairs."""
    lines = comment + ["#pragma once"]
    for name, text in shaders:
        if ")hlsl\"" in text:
            raise SystemExit("%s's HLSL holds the literal's end" % name)
        lines.append("inline constexpr char k_hlsl_%s[] = R\"hlsl(%s)hlsl\";" % (name, text))
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--emit", help="write the full sources to this folder instead")
    parser.add_argument("--check-hlsl", action="store_true",
                        help="compile every HLSL shader with D3DCompile under Wine")
    args = parser.parse_args()
    if args.emit:
        os.makedirs(args.emit, exist_ok=True)
        print("%d shaders written" % len(write_sources(args.emit)))
        return
    lines = ["// Generated by src/native/glsl/compile_game.py from game/ and the hand-written",
             "// shaders (materials/, post/, boot/, vertex/);",
             "// don't edit.", "#pragma once", "#include <cstdint>"]
    hlsl = []
    with tempfile.TemporaryDirectory() as folder:
        sources = os.path.join(folder, "sources")
        os.makedirs(sources)
        names = write_sources(sources)
        checks = []
        for file_name in names:
            name = os.path.splitext(file_name)[0]
            source = os.path.join(sources, file_name)
            words = compile_shader(source, folder)
            rows = [",".join("0x%x" % w for w in words[i:i + 12]) for i in range(0, len(words), 12)]
            lines.append("inline constexpr uint32_t k_game_%s[] = {\n%s};" % (name, ",\n".join(rows)))
            vertex = file_name.endswith(".vert")
            text = to_hlsl(compile_spirv(source, folder, names=True), vertex)
            hlsl.append(("game_" + name, text))
            path = os.path.join(folder, name + ".hlsl")
            open(path, "w", newline="\n").write(text)
            checks.append(("vs_5_0" if vertex else "ps_5_0", path))
        if args.check_hlsl and not check_hlsl(checks):
            raise SystemExit("an HLSL shader didn't compile; nothing written")
    open(os.path.join(HERE, "..", "generated_game_shaders.h"), "w", newline="\n").write(
        "\n".join(lines) + "\n")
    open(os.path.join(HERE, "..", "generated_game_shaders_hlsl.h"), "w", newline="\n").write(
        hlsl_header(["// Generated by src/native/glsl/compile_game.py: the game's shaders as HLSL",
                     "// (Shader Model 5) for the D3D11 backend, through SPIRV-Cross; don't edit."],
                    hlsl))
    print("%d shaders compiled (%d written by hand)" % (len(names), len(read_variants())))


if __name__ == "__main__":
    main()
