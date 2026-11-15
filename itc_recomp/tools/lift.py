#!/usr/bin/env python3
"""The lift sweep's tool (docs/loops/lift-sweep.md): translates recompiled
functions from itc_recomp/generated/ into plain C++ in itc_recomp/src/lifted/.

usage:
  lift.py check sub_X ...           what the tool makes of each: its
                                    parameters and return, or why it refuses
  lift.py survey                    the refusals over every unported function
                                    the static map lists, by reason
  lift.py emit <batch> sub_X ...    writes src/lifted/batch_<batch>.cpp with
                                    those functions, and functions.h

How a function is translated (it reads generated/, never edits it):
  registers   one local each: r0-r31, ctr and the reservation as uint64_t,
              f0-f31 as double, cr0-cr7 as lifted::Cr, the XER's ca, so and ov
              as uint8_t; the generated code's views (.u32, .s16, .f64 ...)
              become casts and bit casts, and partial writes keep the rest
              of the register, as the union did
  parameters  the stack pointer (r1), r2, r13, r3-r10 and f1-f13 when the
              function may read them before writing them (a call's arguments
              count: a direct call's are its callee's parameters, a call by
              address or an import passes them all)
  return      r3 and f1 when the function writes them (a call writes what
              its callee returns); void, uint64_t, double or lifted::Result
  calls       a lifted function directly; any other function through the
              dispatch table (lifted::Call, by address); imports through
              lifted::Import; the register save and restore helpers left
              out (their stack bytes are dead)
  lr          return addresses are dead in C++ (a return is a return): the
              link register's writes are left out and its reads are 0, so
              its saves store 0
  refuses     vector code, setjmp, the machine state register, the global
              lock macros, the flush mode turned on, a read of a register
              another function left (after a call, other than r3 and f1), a
              read of a caller's register outside a save (a nonvolatile, the
              carry, the CTR), and any statement it doesn't know
"""
import collections
import csv
import glob
import json
import os
import pickle
import re
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(TOOLS, "..", ".."))
GEN = os.path.join(ROOT, "itc_recomp", "generated", "default")
SRC = os.path.join(ROOT, "itc_recomp", "src")
OUT = os.path.join(SRC, "lifted")
TSV = os.path.join(ROOT, "itc_recomp", "coverage", "static_map.tsv")
CACHE = os.path.join(ROOT, "itc_recomp", "out", "lift_bodies.pickle")
SIGNATURES = os.path.join(OUT, "signatures.json")

GPRS = [f"r{i}" for i in range(32)]
VRS = [f"v{i}" for i in range(128)]
VR_VOLATILE = {f"v{i}" for i in range(14)} | {f"v{i}" for i in range(32, 64)}
FPRS = [f"f{i}" for i in range(32)]
CRS = [f"cr{i}" for i in range(8)]
GPR_LIKE = set(GPRS) | {"ctr", "reserved"}
XER = {"ca", "so", "ov"}
PARAM_ORDER = (["r1", "r2", "r13"] + [f"r{i}" for i in range(3, 11)] + [f"f{i}" for i in range(1, 14)]
               + sorted(VR_VOLATILE, key=lambda v: int(v[1:])))
PARAMS = set(PARAM_ORDER)
CALL_GPRS = [f"r{i}" for i in range(3, 11)]
CALL_FPRS = [f"f{i}" for i in range(1, 14)]
ALL_ARGS = {"r1", "r13"} | set(CALL_GPRS) | set(CALL_FPRS)
VOLATILE = ({"r0"} | {f"r{i}" for i in range(4, 13)} | {"f0"} | {f"f{i}" for i in range(2, 14)}
            | {"cr0", "cr1", "cr5", "cr6", "cr7", "ctr", "reserved", "ca", "ov"} | VR_VOLATILE)
NONVOLATILE = ({f"r{i}" for i in range(14, 32)} | {f"f{i}" for i in range(14, 32)}
               | {"cr2", "cr3", "cr4"} | (set(VRS) - VR_VOLATILE))
ZERO_AT_ENTRY = {"so", "ov", "vscr_sat"}  # the XER's overflow bits and the vector
# saturation flag: never read for a result by the game's code

TOKEN = re.compile(r"ctx\.(\w+)(?:\.(\w+))?")
REFUSE_TEXT = [
    (re.compile(r"ppc_setjmp|ppc_longjmp"), "setjmp"),
    (re.compile(r"ctx\.msr"), "the machine state register"),
    (re.compile(r"REX_(ENTER|LEAVE|CHECK)_GLOBAL_LOCK"), "the global lock"),
    (re.compile(r"last_indirect_target|REX_UNIMPLEMENTED|REX_MM_|REX_QUERY_TIMEBASE"), "an SDK macro"),
]


class Refused(Exception):
    pass


# --- Reading generated/ ---

def load_bodies():
    sources = sorted(glob.glob(os.path.join(GEN, "itc_recomp.*.cpp")))
    stamp = max(os.path.getmtime(p) for p in sources)
    if os.path.exists(CACHE) and os.path.getmtime(CACHE) > stamp:
        with open(CACHE, "rb") as f:
            return pickle.load(f)
    bodies = {}
    for path in sources:
        name = None
        lines = []
        with open(path) as f:
            for line in f:
                if name is None:
                    m = re.match(r"DEFINE_REX_FUNC\((\w+)\) \{", line)
                    if m:
                        name, lines = m.group(1), []
                elif line.rstrip() == "}":
                    bodies[name] = lines
                    name = None
                else:
                    lines.append(line.rstrip("\n"))
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    with open(CACHE, "wb") as f:
        pickle.dump(bodies, f)
    return bodies


def overridden():
    """Functions with an override of ours outside src/lifted/: a REX_FUNC
    written out or made by a macro (src_edges.py's reading of src/)."""
    sys.path.insert(0, TOOLS)
    import src_edges
    defs = src_edges.parse_all()[0]
    return {d.adapter for d in defs
            if d.adapter and not d.file.replace(os.sep, "/").startswith("lifted/")
            and "/lifted/" not in d.file.replace(os.sep, "/")}


# --- Statements ---

class Stmt:
    def __init__(self, kind, code, comment, **kw):
        self.kind = kind          # label goto if return trap call switch case end plain skip
        self.code = code
        self.comment = comment
        self.__dict__.update(kw)


CALL_RE = re.compile(r"^(sub_[0-9A-F]{8}|__imp__\w+|__(?:save|rest)\w+)\(ctx, base\);$")
INDIRECT_RE = re.compile(r"^REX_CALL_INDIRECT_FUNC\((.*)\);$")
IF_GOTO_RE = re.compile(r"^if \((.*)\) goto (loc_[0-9A-F]+);$")


def parse(name, lines):
    stmts = []
    comment = None
    braces = []
    pending = ""
    for raw in lines:
        s = raw.strip()
        if not s:
            continue
        if s.startswith("//") and not pending:
            comment = s[2:].strip()
            continue
        # A statement split over lines: joined until its parentheses close.
        if pending or s.count("(") > s.count(")"):
            pending = (pending + " " + s).strip()
            if pending.count("(") > pending.count(")"):
                continue
            s, pending = pending, ""
        for pattern, reason in REFUSE_TEXT:
            if pattern.search(s):
                raise Refused(reason)
        if s == "REX_FUNC_PROLOGUE();":
            continue
        m = CALL_RE.match(s)
        if m:
            target = m.group(1)
            if target.startswith("__save") or target.startswith("__rest"):
                stmts.append(Stmt("skip", s, comment))
            elif target.startswith("__imp__"):
                stmts.append(Stmt("call", s, comment, how="import", target=target))
            else:
                stmts.append(Stmt("call", s, comment, how="direct", target=target))
        elif INDIRECT_RE.match(s):
            stmts.append(Stmt("call", s, comment, how="indirect", target=INDIRECT_RE.match(s).group(1)))
        elif re.match(r"^loc_[0-9A-F]+:$", s):
            stmts.append(Stmt("label", s, comment, label=s[:-1]))
        elif IF_GOTO_RE.match(s):
            m = IF_GOTO_RE.match(s)
            stmts.append(Stmt("if", s, comment, cond=m.group(1), label=m.group(2)))
        elif re.match(r"^goto (loc_[0-9A-F]+);$", s):
            stmts.append(Stmt("goto", s, comment, label=s[5:-1]))
        elif s == "return;":
            stmts.append(Stmt("return", s, comment))
        elif s.startswith("__builtin_trap();"):
            stmts.append(Stmt("abort", s, comment))
        elif re.match(r"^ppc_trap\(ctx, base, \d+\);$", s):
            stmts.append(Stmt("trap", s, comment, number=int(re.search(r"\d+", s[16:]).group(0))))
        elif re.match(r"^switch \((.*)\) \{$", s):
            braces.append("switch")
            stmts.append(Stmt("switch", s, comment, expr=s[8:-3], labels=[]))
        elif re.match(r"^case -?\d+:$", s) or s == "default:":
            stmts.append(Stmt("case", s, comment))
        elif s == "}":
            opened = braces.pop() if braces else None
            if opened is None:
                raise Refused("an unmatched }")
            stmts.append(Stmt("end" if opened == "switch" else "brace", s, comment))
        elif s == "{":
            braces.append("block")
            stmts.append(Stmt("brace", s, comment))
        elif re.match(r"^for \(.*\)$", s):
            stmts.append(Stmt("plain", s, comment))
        elif re.match(r"^(uint32_t ea\{\};|PPCRegister temp\{\};|PPCVRegister vTemp\{\};)$", s):
            stmts.append(Stmt("decl", s, comment))
        elif s.endswith(";") and not s.endswith("{") and "{" not in s:
            if re.match(r"^ctx\.lr = ", s):
                stmts.append(Stmt("skip", s, comment))
            else:
                stmts.append(Stmt("plain", s, comment))
        else:
            raise Refused(f"statement: {s[:60]}")
        comment = None
    # A switch's gotos are its edges.
    open_switch = None
    for st in stmts:
        if st.kind == "switch":
            open_switch = st
        elif st.kind == "end":
            open_switch = None
        elif open_switch is not None and st.kind == "goto":
            open_switch.labels.append(st.label)
            st.in_switch = True
    return stmts


def register_of(name, field):
    """A ctx member's register for the analysis, or None (lr, fpscr)."""
    if name in GPR_LIKE or name in FPRS or name in CRS or name in VRS:
        return name
    if name == "vscr_sat":
        return name
    if name == "xer":
        return field if field in XER else "so"
    return None


def defs_uses(st):
    """(full definitions, uses) of a statement's registers."""
    code = st.code
    if st.kind not in ("plain", "if", "switch", "call") or (st.kind == "call" and st.how != "indirect"):
        return set(), set()
    defs = set()
    skip = None
    m = re.match(r"^ctx\.(\w+)\.(\w+)\s*=(?!=)", code)
    if m:
        reg = register_of(m.group(1), m.group(2))
        view = m.group(2)
        if reg in GPR_LIKE and view in ("u64", "s64"):
            defs.add(reg)
            skip = m.start()
        elif reg in FPRS and view in ("f64", "u64", "s64"):
            defs.add(reg)
            skip = m.start()
        elif reg in CRS or reg in XER:
            defs.add(reg)
            skip = m.start()
    m = re.match(r"^ctx\.(cr\d)\.(compare|set_raw|setFromMask)", code)
    if m:
        defs.add(m.group(1))
        skip = m.start()
    # A vector register written whole: a store into it, or assigned.
    m = (re.match(r"^simde_mm_store_(?:si128|ps)\(\(?(?:simde__m128i\*)?\)?ctx\.(v\d+)\.\w+,", code)
         or re.match(r"^ctx\.(v\d+) = ", code))
    if m:
        defs.add(m.group(1))
        skip = m.start(1) - 4
    # A vector element written: counted as the register's write (its other
    # lanes start at 0 where the original had what the caller left).
    m = re.match(r"^ctx\.(v\d+)\.\w+\[[^\]]+\]\s*[|&^+-]?=(?!=)", code)
    if m:
        defs.add(m.group(1))
        skip = m.start(1) - 4
    if re.match(r"^ctx\.vscr_sat = ", code):
        defs.add("vscr_sat")
        skip = 0
    uses = set()
    for t in TOKEN.finditer(code):
        if t.start() == skip:
            continue
        reg = register_of(t.group(1), t.group(2))
        if reg is not None:
            uses.add(reg)
    return defs, uses


def is_save(st, reg):
    """A nonvolatile register stored as it came (std, stfd, stvx): its save."""
    if reg in VRS:
        return st.kind == "plain" and st.code.startswith(
            "simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(") and st.code.count("ctx.v") == 1
    return st.kind == "plain" and re.match(
        r"^REX_STORE_U64\([^;]*, ctx\.%s\.u64\);$" % reg, st.code) is not None


# --- The analysis ---

class Info:
    def __init__(self, name):
        self.name = name
        self.stmts = None
        self.error = None
        self.reads = None       # params-candidate registers read maybe before written (own)
        self.calls = []         # (stmt index, how, target)
        self.writes = set()     # registers written by its own statements
        self.params = []
        self.returns = (False, False)
        self.analyzed = False   # params and returns worked out from its body


def successors(stmts, labels):
    succ = []
    for i, st in enumerate(stmts):
        nxt = [i + 1] if i + 1 < len(stmts) else []
        if st.kind == "goto":
            s = [] if getattr(st, "in_switch", False) else [labels[st.label]]
        elif st.kind == "if":
            s = nxt + [labels[st.label]]
        elif st.kind in ("return", "abort"):
            s = []
        elif st.kind == "switch":
            s = [labels[l] for l in st.labels]
        else:
            s = nxt
        succ.append(s)
    return succ


def call_arg_regs(info_by_name, st):
    """The registers a call passes that its callee reads: a direct callee's
    parameters when its body could be read (lifted or not), else all of
    them; a call by address (a vtable slot or a function pointer) and an
    import pass the integer ones."""
    callee = info_by_name.get(st.target) if st.how == "direct" else None
    if callee is not None and callee.analyzed:
        return set(callee.params)
    if st.how in ("import", "indirect"):
        return {"r1", "r13"} | set(CALL_GPRS)
    return set(ALL_ARGS)


def call_defs(info_by_name, st):
    """What a call writes of r3 and f1: a direct callee's returns when its
    body could be read, else both (a call through the boundary gives back
    what the callee left in both, which is what it had)."""
    callee = info_by_name.get(st.target) if st.how == "direct" else None
    if callee is not None and callee.analyzed:
        r3, f1 = callee.returns
        return ({"r3"} if r3 else set()) | ({"f1"} if f1 else set())
    return {"r3", "f1"}


def prepare(info):
    stmts = info.stmts
    labels = {st.label: i for i, st in enumerate(stmts) if st.kind == "label"}
    info.problems = []
    for st in stmts:
        if st.kind in ("goto", "if") and st.label not in labels:
            info.problems.append(f"a branch out of the function ({st.label})")
            labels[st.label] = None
    succ = successors(stmts, labels) if not info.problems else [[] for _ in stmts]
    info.succ = succ
    info.local_du = [None if st.kind == "call" else defs_uses(st) for st in stmts]
    info.indirect_uses = [defs_uses(st)[1] if st.kind == "call" else None for st in stmts]


EVERY = frozenset(set(GPRS) | set(FPRS) | set(CRS) | set(VRS) | {"ctr", "reserved", "vscr_sat"} | XER)


def analyze(info, info_by_name):
    """Parameters, returns and problems from the statements, given the
    callees' parameters and returns."""
    stmts = info.stmts
    succ = info.succ
    per_stmt = []
    for i, st in enumerate(stmts):
        if st.kind == "call":
            per_stmt.append((frozenset(call_defs(info_by_name, st)),
                             frozenset(call_arg_regs(info_by_name, st)),
                             frozenset(info.indirect_uses[i])))
        else:
            d, u = info.local_du[i]
            per_stmt.append((frozenset(d), frozenset(), frozenset(u)))
    n = len(stmts)
    de_in = [None] * n
    nc_in = [None] * n
    if n:
        de_in[0] = frozenset()
        nc_in[0] = EVERY
    work = [0] if n else []
    while work:
        i = work.pop()
        de, nc = de_in[i], nc_in[i]
        d = per_stmt[i][0]
        if stmts[i].kind == "call":
            de_out, nc_out = de | d, (nc - VOLATILE) | d
        else:
            de_out, nc_out = de | d, nc | d
        for j in succ[i]:
            if de_in[j] is None:
                de_in[j], nc_in[j] = de_out, nc_out
                work.append(j)
            else:
                a, b = de_in[j] & de_out, nc_in[j] & nc_out
                if a != de_in[j] or b != nc_in[j]:
                    de_in[j], nc_in[j] = a, b
                    work.append(j)
    params = set()
    problems = list(info.problems)
    returns_r3 = returns_f1 = False
    for i, st in enumerate(stmts):
        if de_in[i] is None:
            continue  # unreachable
        d, soft, hard = per_stmt[i]
        returns_r3 |= "r3" in d
        returns_f1 |= "f1" in d
        for reg in hard:
            if reg not in de_in[i]:
                if reg in PARAMS:
                    params.add(reg)
                elif reg in ZERO_AT_ENTRY:
                    pass
                elif reg in NONVOLATILE and is_save(st, reg):
                    pass
                else:
                    problems.append(f"reads a caller's {reg}: {st.code[:60]}")
            if reg not in nc_in[i] and reg not in ZERO_AT_ENTRY:
                problems.append(f"reads {reg} a call left: {st.code[:60]}")
        for reg in soft:
            if reg not in de_in[i] and reg in PARAMS:
                params.add(reg)
    # A return reads what the function returns: on a path that didn't write
    # it, the value it was called with (a parameter).
    for i, st in enumerate(stmts):
        if de_in[i] is None:
            continue
        if st.kind == "return" or (st.kind == "plain" and st.code.endswith("return;")):
            if returns_r3 and "r3" not in de_in[i]:
                params.add("r3")
            if returns_f1 and "f1" not in de_in[i]:
                params.add("f1")
    # A function that calls passes its stack pointer and r13 (the thread's
    # block) on: ports and imports use them even where the original callee
    # doesn't read them.
    if any(st.kind == "call" and de_in[i] is not None for i, st in enumerate(stmts)):
        params |= {"r1", "r13"}
    info.params = [r for r in PARAM_ORDER if r in params]
    info.returns = (returns_r3, returns_f1)
    info.final_problems = problems


def load_fragments():
    """Jump-table cases the recompiler split off as functions, by parent (the
    static map's `folded` column): each is a copy of the rest of its parent
    from the case on, using the parent's registers and frame."""
    out = {}
    with open(TSV) as f:
        for r in csv.DictReader(f, delimiter="\t"):
            if r["folded"]:
                out[r["function"]] = r["folded"].split(",")
    return out


def fold(info, frag, stmts):
    """Puts a fragment's statements at the end of its parent (its labels
    renamed apart from the parent's) and turns the parent's calls into it
    into gotos."""
    suffix = "_" + frag[4:]
    entry = "loc_frag" + suffix
    renamed = [Stmt("label", entry + ":", None, label=entry)]
    for st in stmts:
        if hasattr(st, "label") and st.label:
            st.label = st.label + suffix
            if st.kind == "label":
                st.code = st.label + ":"
            elif st.kind == "goto":
                st.code = f"goto {st.label};"
            elif st.kind == "if":
                st.code = f"if ({st.cond}) goto {st.label};"
        if st.kind == "switch":
            st.labels = [l + suffix for l in st.labels]
        renamed.append(st)
    for i, st in enumerate(info.stmts):
        if st.kind == "call" and st.how == "direct" and st.target == frag:
            info.stmts[i] = Stmt("goto", f"goto {entry};", st.comment, label=entry)
    for st in renamed:
        if st.kind == "call" and st.how == "direct" and st.target == frag:
            raise Refused(f"a fragment calling itself ({frag})")
    info.stmts.extend(renamed)


def build(names, bodies, skip):
    """The Info of each function to a fixed point: `names` may be lifted;
    every other function with a body is analyzed too (its parameters and
    returns, for its callers' calls)."""
    infos = {}
    wanted = set(names)
    fragments = load_fragments()
    for name in bodies:
        if not name.startswith("sub_"):
            continue
        info = Info(name)
        info.lifted = False
        infos[name] = info
        try:
            info.stmts = parse(name, bodies[name])
            for frag in fragments.get(name, ()):
                if frag not in bodies:
                    raise Refused(f"a fragment with no body ({frag})")
                fold(info, frag, parse(frag, bodies[frag]))
        except Refused as e:
            info.error = str(e)
            continue
        info.analyzed = True
        prepare(info)
    callers = collections.defaultdict(set)
    for info in infos.values():
        if info.analyzed:
            for st in info.stmts:
                if st.kind == "call" and st.how == "direct":
                    callers[st.target].add(info.name)
    work = collections.deque(n for n, i in infos.items() if i.analyzed)
    queued = set(work)
    steps = 0
    while work:
        name = work.popleft()
        queued.discard(name)
        info = infos[name]
        before = (info.params, info.returns)
        analyze(info, infos)
        steps += 1
        if (info.params, info.returns) != before:
            for c in callers[name]:
                if c not in queued:
                    queued.add(c)
                    work.append(c)
        if steps > 50 * len(infos):
            raise SystemExit("lift.py: the analysis didn't settle")
    # A call into a function that reads its caller's saved registers is a
    # call into a fragment the static map didn't fold: refused.
    for info in infos.values():
        if not info.analyzed:
            continue
        for st in info.stmts:
            callee = infos.get(st.target) if st.kind == "call" and st.how == "direct" else None
            if callee is not None and callee.analyzed and any(
                    "a caller's" in p for p in callee.final_problems):
                info.final_problems.append(f"calls {st.target}, which reads its caller's registers")
                break
    for info in infos.values():
        if not info.analyzed:
            continue
        if info.final_problems:
            info.error = info.final_problems[0]
        elif info.name in skip:
            info.error = "has an override of ours"
        elif info.name in wanted:
            info.lifted = True
    for name in wanted - set(infos):
        info = Info(name)
        info.lifted = False
        info.error = "no body in generated/"
        infos[name] = info
    return infos


# --- Writing C++ ---

def read_expr(name, view):
    if name in ("ctr", "reserved") or name in GPRS:
        return {
            "u64": name, "s64": f"int64_t({name})", "u32": f"uint32_t({name})",
            "s32": f"int32_t({name})", "u16": f"uint16_t({name})", "s16": f"int16_t({name})",
            "u8": f"uint8_t({name})", "s8": f"int8_t({name})",
            "f32": f"std::bit_cast<float>(uint32_t({name}))", "f64": f"std::bit_cast<double>({name})",
        }[view]
    if name in FPRS:
        bits = f"std::bit_cast<uint64_t>({name})"
        return {
            "f64": name, "u64": bits, "s64": f"std::bit_cast<int64_t>({name})",
            "u32": f"uint32_t({bits})", "s32": f"int32_t({bits})",
            "f32": f"std::bit_cast<float>(uint32_t({bits}))",
        }[view]
    raise Refused(f"view {name}.{view}")


def rewrite(text):
    """A statement's ctx reads as locals."""
    def sub(m):
        name, field = m.group(1), m.group(2)
        if name in CRS:
            if field is None:
                return name
            return f"{name}.{'so' if field == 'un' else field}"
        if name == "xer":
            return field if field in XER else "so"
        if name == "lr":
            return "0" if field is None else "0"
        if name in VRS:
            return name if field is None else f"{name}.{field}"
        if name == "vscr_sat":
            return name
        if field is None:
            raise Refused(f"ctx.{name} bare")
        return read_expr(name, field)
    text = re.sub(r"ppc_trap\(ctx, base, (\d+)\)", r"lifted::Trap(\1)", text)
    out = TOKEN.sub(sub, text)
    if "ctx" in re.sub(r"\w*ctx\w+", "", out):
        raise Refused(f"ctx left in: {out[:60]}")
    return out


PARTIAL_MASK = {"u32": ("0xFFFFFFFF00000000ull", "uint32_t"), "s32": ("0xFFFFFFFF00000000ull", "uint32_t"),
                "u16": ("0xFFFFFFFFFFFF0000ull", "uint16_t"), "s16": ("0xFFFFFFFFFFFF0000ull", "uint16_t"),
                "u8": ("0xFFFFFFFFFFFFFF00ull", "uint8_t"), "s8": ("0xFFFFFFFFFFFFFF00ull", "uint8_t")}


def assign(name, view, rhs):
    if name in GPR_LIKE:
        if view == "u64":
            return f"{name} = {rhs};"
        if view == "s64":
            return f"{name} = uint64_t(int64_t({rhs}));"
        if view == "f64":
            return f"{name} = std::bit_cast<uint64_t>(double({rhs}));"
        if view == "f32":
            return f"{name} = ({name} & 0xFFFFFFFF00000000ull) | std::bit_cast<uint32_t>(float({rhs}));"
        mask, cast = PARTIAL_MASK[view]
        signed = "int" + cast[4:] if view.startswith("s") else cast
        return f"{name} = ({name} & {mask}) | {cast}({signed}({rhs}));"
    if name in FPRS:
        if view == "f64":
            return f"{name} = {rhs};"
        if view == "u64":
            return f"{name} = std::bit_cast<double>(uint64_t({rhs}));"
        if view == "s64":
            return f"{name} = std::bit_cast<double>(uint64_t(int64_t({rhs})));"
        if view == "f32":
            return (f"{name} = std::bit_cast<double>((std::bit_cast<uint64_t>({name}) & 0xFFFFFFFF00000000ull)"
                    f" | std::bit_cast<uint32_t>(float({rhs})));")
        if view in ("u32", "s32"):
            return (f"{name} = std::bit_cast<double>((std::bit_cast<uint64_t>({name}) & 0xFFFFFFFF00000000ull)"
                    f" | uint32_t({rhs}));")
    raise Refused(f"write {name}.{view}")


def plain(code):
    m = re.match(r"^ctx\.(cr\d)\.compare(<[^>]+>)?\((.*)\);$", code)
    if m:
        args = rewrite(m.group(3))
        if m.group(2):
            return f"{m.group(1)} = lifted::Compare{m.group(2)}({args});"
        return f"{m.group(1)} = lifted::CompareFloat({args});"
    if re.match(r"^ctx\.fpscr\.disableFlushMode(Unconditional)?\(\);$", code):
        return "lifted::ScalarFloatMode();"
    if re.match(r"^ctx\.fpscr\.enableFlushMode(Unconditional)?\(\);$", code):
        return "lifted::VectorFloatMode();"
    m = re.match(r"^ctx\.(cr\d)\.setFromMask\((.*)\);$", code)
    if m:
        return f"{m.group(1)} = lifted::CrFromMask({rewrite(m.group(2))});"
    if code == "PPCVRegister vTemp{};":
        return "lifted::Vec vTemp{};"
    if code == "PPCRegister temp{};":
        return "lifted::Temp temp{};"
    m = re.match(r"^(--|\+\+)ctx\.ctr\.u64;$", code)
    if m:
        return f"{m.group(1)}ctr;"
    m = re.match(r"^ctx\.(\w+)\.(\w+)\s*=(?!=)\s*(.*);$", code)
    if m:
        name, view, rhs = m.groups()
        rhs = rewrite(rhs)
        if name in CRS:
            return f"{name}.{'so' if view == 'un' else view} = {rhs};"
        if name == "xer":
            return f"{view} = {rhs};"
        return assign(name, view, rhs)
    m = re.match(r"^ctx\.(\w+)\.(\w+)\s*(\+=|-=|\|=|&=|\^=)", code)
    if m:
        raise Refused(f"compound assignment: {code[:60]}")
    return rewrite(code)


def signature(info, qualified=False):
    r3, f1 = info.returns
    ret = "lifted::Result" if r3 and f1 else "uint64_t" if r3 else "double" if f1 else "void"
    def typed(p):
        return ("double " if p.startswith("f") else "lifted::Vec " if p.startswith("v") else "uint64_t ") + p
    params = ["uint8_t* base"] + [typed(p) for p in info.params]
    return f"{ret} {'lifted::' if qualified else ''}{fname(info.name)}({', '.join(params)})"


def fname(name):
    return "f" + name[4:]


def ename(name):
    return "e" + name[4:]


def return_stmt(info):
    r3, f1 = info.returns
    if r3 and f1:
        return "return {r3, f1};"
    if r3:
        return "return r3;"
    if f1:
        return "return f1;"
    return "return;"


def assign_result(info_callee, call):
    """A call's statement, its results into r3 and f1."""
    if info_callee is None:
        return f"{{ const lifted::Result o = {call}; r3 = o.r3; f1 = o.f1; }}"
    r3, f1 = info_callee.returns
    if r3 and f1:
        return f"{{ const lifted::Result o = {call}; r3 = o.r3; f1 = o.f1; }}"
    if r3:
        return f"r3 = {call};"
    if f1:
        return f"f1 = {call};"
    return f"{call};"


def emit_function(info, infos, emitting):
    out = []
    used = set()
    body = []
    decls = {}  # the generated code's scratch declarations, once each
    for st in info.stmts:
        c = st.comment
        if c:
            body.append(f"  // {c}")
        if st.kind == "label":
            body.append(f"{st.label}:")
            continue
        if st.kind == "skip":
            body.append("  // (left out: dead in C++)")
            continue
        if st.kind == "call":
            if st.how == "direct" and st.target in infos and infos[st.target].lifted and st.target in emitting:
                callee = infos[st.target]
                call = f"{fname(st.target)}({', '.join(['base'] + callee.params)})"
                used.update(callee.params)
                line = assign_result(callee, call)
            elif st.how == "direct":
                line = assign_result(None, f"lifted::Call(base, 0x{st.target[4:]}, LIFTED_ARGS)")
                used.update(ALL_ARGS)
            elif st.how == "import":
                line = assign_result(None, f"lifted::Import(base, {st.target}, LIFTED_ARGS)")
                used.update(ALL_ARGS)
            else:
                target = rewrite(st.target)
                line = assign_result(None, f"lifted::Call(base, uint32_t({target}), LIFTED_ARGS)")
                used.update(ALL_ARGS)
            body.append("  " + line)
            used.update({"r3", "f1"})
            continue
        if st.kind == "return":
            body.append("  " + return_stmt(info))
            continue
        if st.kind == "abort":
            body.append("  std::abort();")
            continue
        if st.kind == "trap":
            body.append(f"  lifted::Trap({st.number});")
            continue
        if st.kind == "if":
            body.append(f"  if ({rewrite(st.cond)}) goto {st.label};")
        elif st.kind == "switch":
            body.append(f"  switch ({rewrite(st.expr)}) {{")
        elif st.kind == "decl":
            decls.setdefault(st.code, plain(st.code))
            continue
        elif st.kind in ("case", "end", "goto"):
            body.append("  " + st.code)
        else:
            body.append("  " + plain(st.code))
        # A conditional return (a conditional branch to the link register)
        # returns what the function returns.
        if body[-1].endswith(" return;") and st.kind != "return":
            body[-1] = body[-1][: -len("return;")] + return_stmt(info)
        for t in TOKEN.finditer(st.code):
            reg = register_of(t.group(1), t.group(2))
            if reg is not None:
                used.add(reg)
    r3, f1 = info.returns
    if r3:
        used.add("r3")
    if f1:
        used.add("f1")
    out.append(f"// {info.name}: lifted from generated/ by lift.py.")
    out.append(signature(info) + " {")
    out.append(f'  ITC_NATIVE_RUNNING("{info.name} (lifted)");')
    locals_ = [r for r in GPRS + ["ctr", "reserved"] if r in used and r not in info.params]
    if locals_:
        out.append("  uint64_t " + ", ".join(f"{r} = 0" for r in locals_) + ";")
    flocals = [r for r in FPRS if r in used and r not in info.params]
    if flocals:
        out.append("  double " + ", ".join(f"{r} = 0.0" for r in flocals) + ";")
    vrs = [r for r in VRS if r in used and r not in info.params]
    if vrs:
        out.append("  lifted::Vec " + ", ".join(f"{r}{{}}" for r in vrs) + ";")
    crs = [r for r in CRS if r in used]
    if crs:
        out.append("  lifted::Cr " + ", ".join(crs) + ";")
    xer = [r for r in ("ca", "so", "ov", "vscr_sat") if r in used]
    if xer:
        out.append("  uint8_t " + ", ".join(f"{r} = 0" for r in xer) + ";")
    out.extend("  " + d for d in decls.values())
    out.extend(body)
    if not info.stmts or info.stmts[-1].kind not in ("return", "abort", "goto"):
        out.append("  " + return_stmt(info))
    out.append("}")
    return out


def emit_entry(info):
    args = ["base"]
    for p in info.params:
        if p == "r1":
            args.append("a.r1")
        elif p == "r13":
            args.append("a.r13")
        elif p == "r2":
            args.append("0")
        elif p.startswith("r"):
            args.append(f"a.r[{int(p[1:]) - 3}]")
        elif p.startswith("v"):
            args.append(f"a.v[{int(p[1:]) - 1}]" if 1 <= int(p[1:]) <= 13 else "lifted::Vec{}")
        else:
            args.append(f"a.f[{int(p[1:]) - 1}]")
    call = f"{fname(info.name)}({', '.join(args)})"
    r3, f1 = info.returns
    if r3 and f1:
        body = f"return {call};"
    elif r3:
        body = f"return {{{call}, a.f[0]}};"
    elif f1:
        body = f"return {{a.r[0], {call}}};"
    else:
        body = f"{call};\n  return {{a.r[0], a.f[0]}};"
    return [f"lifted::Result {ename(info.name)}(uint8_t* base, const lifted::Args& a) {{",
            f"  {body}", "}"]


def emit_batch(batch, names, infos, emitting):
    flag = f"native_lifted_{batch}"
    lines = [
        f"// Lift sweep batch {batch} (docs/loops/lift-sweep.md): written by",
        "// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the",
        "// tool again.",
        "",
        '#include "lifted/functions.h"',
        '#include "lifted/sdk.h"',
        "",
        '#include "base/flags.h"',
        '#include "base/log.h"',
        "",
        "#include <bit>",
        "#include <cstdlib>",
        "",
        f"ITC_FLAG_BOOL({flag}, true, \"ITC\",",
        f"              \"Run lift sweep batch {batch}'s functions as lifted C++ (src/lifted/, \"",
        "              \"docs/loops/lift-sweep.md) instead of the recompiled ones.\");",
        "",
        "// A call by address or an import passes every argument register.",
        "#define LIFTED_ARGS \\",
        "  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \\",
        "                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }",
        "",
        "namespace lifted {",
        "",
    ]
    for name in names:
        lines.extend(emit_function(infos[name], infos, emitting))
        lines.append("")
    lines.append("namespace {")
    lines.append("")
    for name in names:
        lines.extend(emit_entry(infos[name]))
        lines.append("")
    lines.append("const Registration kEntries[] = {")
    for name in names:
        lines.append(f"    {{0x{name[4:]}, {ename(name)}}},")
    lines.append("};")
    lines.append("")
    lines.append("const bool kRegistered =")
    lines.append(f"    (Register(kEntries, std::size(kEntries), &ITC_FLAG({flag})), true);")
    lines.append("")
    lines.append("}  // namespace")
    lines.append("")
    lines.append("}  // namespace lifted")
    lines.append("")
    lines.append("// The entries for the SDK and the code not native yet (the function table")
    lines.append("// reaches these): to go once nothing recompiled calls them.")
    for name in names:
        lines.append(f"extern \"C\" REX_FUNC({name}) {{")
        lines.append(f"  if (!ITC_FLAG({flag})) {{")
        lines.append(f"    __imp__{name}(ctx, base);")
        lines.append("    return;")
        lines.append("  }")
        lines.append(f"  lifted::Enter(ctx, base, lifted::{ename(name)});")
        lines.append("}")
        lines.append("")
    # The functions with LIFTED_ARGS but without some of its registers as
    # locals or parameters would not build: every call adds them to `used`.
    return "\n".join(lines)


def emit_header(signatures):
    lines = [
        "// The lifted functions (docs/loops/lift-sweep.md): written by",
        "// itc_recomp/tools/lift.py. Don't edit it by hand: run the tool again.",
        "",
        "#pragma once",
        "",
        '#include "lifted/lifted.h"',
        "",
        "namespace lifted {",
        "",
    ]
    for name in sorted(signatures):
        lines.append(signatures[name] + ";")
    lines += ["", "}  // namespace lifted", ""]
    return "\n".join(lines)


# --- Commands ---

def unported():
    with open(TSV) as f:
        return [r for r in csv.DictReader(f, delimiter="\t") if r["state"] == "unported"]


def load_signatures():
    if os.path.exists(SIGNATURES):
        with open(SIGNATURES) as f:
            return json.load(f)
    return {}


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 1
    bodies = load_bodies()
    skip = overridden()
    rows = unported()
    names = sorted({r["function"] for r in rows} | set(argv[3:] if argv[1] == "emit" else argv[2:]))
    lifted_before = load_signatures()
    # Functions lifted in earlier batches count as lifted (their signatures).
    infos = build(sorted(set(names) | set(lifted_before)), bodies, skip - set(lifted_before))
    if argv[1] == "check":
        for name in argv[2:]:
            info = infos[name]
            if info.lifted:
                print(f"{name}: {signature(info)}")
            else:
                print(f"{name}: refused: {info.error}")
        return 0
    if argv[1] == "survey":
        reasons = collections.Counter()
        seen = {r["function"]: r["seen"] == "1" for r in rows}
        ok = [r["function"] for r in rows if infos[r["function"]].lifted]
        for r in rows:
            info = infos[r["function"]]
            if not info.lifted:
                reasons[re.sub(r":.*", "", info.error)] += 1
        print(f"unported {len(rows)}, liftable {len(ok)} "
              f"({sum(seen[n] for n in ok)} seen in play)")
        for reason, n in reasons.most_common():
            print(f"  {n:5}  {reason}")
        sizes = collections.Counter(len(infos[n].params) for n in ok)
        print("parameters:", dict(sorted(sizes.items())))
        return 0
    if argv[1] == "emit":
        batch = argv[2]
        chosen = argv[3:]
        bad = [n for n in chosen if not infos[n].lifted]
        if bad:
            for n in bad:
                print(f"{n}: refused: {infos[n].error}")
            return 1
        os.makedirs(OUT, exist_ok=True)
        with open(os.path.join(OUT, f"batch_{batch}.cpp"), "w") as f:
            f.write(emit_batch(batch, chosen, infos, set(chosen) | set(lifted_before)))
        signatures = dict(lifted_before)
        for n in chosen:
            signatures[n] = signature(infos[n])
        with open(SIGNATURES, "w") as f:
            json.dump(signatures, f, indent=1, sort_keys=True)
        with open(os.path.join(OUT, "functions.h"), "w") as f:
            f.write(emit_header(signatures))
        print(f"wrote batch {batch}: {len(chosen)} functions")
        return 0
    print(__doc__)
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
