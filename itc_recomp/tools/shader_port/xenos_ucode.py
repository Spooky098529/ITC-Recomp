"""Xbox 360 shader files (XPU) and their Xenos microcode, decoded.

A shader file is a header, an optional block of literal constants, a
constant table (the HLSL names and registers), an info block, and the
microcode. Everything is big-endian. The microcode layout follows the
public descriptions of the Xenos instruction set (control flow words of 48
bits, then ALU and fetch instructions of 96 bits)."""
import struct
from dataclasses import dataclass, field

MAGIC_PIXEL, MAGIC_VERTEX = 0x102A0E00, 0x102A0E01
REGISTER_SETS = {0: "bool", 1: "int4", 2: "float4", 3: "sampler"}

@dataclass
class Constant:
    name: str
    register_set: str
    register: int
    count: int

@dataclass
class ShaderFile:
    is_vertex: bool
    constants: list
    literals: dict          # float register -> (x, y, z, w)
    target: str
    words: list             # the microcode
    info: bytes
    literal_header: tuple = ()

def _cstr(b, o):
    return b[o:b.index(b"\0", o)].decode("ascii", "replace")

def parse_file(b):
    magic, header_size, ucode_size, literal_off, ctab_off, info_off = struct.unpack(">6I", b[:24])
    if magic not in (MAGIC_PIXEL, MAGIC_VERTEX):
        raise ValueError("not a shader file")
    constants, target = [], ""
    if ctab_off:
        t = ctab_off + 4  # the table follows its size
        _, _, _, count, info, _, target_off = struct.unpack(">7I", b[t:t + 28])
        target = _cstr(b, t + target_off)
        for i in range(count):
            name, rset, reg, n, _, _, _ = struct.unpack(">IHHHHII", b[t + info + 20 * i:t + info + 20 * i + 20])
            constants.append(Constant(_cstr(b, t + name), REGISTER_SETS.get(rset, str(rset)), reg, n))
    literals, lit_header = {}, ()
    if literal_off:
        lit_header = struct.unpack(">6I", b[literal_off:literal_off + 24])
        o = literal_off + 24
        end = ctab_off or info_off
        while o + 4 <= end:
            where, n = struct.unpack(">HH", b[o:o + 4])
            if n == 0 or o + 4 + 4 * n > end:
                break
            vals = struct.unpack(">%df" % n, b[o + 4:o + 4 + 4 * n])
            reg = (where & 0xFFF) // 16 + 208  # see docs/shaders.md, "Literal constants"
            for k in range(n // 4):
                literals[reg + k] = vals[4 * k:4 * k + 4]
            o += 4 + 4 * n
    words = list(struct.unpack(">%dI" % (ucode_size // 4), b[header_size:header_size + ucode_size]))
    return ShaderFile(magic == MAGIC_VERTEX, constants, literals, target, words,
                      b[info_off:header_size], lit_header)

# --- microcode ---------------------------------------------------------------
CF = ["nop", "exec", "exece", "cexec", "cexece", "cexec_pred", "cexece_pred", "loop",
      "endloop", "ccall", "ret", "cjmp", "alloc", "cexec_pred_clean", "cexece_pred_clean",
      "vfetch_done"]
VECTOR_OPS = ["add", "mul", "max", "min", "seq", "sgt", "sge", "sne", "frc", "trunc", "floor",
              "mad", "cndeq", "cndge", "cndgt", "dp4", "dp3", "dp2add", "cube", "max4",
              "setp_eq_push", "setp_ne_push", "setp_gt_push", "setp_ge_push", "kill_eq",
              "kill_gt", "kill_ge", "kill_ne", "dst", "maxa"]
VECTOR_ARITY = [2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 3, 3, 3, 3, 2, 2, 3, 2, 1, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2]
SCALAR_OPS = ["adds", "adds_prev", "muls", "muls_prev", "muls_prev2", "maxs", "mins", "seqs",
              "sgts", "sges", "snes", "frcs", "truncs", "floors", "exp", "logc", "log",
              "rcpc", "rcpf", "rcp", "rsqc", "rsqf", "rsq", "maxas", "maxasf", "subs",
              "subs_prev", "setp_eq", "setp_ne", "setp_gt", "setp_ge", "setp_inv", "setp_pop",
              "setp_clr", "setp_rstr", "kills_eq", "kills_gt", "kills_ge", "kills_ne",
              "kills_one", "sqrt", "op41", "mulsc0", "mulsc1", "addsc0", "addsc1", "subsc0",
              "subsc1", "sin", "cos", "retain_prev"]
# Components each scalar operation reads (2: the operand's first two).
SCALAR_ARITY = [2, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
                1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 1, 1, 0]

def bits(v, lo, n):
    return (v >> lo) & ((1 << n) - 1)

def _signed(v, n):
    return v - (1 << n) if v & (1 << (n - 1)) else v

@dataclass
class Operand:
    is_temp: bool
    index: int
    components: list        # source component per result component (0-3)
    negate: bool = False
    absolute: bool = False
    relative: str = ""      # "", "aL" (loop index) or "a0" (address register)

@dataclass
class Alu:
    address: int
    vector_op: str = ""
    vector_operands: list = field(default_factory=list)
    vector_dest: int = 0
    vector_mask: int = 0
    vector_saturate: bool = False
    scalar_op: str = ""
    scalar_operands: list = field(default_factory=list)
    scalar_dest: int = 0
    scalar_mask: int = 0
    scalar_saturate: bool = False
    export: bool = False
    const0_mask: int = 0
    const1_mask: int = 0
    predicated: bool = False
    predicate_condition: bool = False
    vector_dest_relative: bool = False
    scalar_dest_relative: bool = False
    kind: str = "alu"

@dataclass
class VertexFetch:
    address: int
    fetch_constant: int
    source: int
    source_component: int
    dest: int
    dest_swizzle: list      # per component: 0-3 a fetched component, 4 zero, 5 one, 7 keep
    format: int
    stride_words: int
    offset_words: int
    is_signed: bool
    is_integer: bool
    exp_adjust: int
    mini: bool
    predicated: bool
    predicate_condition: bool
    signed_rf_mode: bool = False
    index_round: bool = False
    kind: str = "vfetch"

@dataclass
class TextureFetch:
    address: int
    opcode: int
    fetch_constant: int
    source: int
    source_components: list
    dest: int
    dest_swizzle: list
    dimension: int          # 0 1D, 1 2D, 2 3D or stacked, 3 cube
    unnormalized: bool
    filters: tuple          # mag, min, mip, aniso
    use_computed_lod: bool
    use_register_lod: bool
    use_register_gradients: bool
    lod_bias: float
    offsets: tuple
    predicated: bool
    predicate_condition: bool
    kind: str = "tfetch"

@dataclass
class ControlFlow:
    index: int
    op: str
    address: int = 0
    count: int = 0
    sequence: int = 0
    bool_address: int = -1
    condition: bool = False
    loop_id: int = 0
    alloc_type: int = 0
    alloc_size: int = 0
    predicate_clean: bool = True
    unconditional: bool = False
    is_repeat: bool = False
    predicated: bool = False
    instructions: list = field(default_factory=list)

def _dest_swizzle(v):
    return [bits(v, 3 * i, 3) for i in range(4)]

def _operand(sel, reg, swiz, negate, addressed, abs_constants, address_register):
    comps = [(i + bits(swiz, 2 * i, 2)) & 3 for i in range(4)]
    if sel:
        return Operand(True, reg & 0x3F, comps, bool(negate), bool(reg & 0x80),
                       "aL" if reg & 0x40 else "")
    rel = ("a0" if address_register else "aL") if addressed else ""
    return Operand(False, reg, comps, bool(negate), bool(abs_constants), rel)

def decode_alu(address, w0, w1, w2):
    """Both operations are always decoded: the scalar result becomes the
    previous scalar even when nothing is written, and some vector operations
    change state without a write mask."""
    i = Alu(address)
    scalar_opc, vector_opc = bits(w0, 26, 6), bits(w2, 24, 5)
    i.export = bool(bits(w0, 15, 1))
    vmask, smask = bits(w0, 16, 4), bits(w0, 20, 4)
    i.vector_dest_relative = bool(bits(w0, 6, 1))
    i.scalar_dest_relative = bool(bits(w0, 14, 1))
    abs_constants = bits(w0, 7, 1)
    address_register = bits(w1, 29, 1)
    const_0_addressed, const_1_addressed = bits(w1, 31, 1), bits(w1, 30, 1)
    sel = {1: bits(w2, 31, 1), 2: bits(w2, 30, 1), 3: bits(w2, 29, 1)}
    reg = {1: bits(w2, 16, 8), 2: bits(w2, 8, 8), 3: bits(w2, 0, 8)}
    swiz = {1: bits(w1, 16, 8), 2: bits(w1, 8, 8), 3: bits(w1, 0, 8)}
    neg = {1: bits(w1, 26, 1), 2: bits(w1, 25, 1), 3: bits(w1, 24, 1)}
    # Which of the two addressing bits a constant operand uses.
    addressed = {1: const_0_addressed,
                 2: const_0_addressed if sel[1] else const_1_addressed,
                 3: const_0_addressed if (sel[1] and sel[2]) else const_1_addressed}
    i.predicated, i.predicate_condition = bool(bits(w1, 28, 1)), bool(bits(w1, 27, 1))
    if i.export:
        i.vector_mask, i.scalar_mask = vmask & ~smask, smask & ~vmask
        i.const1_mask = vmask & smask
        i.const0_mask = (0xF & ~(vmask | smask)) if i.scalar_dest_relative else 0
        i.vector_dest = i.scalar_dest = bits(w0, 0, 6)
        i.vector_dest_relative = i.scalar_dest_relative = False
    else:
        i.vector_mask, i.scalar_mask = vmask, smask
        i.vector_dest, i.scalar_dest = bits(w0, 0, 6), bits(w0, 8, 6)
    i.vector_saturate, i.scalar_saturate = bool(bits(w0, 24, 1)), bool(bits(w0, 25, 1))
    def take(n):
        return _operand(sel[n], reg[n], swiz[n], neg[n], addressed[n], abs_constants, address_register)
    i.vector_op = VECTOR_OPS[vector_opc] if vector_opc < len(VECTOR_OPS) else "vop%d" % vector_opc
    if vector_opc < len(VECTOR_OPS):
        i.vector_operands = [take(n + 1) for n in range(VECTOR_ARITY[vector_opc])]
    i.scalar_op = SCALAR_OPS[scalar_opc] if scalar_opc < len(SCALAR_OPS) else "op%d" % scalar_opc
    arity = SCALAR_ARITY[scalar_opc] if scalar_opc < len(SCALAR_ARITY) else 0
    # The scalar operand's components: swizzled component 3, then 0.
    a, b = (bits(swiz[3], 6, 2) + 3) & 3, bits(swiz[3], 0, 2)
    if 42 <= scalar_opc <= 47:
        # A constant (src3, component a) and a temporary register (component b)
        # whose index is put together from other fields. Both take the
        # constants' absolute value bit and src3's negation.
        reg2 = (scalar_opc & 1) | (sel[3] << 1) | (swiz[3] & 0x3C)
        i.scalar_operands = [
            Operand(False, reg[3], [a] * 4, bool(neg[3]), bool(abs_constants),
                    ("a0" if address_register else "aL") if addressed[3] else ""),
            Operand(True, reg2, [b] * 4, bool(neg[3]), bool(abs_constants))]
    elif arity:
        base = take(3)
        i.scalar_operands = [Operand(base.is_temp, base.index, [c] * 4, base.negate,
                                     base.absolute, base.relative) for c in ([a, b][:arity])]
    return i

def decode_fetch(address, w0, w1, w2, previous_vfetch):
    f = _decode_fetch(address, w0, w1, w2, previous_vfetch)
    f.relative = bool(bits(w0, 11, 1) or bits(w0, 18, 1))  # source or destination register
    f.words = (w0, w1, w2)
    return f

def _decode_fetch(address, w0, w1, w2, previous_vfetch):
    opcode = bits(w0, 0, 5)
    if opcode == 0:
        mini = bool(bits(w1, 30, 1))
        exp = bits(w1, 24, 6)
        f = VertexFetch(address, bits(w0, 20, 5) * 3 + bits(w0, 25, 2), bits(w0, 5, 6),
                        bits(w0, 30, 2), bits(w0, 12, 6), _dest_swizzle(bits(w1, 0, 12)),
                        bits(w1, 16, 6), bits(w2, 0, 8), _signed(bits(w2, 8, 23), 23), bool(bits(w1, 12, 1)),
                        bool(bits(w1, 13, 1)), exp - 64 if exp & 32 else exp, mini,
                        bool(bits(w1, 31, 1)), bool(bits(w2, 31, 1)), bool(bits(w1, 14, 1)),
                        bool(bits(w1, 15, 1)))
        if mini and previous_vfetch:
            f.fetch_constant, f.stride_words = previous_vfetch.fetch_constant, previous_vfetch.stride_words
            f.source, f.source_component = previous_vfetch.source, previous_vfetch.source_component
        return f
    lod = bits(w2, 2, 7)
    def off(v):
        return (v - 32 if v & 16 else v) / 2.0
    return TextureFetch(address, opcode, bits(w0, 20, 5), bits(w0, 5, 6),
                        [bits(w0, 26 + 2 * k, 2) for k in range(3)], bits(w0, 12, 6),
                        _dest_swizzle(bits(w1, 0, 12)), bits(w2, 14, 2), bool(bits(w0, 25, 1)),
                        (bits(w1, 12, 2), bits(w1, 14, 2), bits(w1, 16, 2), bits(w1, 18, 3)),
                        bool(bits(w1, 28, 1)), bool(bits(w1, 29, 1)), bool(bits(w2, 0, 1)),
                        (lod - 128 if lod & 64 else lod) / 16.0,
                        (off(bits(w2, 16, 5)), off(bits(w2, 21, 5)), off(bits(w2, 26, 5))),
                        bool(bits(w1, 31, 1)), bool(bits(w2, 31, 1)))

def decode(words):
    """The control flow list, each exec with its instructions."""
    cfs, first_instruction, previous_vfetch = [], len(words) // 3, None
    index = 0
    while index < first_instruction * 2:
        w = words[(index // 2) * 3:(index // 2) * 3 + 3]
        v = (w[0] | (w[1] & 0xFFFF) << 32) if index % 2 == 0 else ((w[1] >> 16) | w[2] << 16)
        d0, d1 = v & 0xFFFFFFFF, v >> 32
        c = ControlFlow(index, CF[d1 >> 12])
        if c.op in ("exec", "exece", "cexec", "cexece", "cexec_pred", "cexece_pred",
                    "cexec_pred_clean", "cexece_pred_clean"):
            c.address, c.count, c.sequence = bits(d0, 0, 12), bits(d0, 12, 3), bits(d0, 16, 12)
            if c.op in ("cexec", "cexece", "cexec_pred_clean", "cexece_pred_clean"):
                c.bool_address, c.condition = bits(d1, 2, 8), bool(bits(d1, 10, 1))
            elif c.op in ("cexec_pred", "cexece_pred"):
                c.condition = bool(bits(d1, 10, 1))
            c.predicate_clean = bool(bits(d1, 9, 1)) if c.op in ("exec", "exece") else True
            if c.count:
                first_instruction = min(first_instruction, c.address)
            for k in range(c.count):
                a = c.address + k
                iw = words[a * 3:a * 3 + 3]
                if bits(c.sequence, 2 * k, 1):
                    ins = decode_fetch(a, *iw, previous_vfetch)
                    if ins.kind == "vfetch":
                        previous_vfetch = ins if not ins.mini else previous_vfetch
                else:
                    ins = decode_alu(a, *iw)
                c.instructions.append(ins)
        elif c.op == "loop":
            c.address, c.loop_id = bits(d0, 0, 13), bits(d0, 16, 5)
            c.is_repeat = bool(bits(d0, 13, 1))
        elif c.op == "endloop":
            c.address, c.loop_id = bits(d0, 0, 13), bits(d0, 16, 5)
            c.predicated, c.condition = bool(bits(d0, 21, 1)), bool(bits(d1, 10, 1))
        elif c.op in ("ccall", "cjmp"):
            c.address, c.unconditional = bits(d0, 0, 13), bool(bits(d0, 13, 1))
            c.predicated = bool(bits(d0, 14, 1))
            c.bool_address, c.condition = bits(d1, 2, 8), bool(bits(d1, 10, 1))
        elif c.op == "alloc":
            c.alloc_size, c.alloc_type = bits(d0, 0, 3), bits(d1, 9, 2)
        cfs.append(c)
        index += 1
    return cfs
