"""Checks each ported shader against the SDK's translation of the same
microcode: both run with the same random inputs (vkdiff), and every pixel
(or vertex output) must match.

    python3 check_ports.py --translate <translate> --vkdiff <vkdiff>
        --specialize <specialize> --ports <folder of .vert/.frag> <shader files...>

With --tolerance, checks hand-written shaders (compile_game.py --emit's
sources) within a tolerance instead of bit for bit.

Needs glslangValidator and spirv-opt, and a Vulkan device (lavapipe is
enough). See README.md."""
import argparse, glob, math, os, random, struct, subprocess, sys, tempfile
import xxhash

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, ".."))
from xenos_ucode import parse_file
from ucode_to_glsl import Port, Unsupported

VERTEX_FORMATS = [6, 7, 16, 17, 25, 26, 31, 32, 33, 34, 35, 36, 37, 38, 57]
FORMAT_WORDS = {6: 1, 7: 1, 16: 1, 17: 1, 25: 1, 26: 2, 31: 1, 32: 2, 33: 1, 34: 2, 35: 4, 36: 1,
                37: 2, 38: 4, 57: 3}
FLOAT_FORMATS = {36, 37, 38, 57}
TEXTURE_SIZE = 16
TEXTURE_LEVELS = 5  # --levels sets it


def words_le(*ws):
    return struct.pack("<%dI" % len(ws), *[w & 0xFFFFFFFF for w in ws])


def fbits(x):
    return struct.unpack("<I", struct.pack("<f", x))[0]


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError("%s failed:\n%s%s" % (cmd[0], r.stdout[-2000:], r.stderr[-2000:]))
    return r.stdout


def parse_translation(path):
    info = {}
    for line in open(path):
        k, *v = line.split()
        info[k] = v
    textures = [tuple(int(x) for x in t.split(":")) for t in info.get("textures", [])]
    samplers = [tuple(int(x) for x in t.split(":")) for t in info.get("samplers", [])]
    floats = [int(x) for x in info.get("float", [])]
    return textures, samplers, floats


def random_float(rng, infinities=True):
    k = rng.random()
    if k < 0.08:
        return 0.0
    if k < 0.12:
        return 1.0
    if k < 0.14:
        return -1.0
    if k < 0.15 and infinities:
        return rng.choice([float("inf"), float("-inf")])
    return rng.uniform(-2.0, 2.0)


def patch_vertex_fetches(words, port, rng):
    """The microcode with each vertex fetch given a random layout, as the
    game's patcher does, and the fields for the port's constants."""
    words = list(words)
    fields = []
    chosen = []
    strides = {}  # a vertex buffer's fetches share its stride
    for f in port.vfetches:
        fmt = rng.choice(VERTEX_FORMATS)
        offset = rng.randrange(0, 4)
        chosen.append((fmt, offset))
        strides[f.fetch_constant] = max(strides.get(f.fetch_constant, 1),
                                        FORMAT_WORDS[fmt] + offset + rng.randrange(0, 3))
    for n, f in enumerate(port.vfetches):
        fmt, offset = chosen[n]
        signed, integer = rng.random() < 0.5, rng.random() < 0.3
        no_zero, rounded = rng.random() < 0.3, rng.random() < 0.3
        exp = rng.choice([0, 0, 0, 1, -2])
        stride = strides[f.fetch_constant]
        a = f.address * 3
        w1 = words[a + 1] & ~(0x3FFFF << 12)
        w1 |= (signed << 12) | (integer << 13) | (no_zero << 14) | (rounded << 15) | (fmt << 16) | \
            ((exp & 0x3F) << 24)
        words[a + 1] = w1
        words[a + 2] = (words[a + 2] & 0x80000000) | stride | (offset << 8)
        flags = signed | (integer << 1) | (no_zero << 2) | (rounded << 3)
        fields += [(100 + 10 * n + 0, f.fetch_constant), (100 + 10 * n + 1, offset),
                   (100 + 10 * n + 2, stride), (100 + 10 * n + 3, fmt), (100 + 10 * n + 4, flags),
                   (100 + 10 * n + 5, exp)]
    return words, fields


def make_job(port, translated_spv, port_spv, textures, sampler_count, floats, rng, vertex_count,
             interpolators, rect, memory_extra, always_pass=False, infinities=True):
    pixel = not port.vertex
    width, height = (32, 32) if pixel else (17, vertex_count)
    job = words_le(1 if pixel else 0, width, height)
    for spv in (translated_spv, port_spv):
        data = open(spv, "rb").read()
        job += words_le(len(data) // 4) + data
    job += words_le(interpolators)
    job += words_le(len(textures))
    for fc, dim, signed in textures:
        view = 3 if dim == 3 else 1
        layers = 6 if view == 3 else 1
        texels = bytes(rng.randrange(256) for _ in range(
            layers * sum(max(TEXTURE_SIZE >> l, 1) ** 2 for l in range(TEXTURE_LEVELS)) * 4))
        job += words_le(view, signed, TEXTURE_SIZE, TEXTURE_LEVELS) + texels + \
            b"\0" * ((-len(texels)) % 4)
    job += words_le(sampler_count)
    # System constants.
    system = bytearray(512)
    flags = 0
    if pixel:
        flags |= rng.choice([0, 1]) << 11 | rng.choice([0, 0, 1]) << 12
        flags |= (7 if always_pass else rng.randrange(8)) << 16 | rng.choice([0, 1]) << 19
    else:
        flags |= rng.choice([0, 1]) << 8 | rng.choice([0, 1]) << 9 | rng.choice([0, 1]) << 10
    struct.pack_into("<I", system, 0, flags)
    struct.pack_into("<I", system, 12, 0xFFFFFFFF)  # line loop closing index
    struct.pack_into("<i", system, 16, rng.randrange(0, 3))  # vertex base index
    struct.pack_into("<3f", system, 124, *(rng.uniform(0.5, 1.5) for _ in range(3)))
    struct.pack_into("<3f", system, 140, *(rng.uniform(-0.5, 0.5) for _ in range(3)))
    for k in range(8):
        struct.pack_into("<I", system, 176 + 4 * k, rng.getrandbits(32))
    struct.pack_into("<f", system, 272, rng.uniform(0.0, 1.0))
    struct.pack_into("<4f", system, 288, *(rng.choice([0.5, 1.0, 2.0]) for _ in range(4)))
    struct.pack_into("<II", system, 496, 0, 0xFFFFFF)
    job += bytes(system)
    # Float constants: the translator's packing, with the literal constants.
    regs = list(range(256)) if port.dynamic_constants else floats
    data = b""
    for r in regs:
        v = port.s.literals.get(r) or [random_float(rng, infinities) for _ in range(4)]
        data += struct.pack("<4f", *v)
    job += words_le(max(len(regs), 1)) + (data or b"\0" * 16)
    # Bool and loop constants.
    bools = [rng.getrandbits(32) for _ in range(8)]
    loops = [rng.choice([0, 1, 2, 3, 3]) | (rng.choice([0, 0, 1]) << 8) | (1 << 16) for _ in range(32)]
    job += words_le(*bools, *loops)
    # Fetch constants: textures (size, LOD bias / exponent word) and vertex
    # buffers (in shared memory from word 1024).
    fetch = [rng.getrandbits(32) for _ in range(192)]
    for fc in range(32):
        fetch[fc * 6 + 2] = (TEXTURE_SIZE - 1) | ((TEXTURE_SIZE - 1) << 13)
        fetch[fc * 6 + 4] = rng.choice([0, 0, rng.getrandbits(32)])
    for f in port.vfetches if port.vertex else []:
        c = f.fetch_constant
        fetch[c * 2] = (1024 << 2) | 3
        fetch[c * 2 + 1] = (4096 << 2) | rng.randrange(4)
    job += words_le(*fetch)
    memory = memory_extra
    job += words_le(len(memory)) + struct.pack("<%dI" % len(memory), *memory)
    if not pixel:
        job += words_le(vertex_count)
    return job, width, height


def compare(a, b, columns=None, tolerance=0.0):
    """How many values differ, and by how much at most. With a tolerance, a
    value within it of the other (relative to the larger of its pixel's or
    vertex output's values, and at least 1) counts as the same, and so does
    one where either is infinite or NaN: a hand-written shader doesn't keep
    the console's 0 * infinity = 0, so where an infinity comes up the two
    can differ."""
    fa = struct.unpack("<%df" % (len(a) // 4), a)
    fb = struct.unpack("<%df" % (len(b) // 4), b)
    bad, worst = 0, 0.0
    for k, (x, y) in enumerate(zip(fa, fb)):
        if columns is not None and (k // 4) % 17 >= columns:
            continue
        if x == y or (x != x and y != y):
            continue
        if tolerance:
            if not math.isfinite(x) or not math.isfinite(y):
                continue
            group = [v for v in fa[k // 4 * 4:k // 4 * 4 + 4] + fb[k // 4 * 4:k // 4 * 4 + 4]
                     if math.isfinite(v)]
            if abs(x - y) <= tolerance * max([1.0] + [abs(v) for v in group]):
                continue
        bad += 1
        if x == x and y == y:
            worst = max(worst, abs(x - y))
        else:
            worst = float("inf")
    return bad, worst


def check(path, args, rng, tmp):
    data = open(path, "rb").read()
    shader = parse_file(data)
    try:
        port = Port(shader, path)
    except Unsupported as e:
        return "unsupported: %s" % e
    stage = "vs" if shader.is_vertex else "ps"
    h = xxhash.xxh3_64(struct.pack(">%dI" % len(shader.words), *shader.words)).hexdigest().upper()
    source = os.path.join(args.ports, "%s_%s.%s" % (stage, h, "vert" if shader.is_vertex else "frag"))
    compiled = os.path.join(tmp, "port.spv")
    # As src/native/glsl/compile_game.py compiles them.
    run(["glslangValidator", "-V", "-g0", "--target-env", "vulkan1.3", source, "-o", compiled + ".raw"])
    run(["spirv-opt", "--eliminate-dead-functions", compiled + ".raw", "-o", compiled])
    results = []
    for case in range(args.cases):
        words, fields = list(shader.words), []
        rect = False
        if shader.is_vertex:
            words, fields = patch_vertex_fetches(words, port, rng)
            rect = case % 2 == 1
            exported = sorted(e for e in port.exports if e < 16)
            mask = 0
            for e in range(16):
                if (e in exported and rng.random() < 0.85) or rng.random() < 0.05:
                    mask |= 1 << e
            param_gen = 255
        else:
            inputs = min(port.register_count, 16)
            full = (1 << inputs) - 1
            mask = full if case == 0 or args.tolerance else full & rng.getrandbits(16)
            param_gen = 255 if case == 0 or args.tolerance or not inputs or rng.random() < 0.5 \
                else rng.randrange(inputs)
            if param_gen != 255:
                mask &= ~(1 << param_gen)
        ucode = os.path.join(tmp, "ucode.bin")
        open(ucode, "wb").write(struct.pack(">%dI" % len(words), *words))
        tspv, ttxt = os.path.join(tmp, "t.spv"), os.path.join(tmp, "t.txt")
        run([args.translate, stage, ucode, "63", str(mask), str(param_gen), str(int(rect)), tspv, ttxt])
        textures, samplers, floats = parse_translation(ttxt)
        port_textures = [(fc, dim, int(s)) for fc, dim, s in port.textures]
        if textures != port_textures:
            return "texture bindings differ: translator %s, port %s" % (textures, port_textures)
        port_samplers = [tuple(s) for s in port.samplers]
        if samplers != port_samplers:
            return "sampler bindings differ: translator %s, port %s" % (samplers, port_samplers)
        if not port.dynamic_constants and floats != sorted(port.float_used):
            return "float constants differ: translator %s, port %s" % (floats, sorted(port.float_used))
        spec = [(2, int(rect))] + fields if shader.is_vertex else \
            [(0, mask), (1, -1 if param_gen == 255 else param_gen)]
        pspv = os.path.join(tmp, "p.spv")
        if args.hookup:
            # The renderer's own path: the port found by the (patched)
            # microcode, its constants read from it.
            run([args.hookup, stage, ucode, str(mask), str(param_gen), str(int(rect)), pspv])
        else:
            run([args.specialize, compiled, pspv, stage, str(mask)] + ["%d=%d" % kv for kv in spec])
        if shader.is_vertex:
            memory = [rng.getrandbits(32) for _ in range(1024)]
            memory += [fbits(random_float(rng, not args.tolerance)) if rng.random() < 0.7 else rng.getrandbits(32)
                       for _ in range(5120)]
            vertex_count = 24 if rect else 16
            interpolators = bin(mask).count("1")
        else:
            memory = [fbits(rng.uniform(-1.0, 2.0)) for _ in range(192)] + [0] * 64
            vertex_count, interpolators = 3, 16
        job, width, height = make_job(port, tspv, pspv, textures, len(samplers), floats, rng,
                                      vertex_count, interpolators, rect, memory, case < 2,
                                      not args.tolerance)
        jp = os.path.join(tmp, "job.bin")
        open(jp, "wb").write(job)
        ra, rb = os.path.join(tmp, "a.bin"), os.path.join(tmp, "b.bin")
        run([args.vkdiff, jp, ra, rb])
        a, b = open(ra, "rb").read(), open(rb, "rb").read()
        bad, worst = compare(a, b, None if not shader.is_vertex else interpolators + 1,
                             args.tolerance)
        values = set(struct.unpack("<%dI" % (len(a) // 4), a))
        # With a tolerance, a few values may be further off: where the random
        # inputs make the result ill-conditioned (a highlight's exponent on a
        # cosine near 0, say), or a texel's filtering lands differently.
        allowed = int(len(a) // 4 * args.outliers) if args.tolerance else 0
        results.append((bad, worst, mask, param_gen, rect, len(values)))
        if bad > allowed:
            if args.keep:
                for name in ("job.bin", "a.bin", "b.bin", "t.spv", "p.spv", "ucode.bin"):
                    os.replace(os.path.join(tmp, name), os.path.join(args.keep, "%s_%s" % (h, name)))
            return "MISMATCH case %d (mask %04X, param gen %d, rect %d): %d values, worst %g" % (
                case, mask, param_gen, rect, bad, worst)
    outliers = sum(r[0] for r in results)
    return "ok (%d cases; distinct values per case: %s%s)" % (
        args.cases, " ".join(str(r[5]) for r in results),
        "; %d values past the tolerance" % outliers if outliers else "")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("inputs", nargs="+")
    ap.add_argument("--translate", required=True)
    ap.add_argument("--vkdiff", required=True)
    ap.add_argument("--specialize", required=True)
    ap.add_argument("--hookup", help="port_hookup: test the renderer's lookup and fitting instead")
    ap.add_argument("--ports", required=True)
    ap.add_argument("--cases", type=int, default=4)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--keep", help="folder to keep a mismatch's files in")
    ap.add_argument("--tolerance", type=float, default=0.0,
                    help="for hand-written shaders: how far a value may be off (relative); "
                         "the inputs then have no infinities and every interpolator")
    ap.add_argument("--levels", type=int, default=5,
                    help="mip levels of the random textures (1: the level picked doesn't "
                         "matter, for shaders that sample where it's undefined)")
    ap.add_argument("--outliers", type=float, default=0.002,
                    help="with --tolerance: the share of values that may be past it")
    args = ap.parse_args()
    global TEXTURE_LEVELS
    TEXTURE_LEVELS = args.levels
    files = []
    for p in args.inputs:
        files += sorted(glob.glob(os.path.join(p, "*.xpu"))) if os.path.isdir(p) else [p]
    seen, failures = set(), 0
    with tempfile.TemporaryDirectory() as tmp:
        for f in files:
            shader = parse_file(open(f, "rb").read())
            key = tuple(shader.words)
            if key in seen:
                continue
            seen.add(key)
            rng = random.Random("%d:%s" % (args.seed, key[:64]))
            try:
                result = check(f, args, rng, tmp)
            except RuntimeError as e:
                result = "ERROR %s" % str(e)[:600]
            if not result.startswith(("ok", "unsupported")):
                failures += 1
            print("%s: %s" % (os.path.basename(f), result), flush=True)
    print("%d shaders, %d failed" % (len(seen), failures))
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
