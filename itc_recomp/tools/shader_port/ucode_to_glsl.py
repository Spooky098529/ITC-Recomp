"""Writes a shader's microcode as GLSL for the native renderer.

One instruction becomes one statement (or a short block), with the constant
table's names for the constants and the file's literal constants written in
place. The result includes xe_interface.glsl, which holds what isn't the
shader's own logic: the buffers, the fetches, the instructions that need
more than an operator, and what happens before and after the shader.

The port computes what the SDK's SPIR-V translator does, step for step, so
the two give the same values (tools/shader_port/check compares them).
Anything the translator does that the game's shaders don't use raises
Unsupported, and the renderer keeps translating that shader."""
import re
import struct
from xenos_ucode import decode

XYZW = "xyzw"
GLSL_WORDS = {"texture", "sampler", "input", "output", "filter", "sample", "common", "active",
              "partition", "uniform", "buffer", "shared", "precise", "smooth", "flat"}


class Unsupported(Exception):
    pass


def f32(v):
    return struct.unpack("<f", struct.pack("<f", v))[0]


def fnum(v):
    """A float literal that reads back as the same 32-bit float."""
    import math
    v = f32(v)
    if v != v or math.isinf(v):
        return "uintBitsToFloat(0x%08Xu)" % struct.unpack("<I", struct.pack("<f", v))[0]
    if v == 0:
        return "-0.0" if math.copysign(1, v) < 0 else "0.0"
    for digits in range(6, 10):
        text = "%.*g" % (digits, v)
        if f32(float(text)) == v:
            break
    if "e" not in text and "." not in text:
        text += ".0"
    return text


def ident(name):
    name = re.sub(r"\W", "_", name)
    return name + "_c" if name in GLSL_WORDS or name.startswith(("gl_", "xe_")) else name


def swizzle(comps):
    return "".join(XYZW[c] for c in comps)


def vec_type(n):
    return "float" if n == 1 else "vec%d" % n


EXEC_OPS = ("exec", "exece", "cexec", "cexece", "cexec_pred", "cexece_pred",
            "cexec_pred_clean", "cexece_pred_clean")
PS_READERS = ("adds_prev", "muls_prev", "muls_prev2", "subs_prev")
# Vector operations that do something without a write mask.
VECTOR_STATE_OPS = ("setp_eq_push", "setp_ne_push", "setp_gt_push", "setp_ge_push",
                    "kill_eq", "kill_gt", "kill_ge", "kill_ne", "maxa")
SCALAR_STATE_OPS = ("maxas", "maxasf", "setp_eq", "setp_ne", "setp_gt", "setp_ge", "setp_inv",
                    "setp_pop", "setp_clr", "setp_rstr", "kills_eq", "kills_gt", "kills_ge",
                    "kills_ne", "kills_one")
VS_POSITION, VS_POINT = 62, 63
PS_DEPTH = 61


class Port:
    def __init__(self, shader, name):
        self.s, self.name = shader, name
        self.vertex = shader.is_vertex
        self.cfs = decode(shader.words)
        self.notes = []
        self._analyze()

    # --- what the shader uses -------------------------------------------------
    def _instructions(self):
        for c in self.cfs:
            for i in c.instructions:
                yield c, i

    def _analyze(self):
        bound = 0                      # the translator's register_count()
        self.float_used = set()        # absolute float constant reads
        self.dynamic_constants = False
        self.textures, self.samplers, self.vfetches = [], [], []
        self.exports = set()
        self.color_targets = set()
        self.uses_ps = self.kills = False
        self.labels = set()
        for c in self.cfs:
            if c.op in ("ccall", "ret"):
                raise Unsupported("subroutine calls")
            if c.op in ("cjmp", "loop", "endloop"):
                self.labels.add(c.address)
            for i in c.instructions:
                if i.kind == "alu":
                    if i.vector_dest_relative or i.scalar_dest_relative:
                        raise Unsupported("a result relative to the loop index")
                    for o in i.vector_operands + i.scalar_operands:
                        if o.is_temp:
                            if o.relative:
                                raise Unsupported("a register relative to the loop index")
                            bound = max(bound, o.index + 1)
                        elif o.relative:
                            self.dynamic_constants = True
                        else:
                            self.float_used.add(o.index)
                    if i.scalar_op in PS_READERS or (i.scalar_op == "retain_prev" and i.scalar_mask):
                        self.uses_ps = True
                    if i.vector_op.startswith("kill") or i.scalar_op.startswith("kills"):
                        self.kills = True
                    if i.vector_op.startswith("vop") or i.scalar_op.startswith("op"):
                        raise Unsupported("ALU opcode %s/%s" % (i.vector_op, i.scalar_op))
                    if i.export:
                        if i.vector_mask | i.scalar_mask | i.const0_mask | i.const1_mask:
                            self._export(i.vector_dest)
                    else:
                        if i.vector_mask:
                            bound = max(bound, i.vector_dest + 1)
                        if i.scalar_mask:
                            bound = max(bound, i.scalar_dest + 1)
                    continue
                if i.relative:
                    raise Unsupported("a fetch register relative to the loop index")
                bound = max(bound, i.source + 1)
                if any(sw != 7 for sw in i.dest_swizzle):
                    bound = max(bound, i.dest + 1)
                if any(sw == 6 for sw in i.dest_swizzle):
                    raise Unsupported("fetch destination swizzle 6")
                if i.kind == "vfetch":
                    if i.mini:
                        raise Unsupported("mini vertex fetches")
                    if i.predicated:
                        raise Unsupported("a predicated vertex fetch (its condition is patched)")
                    self.vfetches.append(i)
                    continue
                if i.opcode != 1:
                    raise Unsupported("texture fetch opcode %d" % i.opcode)
                if i.dimension == 2:
                    raise Unsupported("3D or stacked textures")
                if i.unnormalized or i.use_register_lod or i.use_register_gradients:
                    raise Unsupported("unnormalized coordinates or a register LOD or gradients")
                computed = i.use_computed_lod and not self.vertex
                mag, min_, mip, aniso = i.filters
                skey = (i.fetch_constant, mag, min_, mip, aniso if computed else 0)
                if skey not in self.samplers:
                    self.samplers.append(skey)
                dim = 1 if i.dimension == 0 else i.dimension   # 1D is bound as 2D
                for signed in (False, True):
                    if (i.fetch_constant, dim, signed) not in self.textures:
                        self.textures.append((i.fetch_constant, dim, signed))
        self.register_count = bound
        self.packed = {r: r for r in range(256)} if self.dynamic_constants else \
            {r: n for n, r in enumerate(sorted(self.float_used))}
        self.constant_names = {}
        taken = set()
        for c in self.s.constants:
            if c.register_set == "float4":
                for k in range(c.count):
                    name = ident(c.name) + ("_%d" % k if c.count > 1 else "")
                    if name not in taken and c.register + k not in self.s.literals:
                        self.constant_names[c.register + k] = name
                        taken.add(name)

    def _export(self, dest):
        if self.vertex:
            if dest == VS_POINT:
                raise Unsupported("the point size, edge flag and kill vertex export")
            if dest != VS_POSITION and dest >= 16:
                raise Unsupported("export register %d (memory export)" % dest)
        else:
            if dest == PS_DEPTH:
                raise Unsupported("depth export")
            if dest >= 4:
                raise Unsupported("export register %d (memory export)" % dest)
            self.color_targets.add(dest)
        self.exports.add(dest)

    # --- expressions ----------------------------------------------------------
    def constant(self, o):
        if o.relative:
            index = "a0" if o.relative == "a0" else "loop_aL.x"
            return "xe_fc[%s + %d]" % (index, o.index) if o.index else "xe_fc[%s]" % index
        if o.index in self.s.literals:
            return "lit%d" % o.index
        return self.constant_names.get(o.index, "c%d" % o.index)

    def operand(self, o, comps):
        """The operand's swizzled components `comps` (0-3), modifiers applied."""
        base = ("r%d" % o.index) if o.is_temp else self.constant(o)
        sw = swizzle(o.components[c] for c in comps)
        e = base if sw == "xyzw" else "%s.%s" % (base, sw)
        if o.absolute:
            e = "abs(%s)" % e
        if o.negate:
            e = "-%s" % e
        return e

    def scalar_operand(self, o, k):
        """Scalar operations' operand component k (0: the first, 1: the second)."""
        return self.operand(o, [k])

    def vector_value(self, i, comps, pre):
        """The vector operation's result for the result components `comps`,
        as a float or vecN expression; side effects go to `pre`."""
        op, a = i.vector_op, i.vector_operands
        n = len(comps)
        T = vec_type(n)
        A = [self.operand(o, comps) for o in a] if comps else []
        def smear(e):
            return e if n == 1 else "%s(%s)" % (T, e)
        def compare(fn, sym, negate=False):
            if n == 1:
                cond = "%s %s %s" % (A[0], sym, A[1])
                return "(%s ? 1.0 : 0.0)" % ("!(%s)" % cond if negate else cond)
            cond = "%s(%s, %s)" % (fn, A[0], A[1])
            return "%s(%s)" % (T, "not(%s)" % cond if negate else cond)
        def select(fn, sym):
            if n == 1:
                return "(%s %s 0.0 ? %s : %s)" % (A[0], sym, A[1], A[2])
            return "mix(%s, %s, %s(%s, %s(0.0)))" % (A[2], A[1], fn, A[0], T)
        if op == "add": return "%s + %s" % (A[0], A[1])
        if op == "mul": return "xe_mul(%s, %s)" % (A[0], A[1])
        if op == "mad": return "xe_mul(%s, %s) + %s" % (A[0], A[1], A[2])
        if op == "max": return A[0] if A[0] == A[1] else "xe_max(%s, %s)" % (A[0], A[1])
        if op == "min": return A[0] if A[0] == A[1] else "xe_min(%s, %s)" % (A[0], A[1])
        if op == "seq": return compare("equal", "==")
        if op == "sgt": return compare("greaterThan", ">")
        if op == "sge": return compare("greaterThanEqual", ">=")
        if op == "sne": return compare("equal", "==", negate=True)
        if op == "frc": return "fract(%s)" % A[0]
        if op == "trunc": return "trunc(%s)" % A[0]
        if op == "floor": return "floor(%s)" % A[0]
        if op == "cndeq": return select("equal", "==")
        if op == "cndge": return select("greaterThanEqual", ">=")
        if op == "cndgt": return select("greaterThan", ">")
        if op == "dp4":
            return smear("xe_dp4(%s, %s)" % tuple(self.operand(o, range(4)) for o in a[:2]))
        if op == "dp3":
            return smear("xe_dp3(%s, %s)" % tuple(self.operand(o, range(3)) for o in a[:2]))
        if op == "dp2add":
            return smear("xe_dp2add(%s, %s, %s)" % (self.operand(a[0], range(2)),
                                                    self.operand(a[1], range(2)),
                                                    self.operand(a[2], [0])))
        if op == "cube":
            e = "xe_cube(%s)" % self.operand(a[0], range(4))
            return e if n == 4 else "%s.%s" % (e, swizzle(comps))
        if op == "max4":
            # The largest of the components the swizzle names, in component order.
            o = a[0]
            plain = type(o)(o.is_temp, o.index, [0, 1, 2, 3], o.negate, o.absolute, o.relative)
            pre.append("vec4 m4 = %s;" % self.operand(plain, range(4)))
            used = sorted(set(o.components))
            e = "m4.%s" % XYZW[used[0]]
            for c in used[1:]:
                e = "max(%s, m4.%s)" % (e, XYZW[c])
            return smear(e)
        if op.startswith("setp_") and op.endswith("_push"):
            kind = op.split("_")[1]
            a0, b0 = self.operand(a[0], range(4)), self.operand(a[1], range(4))
            pre.append("vec4 pa = %s, pb = %s;" % (a0, b0))
            test = {"eq": "%s == 0.0", "ne": "!(%s == 0.0)", "gt": "%s > 0.0", "ge": "%s >= 0.0"}[kind]
            pre.append("p = pa.w == 0.0 && %s;" % (test % "pb.w"))
            if not comps:
                return None
            return smear("((pa.x == 0.0 && %s) ? -1.0 : pa.x) + 1.0" % (test % "pb.x"))
        if op.startswith("kill_"):
            kind = op.split("_")[1]
            a0, b0 = self.operand(a[0], range(4)), self.operand(a[1], range(4))
            fn = {"eq": "equal(%s, %s)", "ne": "not(equal(%s, %s))", "gt": "greaterThan(%s, %s)",
                  "ge": "greaterThanEqual(%s, %s)"}[kind]
            pre.append("if (any(%s)) xe_killed = true;" % (fn % (a0, b0)))
            return smear("0.0") if comps else None
        if op == "dst":
            # (1, a.y * b.y, a.z, b.w)
            parts = {0: "1.0", 1: "xe_mul(%s, %s)" % (self.operand(a[0], [1]), self.operand(a[1], [1])),
                     2: self.operand(a[0], [2]), 3: self.operand(a[1], [3])}
            return parts[comps[0]] if n == 1 else "%s(%s)" % (T, ", ".join(parts[c] for c in comps))
        if op == "maxa":
            pre.append("a0 = int(clamp(floor(%s + 0.5), -256.0, 255.0));" % self.operand(a[0], [3]))
            if not comps:
                return None
            return A[0] if A[0] == A[1] else "xe_max(%s, %s)" % (A[0], A[1])
        raise Unsupported("vector operation %s" % op)

    def scalar_value(self, i, pre):
        """The scalar operation's result (a float expression); side effects go
        to `pre`. None for retain_prev."""
        op, o = i.scalar_op, i.scalar_operands
        # The decoder gives the operand's components (or, for the constant
        # times register operations, the constant and the register) as two.
        a = self.operand(o[0], [0]) if o else ""
        b = self.operand(o[1], [0]) if len(o) > 1 else ""
        if op in ("mulsc0", "mulsc1", "addsc0", "addsc1", "subsc0", "subsc1"):
            return {"mul": "xe_mul(%s, %s)", "add": "%s + %s", "sub": "%s - %s"}[op[:3]] % (a, b)
        simple = {
            "adds": "%s + %s" % (a, b), "adds_prev": "%s + ps" % a,
            "subs": "%s - %s" % (a, b), "subs_prev": "%s - ps" % a,
            "muls": "xe_mul(%s, %s)" % (a, b), "muls_prev": "xe_mul(%s, ps)" % a,
            "muls_prev2": "xe_muls_prev2(%s, %s, ps)" % (a, b),
            "maxs": a if a == b else "xe_max(%s, %s)" % (a, b),
            "mins": a if a == b else "xe_min(%s, %s)" % (a, b),
            "seqs": "(%s == 0.0 ? 1.0 : 0.0)" % a, "sgts": "(%s > 0.0 ? 1.0 : 0.0)" % a,
            "sges": "(%s >= 0.0 ? 1.0 : 0.0)" % a, "snes": "(!(%s == 0.0) ? 1.0 : 0.0)" % a,
            "frcs": "fract(%s)" % a, "truncs": "trunc(%s)" % a, "floors": "floor(%s)" % a,
            "exp": "exp2(%s)" % a, "log": "log2(%s)" % a, "logc": "xe_log_clamped(%s)" % a,
            "rcp": "1.0 / %s" % a, "rcpc": "xe_clamp_infinity(1.0 / %s)" % a,
            "rcpf": "xe_infinity_to_zero(1.0 / %s)" % a,
            "rsq": "inversesqrt(%s)" % a, "rsqc": "xe_clamp_infinity(inversesqrt(%s))" % a,
            "rsqf": "xe_infinity_to_zero(inversesqrt(%s))" % a,
            "sqrt": "sqrt(%s)" % a, "sin": "sin(%s)" % a, "cos": "cos(%s)" % a,
        }
        if op in simple:
            return simple[op]
        if op == "retain_prev":
            return None
        if op in ("maxas", "maxasf"):
            pre.append("a0 = int(clamp(floor(%s%s), -256.0, 255.0));" % (a, " + 0.5" if op == "maxas" else ""))
            return a if a == b else "xe_max(%s, %s)" % (a, b)
        tests = {"eq": "%s == 0.0", "ne": "!(%s == 0.0)", "gt": "%s > 0.0", "ge": "%s >= 0.0"}
        if op[5:] in tests and op.startswith("setp_"):
            pre.append("p = %s;" % (tests[op[5:]] % a))
            return "(p ? 0.0 : 1.0)"
        if op == "setp_inv":
            pre.append("p = %s == 1.0;" % a)
            return "(p ? 0.0 : (%s == 0.0 ? 1.0 : %s))" % (a, a)
        if op == "setp_pop":
            pre.append("float pop = %s - 1.0;" % a)
            pre.append("p = pop <= 0.0;")
            return "(p ? 0.0 : pop)"
        if op == "setp_clr":
            pre.append("p = false;")
            return "kXeFltMax"
        if op == "setp_rstr":
            pre.append("p = %s == 0.0;" % a)
            return "(p ? 0.0 : %s)" % a
        if op.startswith("kills_"):
            k = op[6:]
            cond = "%s == 1.0" % a if k == "one" else tests[k] % a
            pre.append("if (%s) xe_killed = true;" % cond)
            return "0.0"
        raise Unsupported("scalar operation %s" % op)

    # --- statements -----------------------------------------------------------
    def dest(self, i, register):
        if not i.export:
            return "r%d" % register
        if self.vertex:
            return "xe_position" if register == VS_POSITION else "xe_interpolators[%d]" % register
        return "xe_color%d" % register

    def store(self, out, dest, mask, value, saturate, n_value):
        """dest's components in mask = value (n_value components, or 1 to
        smear)."""
        comps = [c for c in range(4) if (mask >> c) & 1]
        if not comps:
            return
        n = len(comps)
        if n_value == 1 and n > 1:
            value = "%s(%s)" % (vec_type(n), value)
        if saturate:
            value = "clamp(%s, 0.0, 1.0)" % value
        sw = "" if n == 4 else "." + swizzle(comps)
        if dest.startswith("r") or value in ("v", "s", "ps") or value[0].isdigit():
            out.append("%s%s = %s;" % (dest, sw, value))
        else:
            # An export: computed into a precise value first (globals can't
            # be precise).
            out.append("{ precise %s e = %s; %s%s = e; }" % (vec_type(n), value, dest, sw))

    def emit_alu(self, i, out):
        vop, sop = i.vector_op, i.scalar_op
        vcomps = [c for c in range(4) if (i.vector_mask >> c) & 1]
        vector_active = bool(vcomps) or vop in VECTOR_STATE_OPS
        if sop == "retain_prev" and not i.scalar_mask and not i.vector_mask and \
                not (i.const0_mask | i.const1_mask) and vop not in VECTOR_STATE_OPS:
            return  # a nop
        scalar_needed = sop != "retain_prev" and (self.uses_ps or i.scalar_mask or sop in SCALAR_STATE_OPS)
        body = []
        vpre, spre = [], []
        vvalue = self.vector_value(i, vcomps, vpre) if vector_active else None
        svalue = self.scalar_value(i, spre) if scalar_needed else None
        dest_v, dest_s = self.dest(i, i.vector_dest), self.dest(i, i.scalar_dest)
        n = len(vcomps)
        if vvalue is not None and svalue is not None:
            # Both results are worked out before either is written.
            body += vpre
            body.append("precise %s v = %s;" % (vec_type(n), vvalue))
            body += spre
            if svalue is not None:
                body.append("precise float s = %s;" % svalue)
                if self.uses_ps:
                    body.append("ps = s;")
            self.store(body, dest_v, i.vector_mask, "v", i.vector_saturate, n)
            self.store_constants(body, i)
            self.store(body, dest_s, i.scalar_mask, "s", i.scalar_saturate, 1)
        else:
            body += vpre
            if vvalue is not None:
                self.store(body, dest_v, i.vector_mask, vvalue, i.vector_saturate, n)
            self.store_constants(body, i)
            body += spre
            if svalue is not None and self.uses_ps:
                body.append("ps = %s;" % svalue)
                svalue = "ps"
            if sop == "retain_prev":
                svalue = "ps"
            if svalue is not None:
                self.store(body, dest_s, i.scalar_mask, svalue, i.scalar_saturate, 1)
        if any(s.startswith(("vec", "float", "precise")) for s in body) and len(body) > 1:
            body = ["{ " + " ".join(body) + " }"]
        out.extend(body)

    def store_constants(self, out, i):
        for mask, value in ((i.const0_mask, "0.0"), (i.const1_mask, "1.0")):
            self.store(out, self.dest(i, i.vector_dest), mask, value, False, 1)

    def store_fetch(self, out, dest, dest_swizzle, value):
        """A fetch result to dest by its destination swizzle (0-3 a result
        component, 4 zero, 5 one, 7 keep)."""
        fetched = [c for c in range(4) if dest_swizzle[c] < 4]
        constant = [c for c in range(4) if dest_swizzle[c] in (4, 5)]
        lines = []
        if fetched:
            src = swizzle(dest_swizzle[c] for c in fetched)
            d = swizzle(fetched)
            if len(fetched) == 4 and src == "xyzw":
                lines.append("r%d = %s;" % (dest, value))
            else:
                lines.append("r%d%s = %s.%s;" % (dest, "" if d == "xyzw" else "." + d, value, src))
        for v in (4, 5):
            comps = [c for c in constant if dest_swizzle[c] == v]
            if comps:
                k = "0.0" if v == 4 else "1.0"
                lines.append("r%d.%s = %s;" % (dest, swizzle(comps),
                                               k if len(comps) == 1 else "%s(%s)" % (vec_type(len(comps)), k)))
        out.extend(lines)

    def emit_fetch(self, i, out):
        used = 0
        for sw in i.dest_swizzle:
            if sw < 4:
                used |= 1 << sw
        if i.kind == "vfetch":
            slot = self.vfetches.index(i)
            if not used:
                self.store_fetch(out, i.dest, i.dest_swizzle, "vec4(0.0)")
                return
            index = "r%d.%s" % (i.source, XYZW[i.source_component])
            call = ("xe_vfetch(%d, XE_VF%d_CONSTANT, XE_VF%d_OFFSET, XE_VF%d_STRIDE, XE_VF%d_FORMAT, "
                    "XE_VF%d_FLAGS, XE_VF%d_EXP_ADJUST, XE_VF%d_SWIZZLE, %s, %du)" % ((slot,) * 8 + (index, used)))
            lines = []
            self.store_fetch(lines, i.dest, i.dest_swizzle, "f")
            if len(lines) == 1 and lines[0].count(" = f") == 1:
                out.append(lines[0].replace(" = f", " = " + call, 1))
            else:
                out.append("{ vec4 f = %s; %s }" % (call, " ".join(lines)))
            return
        computed = i.use_computed_lod and not self.vertex
        mag, min_, mip, aniso = i.filters
        sampler = self.samplers.index((i.fetch_constant, mag, min_, mip, aniso if computed else 0))
        dim = 1 if i.dimension == 0 else i.dimension
        tu = self.textures.index((i.fetch_constant, dim, False))
        ts = self.textures.index((i.fetch_constant, dim, True))
        rounding = 1.5 / 1024.0
        ox, oy, oz = i.offsets
        if i.dimension == 0:
            fn, coords, offset = "xe_tfetch1d", 1, fnum(f32(f32(ox) + f32(rounding)))
        elif i.dimension == 1:
            fn, coords = "xe_tfetch2d", 2
            offset = "vec2(%s, %s)" % (fnum(f32(f32(ox) + f32(rounding))), fnum(f32(f32(oy) + f32(rounding))))
        else:
            fn, coords = "xe_tfetchcube", 3
            offset = "vec3(%s, %s, %s)" % (fnum(f32(f32(ox) + f32(rounding))),
                                           fnum(f32(f32(oy) + f32(rounding))), fnum(oz))
        source = "r%d.%s" % (i.source, swizzle(i.source_components[:coords]))
        if not used:
            self.store_fetch(out, i.dest, i.dest_swizzle, "vec4(0.0)")
            return
        call = "%s(xe_texture%d, xe_texture%d, xe_sampler%d, %du, %s, %s, %s, %s)" % (
            fn, tu, ts, sampler, i.fetch_constant, source, offset, fnum(i.lod_bias),
            "true" if computed else "false")
        lines = []
        self.store_fetch(lines, i.dest, i.dest_swizzle, "t")
        if len(lines) == 1 and lines[0].count(" = t") == 1:
            out.append(lines[0].replace(" = t", " = " + call, 1))
        else:
            out.append("{ vec4 t = %s; %s }" % (call, " ".join(lines)))

    @staticmethod
    def writes_predicate(i):
        return i.kind == "alu" and (i.scalar_op.startswith("setp") or
                                    (i.vector_op.startswith("setp") and i.vector_op.endswith("push")))

    def emit_instructions(self, instructions, out, exec_predicate=None):
        """An exec's instructions; a run of instructions under the same
        predicate shares one if, up to one that changes the predicate. In an
        exec that already tests the predicate, instructions that test it the
        same way need no if of their own until the predicate changes."""
        k = 0
        predicate_unchanged = True
        while k < len(instructions):
            i = instructions[k]
            if i.predicated and predicate_unchanged and exec_predicate == i.predicate_condition:
                (self.emit_alu if i.kind == "alu" else self.emit_fetch)(i, out)
                predicate_unchanged = not self.writes_predicate(i)
                k += 1
                continue
            if self.writes_predicate(i):
                predicate_unchanged = False
            if not i.predicated:
                (self.emit_alu if i.kind == "alu" else self.emit_fetch)(i, out)
                k += 1
                continue
            run = []
            while k < len(instructions) and instructions[k].predicated and \
                    instructions[k].predicate_condition == i.predicate_condition:
                j = instructions[k]
                (self.emit_alu if j.kind == "alu" else self.emit_fetch)(j, run)
                k += 1
                if self.writes_predicate(j):
                    predicate_unchanged = False
                    break
            if run:
                out.append("if (%sp) {" % ("" if i.predicate_condition else "!"))
                out.extend("  " + s for s in run)
                out.append("}")

    def bool_name(self, n):
        """A bool constant's name from the constant table, as a comment (a
        pixel shader's bool constants are 128 on)."""
        base = 0 if self.vertex else 128
        for c in self.s.constants:
            if c.register_set == "bool" and base + c.register <= n < base + c.register + c.count:
                return " /* %s */" % c.name
        return ""

    def condition(self, c):
        if c.op in ("cexec", "cexece", "cexec_pred_clean", "cexece_pred_clean") or \
                (c.op == "cjmp" and not c.predicated and not c.unconditional):
            return "%sxe_bool(%du%s)" % ("" if c.condition else "!", c.bool_address,
                                          self.bool_name(c.bool_address))
        if c.op in ("cexec_pred", "cexece_pred") or (c.op == "cjmp" and c.predicated and not c.unconditional):
            return "%sp" % ("" if c.condition else "!")
        return ""

    def body(self):
        out = []
        switch = bool(self.labels)
        for c in self.cfs:
            if switch and c.index in self.labels and c.index != 0:
                out.append("case %d:" % c.index)
            cond = self.condition(c)
            block = []
            if c.op in EXEC_OPS:
                self.emit_instructions(c.instructions, block,
                                       c.condition if c.op in ("cexec_pred", "cexece_pred") else None)
                if c.op.startswith(("exece", "cexece")):
                    block.append("break;  // the end")
            elif c.op == "loop":
                block += ["// loop %d" % c.loop_id,
                          "loop_count = uvec4(xe_loop_constant(%du) & 0xFFu, loop_count.xyz);" % c.loop_id,
                          "loop_aL = ivec4(%s, loop_aL.xyz);" % (
                              "loop_aL.x" if c.is_repeat else
                              "int((xe_loop_constant(%du) >> 8u) & 0xFFu)" % c.loop_id),
                          "if (loop_count.x == 0u) { pc = %d; continue; }" % c.address]
            elif c.op == "endloop":
                if c.predicated and c.condition:
                    again = "!(count == 0u || p)"
                elif c.predicated:
                    again = "count != 0u && p"
                else:
                    again = "count != 0u"
                block += ["// end of loop %d" % c.loop_id,
                          "{ uint count = loop_count.x - 1u;",
                          "  if (%s) {" % again,
                          "    loop_count.x = count;",
                          "    loop_aL.x += bitfieldExtract(int(xe_loop_constant(%du)), 16, 8);" % c.loop_id,
                          "    pc = %d; continue;" % c.address,
                          "  }",
                          "  loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }"]
            elif c.op == "cjmp":
                block.append("pc = %d; continue;" % c.address)
            elif c.op in ("nop", "alloc", "vfetch_done"):
                continue
            else:
                raise Unsupported("control flow %s" % c.op)
            if not block:
                continue
            if cond:
                out.append("if (%s) {" % cond)
                out.extend("  " + s for s in block)
                out.append("}")
            else:
                out.extend(block)
                if not switch and c.op.startswith("exece"):
                    break
        return out

    # --- the file ---------------------------------------------------------------
    def glsl(self, title):
        body = self.body()
        L = ["#version 450", "// " + title,
             "// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.",
             "#extension GL_GOOGLE_include_directive : require",
             "#define XE_%s" % ("VERTEX" if self.vertex else "PIXEL"),
             "#define XE_FLOAT_CONSTANTS %d" % max(len(self.packed), 1),
             '#include "xe_interface.glsl"', ""]
        tset = 2 if self.vertex else 3
        dims = {1: "texture2DArray", 3: "textureCube"}
        for n, (fc, dim, signed) in enumerate(self.textures):
            L.append("layout(set = %d, binding = %d) uniform %s xe_texture%d;  // fetch constant %d, %s"
                     % (tset, n, dims[dim], n, fc, "signed" if signed else "unsigned"))
        for n, (fc, mag, min_, mip, aniso) in enumerate(self.samplers):
            L.append("layout(set = %d, binding = %d) uniform sampler xe_sampler%d;  // fetch constant %d"
                     % (tset, len(self.textures) + n, n, fc))
        if self.textures:
            L.append("")
        for c in self.s.constants:
            if c.register_set != "float4":
                L.append("// %s: %s %d%s" % (c.name, c.register_set, c.register,
                                             " (%d)" % c.count if c.count > 1 else ""))
        for r in sorted(self.float_used):
            if r in self.s.literals:
                L.append("const vec4 lit%d = vec4(%s);" % (r, ", ".join(fnum(v) for v in self.s.literals[r])))
            else:
                L.append("#define %s xe_fc[%d]  // c%d" % (self.constant_names.get(r, "c%d" % r),
                                                            self.packed[r], r))
        L.append("")
        regs = range(self.register_count)
        switch = bool(self.labels)
        state = ["  bool p = false;", "  int a0 = 0;"]
        if self.uses_ps:
            state.append("  precise float ps = 0.0;")
        if switch:
            state.append("  int pc = 0;")
            state.append("  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first")
            state.append("  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)")
        if self.vertex:
            for n, f in enumerate(self.vfetches):
                w0, w1, w2 = f.words
                flags = int(f.is_signed) | (int(f.is_integer) << 1) | (int(f.signed_rf_mode) << 2) | \
                    (int(f.index_round) << 3)
                L.append("// Vertex fetch %d (instruction %d): its fields as in the shader file, until "
                         "the renderer sets them from the patched instruction." % (n, f.address))
                for k, (field, ty, value) in enumerate((
                        ("CONSTANT", "uint", "%du" % f.fetch_constant), ("OFFSET", "int", str(f.offset_words)),
                        ("STRIDE", "uint", "%du" % f.stride_words), ("FORMAT", "uint", "%du" % f.format),
                        ("FLAGS", "uint", "%du" % flags), ("EXP_ADJUST", "int", str(f.exp_adjust)),
                        ("SWIZZLE", "uint", "0x688u"))):
                    L.append("layout(constant_id = %d) const %s XE_VF%d_%s = %s;" % (100 + 10 * n + k, ty, n, field, value))
                # The fetch's attribute: the element's raw words, a vertex of
                # the rectangle each (xe_interface.glsl, XE_VF_IN).
                for g in range(3):
                    L.append("layout(location = %d) in uvec4 xe_vf_in_%d_%d;" % (3 * n + g, n, g))
            L += ["// Drawing a rectangle list: each invocation runs the shader for the",
                  "// rectangle's three vertices and makes the fourth.",
                  "layout(constant_id = 2) const bool XE_RECTANGLE_LIST = false;", ""]
            L += ["// The interpolators, at locations the renderer sets for the pixel shader.",
                  ] + ["layout(location = %d) out vec4 xe_out_interpolator_%d;" % (k, k) for k in range(16)]
            L += ["", "vec4 xe_position = vec4(0.0);", "vec4 xe_interpolators[16];", ""]
            L.append("void xe_shader(float vertex_index) {")
            L.append("  for (int i = 0; i < 16; i++) xe_interpolators[i] = vec4(0.0);")
            for r in regs:
                L.append("  precise vec4 r%d = %s;" % (r, "vec4(vertex_index, 0.0, 0.0, 0.0)" if r == 0 else "vec4(0.0)"))
        else:
            L += ["// The interpolators the vertex shader writes (a bit each), and the register",
                  "// the generated parameter goes in (-1: none).",
                  "layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;",
                  "layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;"]
            inputs = [r for r in regs if r < 16]
            for r in inputs:
                L.append("layout(location = %d) in vec4 xe_in_interpolator_%d;" % (r, r))
            for t in sorted(self.color_targets):
                L.append("layout(location = %d) out vec4 xe_out_color_%d;" % (t, t))
            L.append("")
            for t in sorted(self.color_targets):
                L.append("vec4 xe_color%d = vec4(0.0);" % t)
            L.append("bool xe_killed = false;")
            L.append("")
            L.append("vec4 xe_input(int n, vec4 interpolator) {")
            L.append("  if (n == XE_PARAM_GEN_REGISTER) return xe_param_gen();")
            L.append("  return ((XE_INTERPOLATOR_MASK >> uint(n)) & 1u) != 0u ? interpolator : vec4(0.0);")
            L.append("}")
            L.append("")
            L.append("void xe_shader() {")
            for r in regs:
                init = ("xe_input(%d, xe_in_interpolator_%d)" % (r, r)) if r < 16 else "vec4(0.0)"
                L.append("  precise vec4 r%d = %s;" % (r, init))
        if not any("a0" in b for b in body):
            state = [x for x in state if "a0" not in x]
        L.extend(state)
        if switch:
            L += ["  while (true) {", "    switch (pc) {", "    case 0:"]
            L.extend("      " + s if not s.startswith("case ") else "    " + s for s in body)
            L += ["    }", "    break;", "  }"]
        else:
            L.append("  do {")
            L.extend("    " + s for s in body)
            L.append("  } while (false);")
        L += ["}", ""]
        if self.vertex:
            L += [
                "void xe_write_interpolators(vec4 interpolators[16]) {",
            ] + ["  xe_out_interpolator_%d = interpolators[%d];" % (k, k) for k in range(16)] + [
                "}", "",
                "void main() {",
                "  // A rectangle list runs the shader for the rectangle's three vertices",
                "  // (an instance a rectangle, its vertices' attributes at instance rate).",
                "  vec4 positions[3];",
                "  vec4 interpolators[3][16];",
                "  uint runs = XE_RECTANGLE_LIST ? 3u : 1u;",
                "  for (uint g = 0u; g < runs; g++) {",
                "    xe_vf_slot = g;",
                "    xe_shader(XE_RECTANGLE_LIST ? float(uint(gl_InstanceIndex) * 3u + g) : float(gl_VertexIndex));",
                "    positions[g] = xe_host_position(xe_position);",
                "    interpolators[g] = xe_interpolators;",
                "  }",
                "  if (!XE_RECTANGLE_LIST) {",
                "    gl_Position = positions[0];",
                "    xe_write_interpolators(interpolators[0]);",
                "    return;",
                "  }",
                "  // The rectangle's corners in order from the one opposite the longest edge;",
                "  // the fourth vertex is v1 - v0 + v2.",
                "  float edges[3];",
                "  for (int e = 0; e < 3; e++) {",
                "    vec2 d = positions[(2 + e) % 3].xy - positions[(1 + e) % 3].xy;",
                "    edges[e] = d.x * d.x + d.y * d.y;",
                "  }",
                "  int v0 = (edges[0] > edges[1] && edges[0] > edges[2]) ? 0 : (edges[1] > edges[2] ? 1 : 2);",
                "  int v1 = (v0 + 1) % 3, v2 = (v0 + 2) % 3;",
                "  uint corner = uint(gl_VertexIndex) & 3u;",
                "  int pick = corner == 0u ? v0 : (corner == 1u ? v1 : v2);",
                "  precise vec4 synthetic_position = (positions[v1] - positions[v0]) + positions[v2];",
                "  gl_Position = corner == 3u ? synthetic_position : positions[pick];",
                "  vec4 out_interpolators[16];",
                "  for (int i = 0; i < 16; i++) {",
                "    precise vec4 synthetic = (interpolators[v1][i] - interpolators[v0][i]) + interpolators[v2][i];",
                "    out_interpolators[i] = corner == 3u ? synthetic : interpolators[pick][i];",
                "  }",
                "  xe_write_interpolators(out_interpolators);",
                "}"]
        else:
            L += ["void main() {", "  xe_shader();"]
            if self.kills:
                L.append("  if (xe_killed) discard;")
            if 0 in self.color_targets:
                L.append("  if (!xe_alpha_test_passes(xe_color0.a)) discard;")
            for t in sorted(self.color_targets):
                L.append("  xe_out_color_%d = xe_color_output(xe_color%d, %d);" % (t, t, t))
            L.append("}")
        return "\n".join(L) + "\n"
