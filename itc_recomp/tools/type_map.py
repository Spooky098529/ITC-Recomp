#!/usr/bin/env python3
"""E5's measure (docs/e5-native.md, step 1): the game's types, their fields,
their owners, and which of them can't move to the host by themselves.

It reads the game's code as the original PowerPC instructions (the comments
`// lwz r11,24(r31)` in itc_recomp/generated/, through lift.py's cache of
its function bodies), over the functions the static map reaches from `main`
(itc_recomp/coverage/static_map.tsv, ours and unported; the replaced
libraries left out), and follows pointers through registers:

  seeds      a function whose row names it `X::Y` takes an `X` in r3 (its
             `this`; constructors and destructors too); a call to an
             allocator with a constant size makes a new object, named by the
             constructor it's handed to (`X::X` with it in r3), else
             `new<size>@<caller>`; a function returns what it leaves in r3
  registers  a pointer into a type at an offset (addi, add of a constant,
             mr), a constant (li, lis, ori, addis), or unknown; a call clobbers
             r0 and r3 to r12; branches are read straight through (a label
             keeps what came before: a simplification)
  fields     every load and store at a register holding a typed pointer: the
             offset, width, and integer, float or vector; a store's value when
             it's itself a typed pointer (the field's pointee), a code address
             (a function pointer) or, at +0 in a constructor, an image address
             (the vtable)
  calls      the types passed in r3 to r10 are voted into the callee's
             parameters, and its return into the callers' r3; the whole pass
             runs again with those votes (three passes)
  globals    loads and stores at a constant image address (0x82...) are the
             image's data: counted by address

Then, per type: the areas (docs/functions/*.md, the static map's `file`)
whose functions touch it (one: private, several: shared), and tags for what
can't move by itself (an area that reads DATA/ files in place, one that
feeds the GPU, the image's data, saves). The graph: areas as nodes, shared
types as edges weighted by the functions touching them.

usage: type_map.py   (writes itc_recomp/coverage/type_map.tsv and
                      type_map_areas.tsv, prints the summary)
"""
import collections
import csv
import json
import os
import pickle
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import function_names  # noqa: E402
import lift  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(HERE))
COVERAGE = os.path.join(ROOT, "itc_recomp", "coverage")
TSV = os.path.join(COVERAGE, "static_map.tsv")

IMAGE_LO, IMAGE_HI = 0x82000000, 0x82A00000
CODE_LO, CODE_HI = 0x82090000, 0x82600000

# The allocators: address -> the argument register holding the size.
ALLOCATORS = {
    "825A6880": 3,  # operator new
    "825A6940": 3,  # operator new[]
    "825A9A90": 3,  # malloc
    "822976F8": 3,  # XMemAlloc
    "82299D60": 5,  # HeapAlloc
    "82297F48": 4,  # LocalAlloc
    "8258DFF0": 3,  # Memory::AllocVirtual
    "8258E098": 3,  # Memory::AllocPhysicalEx
    "822964C8": 3,  # XPhysicalAlloc
    "82296550": 3,  # XPhysicalAllocEx
    "82295398": 4,  # VirtualAlloc
}

# Areas whose objects can't move by themselves (docs/e5-native.md, step 1.4):
# the game reads them from DATA/ in place, the GPU reads them, or saves.
FILE_AREAS = {
    "archive.md", "file-layer.md", "models.md", "scene-loaders.md", "car-models.md",
    "xact.md", "xact-waves.md", "xact-tracks.md", "sound-cues.md", "strings-and-font.md",
    "data-tables.md", "car-specs.md", "ui-movies.md", "ui-movies-game.md", "data-files.md",
    "data-load.md", "shader-objects.md", "material-passes.md", "model-animation.md",
    "course-scenery.md", "scenery-lists.md", "texture-compression.md",
}
GPU_AREAS = {
    "device-state.md", "device-shaders.md", "command-buffer.md", "render-queue.md",
    "graphics-resources.md", "command-list.md", "single-commands.md", "predicated-tiling.md",
    "video-output.md", "post-effects.md", "scene-target.md", "frame-passes.md", "sprites.md",
}
SAVE_AREAS = {"save-data.md", "save-interface.md", "save-screen.md", "options-load-save.md"}

GPR = re.compile(r"^r(\d+)$")
MEM = re.compile(r"^(-?\d+)\(r(\d+)\)$")
HELPER = re.compile(r"__(save|rest)(gpr|fpr|vmx)")
PARAM = re.compile(r"^(sub_[0-9A-F]{8})\.r(\d+)$")
INSN = re.compile(r"^\s*// ([a-z][\w.]*)(?: (.*))?$")

LOADS = {"lwz": (4, "int"), "lwzu": (4, "int"), "lhz": (2, "int"), "lha": (2, "int"),
         "lbz": (1, "int"), "ld": (8, "int"), "lwa": (4, "int"), "lfs": (4, "float"),
         "lfd": (8, "double"), "lhzu": (2, "int"), "lbzu": (1, "int"), "lfsu": (4, "float")}
STORES = {"stw": (4, "int"), "stwu": (4, "int"), "sth": (2, "int"), "stb": (1, "int"),
          "std": (8, "int"), "stfs": (4, "float"), "stfd": (8, "double"), "stfiwx": (4, "int"),
          "sthu": (2, "int"), "stbu": (1, "int"), "stfsu": (4, "float")}
INDEXED = {"lwzx", "lhzx", "lbzx", "lhax", "ldx", "lfsx", "lfdx", "stwx", "sthx", "stbx",
           "stdx", "stfsx", "stfdx", "lvx", "lvx128", "stvx", "stvx128", "lvlx", "lvrx",
           "stvlx", "stvrx", "lvlx128", "lvrx128", "stvlx128", "stvrx128", "lwbrx", "stwbrx"}
NO_WRITE = ("st", "cmp", "b", "mt", "tw", "td", "dcb", "sync", "eieio", "lwsync", "isync", "db")


def path(t, off):
    """The type of what a field holds: `T.+off`; a field that leads to its
    own type's same field (a list's next) is that type, and paths stop at
    four steps."""
    step = f".+{off}"
    if t.endswith(step):
        return t
    if t.count(".+") >= 4:
        return t
    return t + step


class Stack:
    """An address in the function's own frame (addi rN,r1,k): a local."""
    __slots__ = ()


class Ptr:
    __slots__ = ("type", "off")

    def __init__(self, type_, off):
        self.type, self.off = type_, off


class Const:
    __slots__ = ("value",)

    def __init__(self, value):
        self.value = value & 0xFFFFFFFF


def imm(text):
    return int(text, 0)


class Map:
    def __init__(self):
        self.rows = [r for r in csv.DictReader(open(TSV), delimiter="\t")
                     if r["reach"] == "main" and r["state"] in ("ours", "unported")]
        self.area = {r["function"]: r["file"] or "?" for r in self.rows}
        self.bodies = lift.load_bodies()
        self.names = {"sub_" + a: n for a, n in function_names.row_names().items() if n}
        # Parsed instructions per function: (mnemonic, operands); each one's
        # ordinal among the body's comments (lift.py's Stmt.insn).
        self.code = {}
        self.ordinals = {}
        self.recording = None
        for r in self.rows:
            insns = []
            ordinal = 0
            for line in self.bodies.get(r["function"], []):
                if line.strip().startswith("//"):
                    # Counted as lift.py counts its statements' comments.
                    ordinal += 1
                m = INSN.match(line)
                if m:
                    insns.append((m.group(1), [o.strip() for o in (m.group(2) or "").split(",")]))
                    self.ordinals[(r["function"], len(insns) - 1)] = ordinal
                elif line.startswith("loc_"):
                    insns.append(("label", []))
                elif HELPER.search(line) and insns and insns[-1][0] in ("bl", "b"):
                    # The register save and restore helpers: not calls.
                    insns[-1] = ("helper", [])
            self.code[r["function"]] = insns
        self.assign_areas()
        self.parent = {}
        self.refused = 0
        self.generic = set()
        self.params = {}
        self.returns = {}
        self.field_votes = collections.defaultdict(collections.Counter)
        self.field_ptr = {}

    def display(self, t):
        """A type's name with functions named by their rows."""
        return re.sub(r"sub_([0-9A-F]{8})",
                      lambda g: self.names.get(g.group(0)) or g.group(0), t)

    def assign_areas(self):
        """Functions whose rows are in lifted.md (the lift sweep's catch-all)
        or nowhere take the area most of their callers have (then callees),
        repeated until nothing changes."""
        callers = collections.defaultdict(list)
        callees = collections.defaultdict(list)
        for fn, insns in self.code.items():
            for op, args in insns:
                if op in ("bl", "b") and args and args[0].startswith("0x"):
                    t = "sub_%08X" % int(args[0], 16)
                    if t in self.code and t != fn:
                        callers[t].append(fn)
                        callees[fn].append(t)
        loose = {"lifted.md", "?", "functions.md"}
        self.loose = {f for f, a in self.area.items() if a in loose}
        for _ in range(20):
            changed = False
            for f in sorted(self.loose):
                votes = collections.Counter(self.area[c] for c in callers[f] if self.area[c] not in loose)
                if not votes:
                    votes = collections.Counter(self.area[c] for c in callees[f] if self.area[c] not in loose)
                if votes:
                    a = votes.most_common(1)[0][0]
                    if self.area[f] != a:
                        self.area[f] = a
                        changed = True
            if not changed:
                break

    # Types: union-find over names (an allocation joined to its class, a
    # field's pointee to the type stored there). A name beats an allocation,
    # an allocation beats a path.
    @staticmethod
    def rank(t):
        if ".r" in t[:14] and t.startswith("sub_") or t.endswith(".ret"):
            return 3
        if t.startswith("[") or ".+" in t:
            return 2
        if t.startswith("new<"):
            return 1
        return 0

    def find(self, t):
        while self.parent.get(t, t) != t:
            t = self.parent[t]
        return t

    def join(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b:
            return
        if self.rank(b) < self.rank(a):
            a, b = b, a
        if self.rank(a) == 0 and self.rank(b) == 0:
            # Two classes the rows name differently are different types
            # (at most a base and a derived class): not merged, so what
            # joined them stays generic.
            self.refused += 1
            return
        self.parent[b] = a

    def class_of(self, fn):
        n = self.names.get(fn)
        if not n or "::" not in n:
            return None
        parts = n.split("::")
        return "::".join(parts[:-1])

    def is_ctor(self, fn):
        n = self.names.get(fn)
        if not n or "::" not in n:
            return False
        parts = n.split("::")
        return parts[-1] == parts[-2]

    def run_pass(self, record):
        self.fields = collections.defaultdict(lambda: collections.defaultdict(collections.Counter))
        self.touch = collections.defaultdict(set)        # type -> functions
        self.allocs = collections.defaultdict(list)       # type -> (caller, size)
        self.vtables = collections.defaultdict(set)
        self.globals = collections.defaultdict(set)       # image address -> functions
        self.stats = collections.Counter()
        self.unattributed = collections.Counter()         # function -> accesses
        self.used_ptr = collections.defaultdict(set)      # function -> parameters used as pointers
        self.ctor_joins = []
        new_params = collections.defaultdict(lambda: collections.defaultdict(collections.Counter))
        new_returns = collections.defaultdict(collections.Counter)
        new_fields = collections.defaultdict(collections.Counter)
        for fn, insns in self.code.items():
            regs = {i: Ptr(f"{fn}.r{i}", 0) for i in range(3, 11)}
            ctor = self.is_ctor(fn)
            self._alloc_seen = collections.Counter()
            for index, (op, args) in enumerate(insns):
                self._insn = (fn, index)
                self.step(fn, op, args, regs, ctor, new_params, new_returns, new_fields)
        if record:
            return
        # A parameter the function uses as a pointer (a load or store through
        # it) is joined: with the class its row names (r3 of `X::Y`), and
        # with the type every caller passes (one given several, a generic
        # function's, keeps its own); the same for a field's pointee and a
        # return. The joins are decided on this pass's votes and the types
        # built again from them alone, so a join an earlier pass made on
        # less is undone when the votes disagree now.
        pairs = list(self.ctor_joins)
        for fn, used in self.used_ptr.items():
            cls = self.class_of(fn)
            if cls and 3 in used:
                pairs.append((cls, f"{fn}.r3"))
        for fn, v in new_params.items():
            for i, c in v.items():
                if i not in self.used_ptr.get(fn, ()):
                    continue
                kinds = {self.find(t) for t in c}
                if len(kinds) == 1:
                    pairs.append((f"{fn}.r{i}", min(kinds)))
        for field, c in new_fields.items():
            kinds = {self.find(t) for t in c}
            if len(kinds) == 1:
                pairs.append((field, min(kinds)))
        for fn, c in new_returns.items():
            kinds = {self.find(t) for t in c}
            if len(kinds) == 1:
                pairs.append((f"{fn}.ret", min(kinds)))
        self.parent = {}
        self.refused = 0
        for a, b in pairs:
            self.join(a, b)

    def access(self, fn, regs, base, disp, width, kind, value, store):
        b = regs.get(base)
        if base == 1 or isinstance(b, Stack):
            self.stats["stack"] += 1
            return None
        if base == 13:
            self.stats["thread block"] += 1
            return None
        if isinstance(b, Ptr) and b.off is None:
            # An element of an array inside the type (an index added).
            self.stats["typed array"] += 1
            self.touch[self.find(b.type)].add(fn)
            self.fields[self.find(b.type)]["[i]"][(width, kind)] += 1
            return None
        if isinstance(b, Ptr):
            pm = PARAM.match(b.type)
            if pm and pm.group(1) == fn:
                self.used_ptr[fn].add(int(pm.group(2)))
            t = self.find(b.type)
            off = b.off + disp
            self.stats["typed"] += 1
            if self.recording is not None:
                self.recording[(fn, self.ordinals[self._insn])] = (t, b.off, disp, width, store)
            self.touch[t].add(fn)
            f = self.fields[t][off]
            f[(width, kind)] += 1
            if store and isinstance(value, Ptr) and width == 4:
                f[("ptr", self.find(value.type))] += 1
                self._new_fields[path(t, off)][self.find(value.type)] += 1
            elif store and isinstance(value, Const) and CODE_LO <= value.value < CODE_HI:
                f[("fn", "%08X" % value.value)] += 1
            elif store and isinstance(value, Const) and IMAGE_LO <= value.value < IMAGE_HI:
                f[("image", "%08X" % value.value)] += 1
                if off == 0 and self.is_ctor(fn):
                    self.vtables[t].add("%08X" % value.value)
            if not store and kind == "int" and width == 4:
                return Ptr(path(t, off), 0)
            return None
        if isinstance(b, Const):
            address = (b.value + disp) & 0xFFFFFFFF
            if IMAGE_LO <= address < IMAGE_HI:
                self.stats["image"] += 1
                self.globals[address].add(fn)
                if store and isinstance(value, Ptr) and width == 4:
                    self._new_fields["[%08X]" % address][self.find(value.type)] += 1
                if not store and kind == "int" and width == 4:
                    return Ptr("[%08X]" % address, 0)
                return None
            self.stats["other constant"] += 1
            return None
        self.stats["untyped"] += 1
        self.unattributed[fn] += 1
        return None

    def step(self, fn, op, args, regs, ctor, new_params, new_returns, new_fields):
        self._new_fields = new_fields
        g = [GPR.match(a) for a in args]
        r = [int(m.group(1)) if m else None for m in g]
        if op == "label":
            return
        if op in LOADS and len(args) == 2:
            m = MEM.match(args[1])
            if m:
                width, kind = LOADS[op]
                v = self.access(fn, regs, int(m.group(2)), int(m.group(1)), width, kind, None, False)
                if r[0] is not None:
                    if v is None:
                        regs.pop(r[0], None)
                    else:
                        regs[r[0]] = v
                if op.endswith("u"):
                    regs.pop(int(m.group(2)), None)
            return
        if op in STORES and len(args) == 2:
            m = MEM.match(args[1])
            if m:
                width, kind = STORES[op]
                value = regs.get(r[0]) if r[0] is not None else None
                self.access(fn, regs, int(m.group(2)), int(m.group(1)), width, kind, value, True)
                if op.endswith("u") and int(m.group(2)) != 1:
                    regs.pop(int(m.group(2)), None)
            return
        if op in INDEXED:
            base = r[1] if len(r) > 1 else None
            b = regs.get(base)
            if isinstance(b, Ptr):
                self.stats["typed indexed"] += 1
                self.touch[self.find(b.type)].add(fn)
            else:
                self.stats["indexed"] += 1
                self.unattributed[fn] += 1
            if not op.startswith("st") and r[0] is not None:
                regs.pop(r[0], None)
            return
        if op in ("bl", "b") and args and args[0].startswith("0x"):
            target = "sub_%08X" % int(args[0], 16)
            if op == "b" and target not in self.code and target[4:] not in ALLOCATORS:
                return  # a branch inside the function
            for i in range(3, 11):
                v = regs.get(i)
                if isinstance(v, Ptr):
                    new_params[target][i][self.find(v.type)] += 1
                    if i == 3 and self.is_ctor(target):
                        cls = self.class_of(target)
                        # A constructor calling its base class's on its own
                        # `this` (ScreenMachine's StateMachine) makes no join.
                        base_ctor = self.is_ctor(fn) and v.type == f"{fn}.r3"
                        if cls and v.off == 0 and not base_ctor:
                            self.ctor_joins.append((cls, v.type))
            result = None
            size_reg = ALLOCATORS.get(target[4:])
            if size_reg is not None:
                size = regs.get(size_reg)
                key = "new<%s>@%s" % (size.value if isinstance(size, Const) else "?", fn)
                # A function's second allocation of the same size is another
                # object (the type 2 dialog's menu and data, both 68 bytes).
                self._alloc_seen[key] += 1
                if self._alloc_seen[key] > 1:
                    key += "#%d" % self._alloc_seen[key]
                self.allocs[self.find(key)].append((fn, size.value if isinstance(size, Const) else None))
                result = Ptr(key, 0)
            elif target in self.code:
                result = Ptr(f"{target}.ret", 0)
            for i in [0] + list(range(3, 13)):
                regs.pop(i, None)
            if result:
                regs[3] = result
            if op == "b" and isinstance(result, Ptr):
                new_returns[fn][self.find(result.type)] += 1
            return
        if op in ("bctrl", "bctr"):
            for i in [0] + list(range(3, 13)):
                regs.pop(i, None)
            return
        if op in ("blr", "beqlr", "bnelr", "bltlr", "bgtlr", "blelr", "bgelr"):
            v = regs.get(3)
            if isinstance(v, Ptr) and v.off == 0:
                new_returns[fn][self.find(v.type)] += 1
            return
        if not r or r[0] is None or op.startswith(NO_WRITE):
            return
        d = r[0]
        try:
            if op == "li":
                regs[d] = Const(imm(args[1]))
            elif op == "lis":
                regs[d] = Const(imm(args[1]) << 16)
            elif op in ("mr", "mr."):
                v = regs.get(r[1])
                regs[d] = v
                if v is None:
                    regs.pop(d, None)
            elif op in ("addi", "addis"):
                v = 0 if args[1] == "r0" and op == "addi" else regs.get(r[1])
                k = imm(args[2]) << (16 if op == "addis" else 0)
                if r[1] == 1 or isinstance(v, Stack):
                    regs[d] = Stack()
                elif v == 0:
                    regs[d] = Const(k)
                elif isinstance(v, Const):
                    regs[d] = Const(v.value + k)
                elif isinstance(v, Ptr):
                    regs[d] = Ptr(v.type, None if v.off is None else v.off + k)
                else:
                    regs.pop(d, None)
            elif op in ("ori", "oris"):
                v = regs.get(r[1])
                k = imm(args[2]) << (16 if op == "oris" else 0)
                if isinstance(v, Const):
                    regs[d] = Const(v.value | k)
                else:
                    regs.pop(d, None)
            elif op == "add":
                a, b = regs.get(r[1]), regs.get(r[2])
                if isinstance(a, Ptr) and isinstance(b, Const) and a.off is not None:
                    regs[d] = Ptr(a.type, a.off + b.value)
                elif isinstance(b, Ptr) and isinstance(a, Const) and b.off is not None:
                    regs[d] = Ptr(b.type, b.off + a.value)
                elif isinstance(a, Ptr) and not isinstance(b, Ptr):
                    regs[d] = Ptr(a.type, None)
                elif isinstance(b, Ptr) and not isinstance(a, Ptr):
                    regs[d] = Ptr(b.type, None)
                elif isinstance(a, Stack) or isinstance(b, Stack):
                    regs[d] = Stack()
                else:
                    regs.pop(d, None)
            else:
                regs.pop(d, None)
        except (ValueError, IndexError, TypeError):
            regs.pop(d, None)


def typed_accesses():
    """The measure's typed accesses by instruction, for lift.py: (function,
    its comment ordinal) -> (the type's display name, the register's offset
    into the object, the displacement, the width, a store)."""
    m = Map()
    for _ in range(4):
        m.run_pass(record=False)
    m.recording = {}
    m.run_pass(record=True)
    return {k: (m.display(t), off, disp, width, store)
            for k, (t, off, disp, width, store) in m.recording.items()}, m


def main():
    m = Map()
    for _ in range(4):
        m.run_pass(record=False)
    m.run_pass(record=True)

    areas_of = {}
    for t, fns in m.touch.items():
        areas_of[t] = collections.Counter(m.area[f] for f in fns)
    for a, fns in m.globals.items():
        areas_of["global@%08X" % a] = collections.Counter(m.area[f] for f in fns)

    def tags(t, areas):
        out = []
        if t.startswith("global@"):
            out.append("image")
        if set(areas) & FILE_AREAS:
            out.append("file")
        if set(areas) & GPU_AREAS:
            out.append("gpu")
        if set(areas) & SAVE_AREAS:
            out.append("save")
        if not t.startswith("global@") and not m.allocs.get(t):
            out.append("no allocation seen")
        return out

    rows = []
    for t in sorted(m.touch):
        fields = m.fields.get(t, {})
        size = max([o + max(w for (w, k) in c if isinstance(w, int)) for o, c in fields.items()
                    if isinstance(o, int) and any(isinstance(w, int) for (w, k) in c)] + [0])
        alloc_sizes = sorted({s for _, s in m.allocs.get(t, []) if s is not None})
        fdesc = []
        for off in sorted(fields, key=lambda o: (isinstance(o, str), o if isinstance(o, int) else 0)):
            c = fields[off]
            widths = sorted({f"{w}{k[0]}" for (w, k) in c if isinstance(w, int)})
            extra = sorted({f"{w}:{k}" for (w, k) in c if not isinstance(w, int)})[:2]
            fdesc.append(f"+{off}:{'/'.join(widths)}" + (f"[{';'.join(extra)}]" if extra else ""))
        areas = areas_of[t]
        rows.append({
            "type": m.display(t), "functions": len(m.touch[t]), "accesses": sum(sum(c.values()) for c in fields.values()),
            "fields": len(fields), "size_seen": size,
            "alloc_sizes": ",".join(map(str, alloc_sizes)), "alloc_sites": len(m.allocs.get(t, [])),
            "vtables": ",".join(sorted(m.vtables.get(t, ()))),
            "owner": "private" if len(areas) == 1 else "shared", "areas": len(areas),
            "area_list": ",".join(a for a, _ in sorted(areas.items(), key=lambda kv: (-kv[1], kv[0]))),
            "tags": ",".join(tags(t, areas)), "field_list": " ".join(fdesc)[:4000],
        })
    # The full fields, for the schema's seed (tools/schema_seed.py): not
    # committed (out/).
    full = {}
    for t in m.touch:
        fields = {}
        for off, c in m.fields.get(t, {}).items():
            fields[str(off)] = {
                "widths": sorted({f"{w}{k[0]}" for (w, k) in c if isinstance(w, int)}),
                "pointee": sorted({m.display(k) for (w, k) in c if w == "ptr"}),
                "fn": sorted({k for (w, k) in c if w == "fn"}),
                "image": sorted({k for (w, k) in c if w == "image"}),
                "count": sum(c.values()),
            }
        areas = areas_of[t]
        full[m.display(t)] = {
            "areas": dict(sorted(areas.items(), key=lambda kv: (-kv[1], kv[0]))), "functions": len(m.touch[t]),
            "alloc_sizes": sorted({s_ for _, s_ in m.allocs.get(t, []) if s_ is not None}),
            "vtables": sorted(m.vtables.get(t, ())), "tags": tags(t, areas), "fields": fields,
        }
    os.makedirs(os.path.join(ROOT, "itc_recomp", "out"), exist_ok=True)
    with open(os.path.join(ROOT, "itc_recomp", "out", "type_map.json"), "w") as f:
        json.dump(full, f, indent=0, sort_keys=True)
    with open(os.path.join(COVERAGE, "type_map.tsv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]), delimiter="\t")
        w.writeheader()
        w.writerows(rows)

    # The graph: areas, and the shared types between them (functions touching).
    edges = collections.Counter()
    node_types = collections.defaultdict(lambda: collections.Counter())
    for t, areas in areas_of.items():
        kind = "private" if len(areas) == 1 else "shared"
        for a in areas:
            node_types[a][kind] += 1
        if len(areas) > 1:
            ranked = sorted(areas)
            for i, a in enumerate(ranked):
                for b in ranked[i + 1:]:
                    edges[(a, b)] += min(areas[a], areas[b])
    degree = collections.Counter()
    for (a, b), wgt in edges.items():
        degree[a] += wgt
        degree[b] += wgt
    with open(os.path.join(COVERAGE, "type_map_areas.tsv"), "w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["area", "functions", "private_types", "shared_types", "shared_weight",
                    "tags", "top_neighbors"])
        fn_count = collections.Counter(m.area.values())
        for a in sorted(fn_count, key=lambda a: (degree[a], a)):
            nb = collections.Counter()
            for (x, y), wgt in edges.items():
                if x == a:
                    nb[y] += wgt
                elif y == a:
                    nb[x] += wgt
            tg = [n for n, s in (("file", FILE_AREAS), ("gpu", GPU_AREAS), ("save", SAVE_AREAS)) if a in s]
            w.writerow([a, fn_count[a], node_types[a]["private"], node_types[a]["shared"], degree[a],
                        ",".join(tg), ",".join(f"{n}:{v}" for n, v in nb.most_common(5))])

    s = m.stats
    total = sum(s.values())
    heap = s["typed"] + s["typed indexed"]
    print(f"functions read: {len(m.code)}; joins of two named classes refused: {m.refused}")
    print(f"loads and stores: {total}")
    for k, v in s.most_common():
        print(f"  {k:16} {v:8}  {100 * v / total:5.1f}%")
    nonframe = total - s["stack"] - s["thread block"]
    print(f"attributed (typed + image) of those not on the stack: "
          f"{100 * (heap + s['image']) / nonframe:.1f}%")
    obj = [r for r in rows if not r["type"].startswith("global@")]
    named = [r for r in obj if not r["type"].startswith("new<")]
    print(f"types: {len(obj)} ({len(named)} named, {len(obj) - len(named)} allocations without a "
          f"class); private {sum(r['owner'] == 'private' for r in obj)}, "
          f"shared {sum(r['owner'] == 'shared' for r in obj)}")
    print(f"image addresses accessed: {len(m.globals)}; "
          f"private {sum(1 for a in m.globals if len(areas_of['global@%08X' % a]) == 1)}")
    tagc = collections.Counter(tg for r in obj for tg in r["tags"].split(",") if tg)
    print("tags:", dict(tagc))
    print("allocation sites:", sum(len(v) for v in m.allocs.values()),
          "constructors' vtables found:", sum(len(v) for v in m.vtables.values()))
    print("most shared types (areas, functions):")
    for r in sorted(obj, key=lambda r: (-r["areas"], -r["functions"]))[:15]:
        print(f"  {r['type'][:48]:48} {r['areas']:3} {r['functions']:5}  {r['tags']}")
    print("areas, fewest shared ties first (leaves):")
    for a in sorted(set(m.area.values()), key=lambda a: (degree[a], a))[:15]:
        print(f"  {a:34} weight {degree[a]:6}  private {node_types[a]['private']:4}  "
              f"shared {node_types[a]['shared']:4}")
    # Where the typed accesses fall: on private types (step 3), shared
    # (step 4), types tagged file, GPU or save (steps 5 and 7), per area.
    split = collections.Counter()
    per_area = collections.defaultdict(collections.Counter)
    for r in obj:
        tg = set(r["tags"].split(",")) - {"", "no allocation seen"}
        bucket = ("file/gpu/save" if tg else r["owner"])
        split[bucket] += r["accesses"]
        for a in r["area_list"].split(","):
            per_area[a][bucket] += r["accesses"] / r["areas"]
    total_typed = sum(split.values())
    print("typed accesses by the types' kind:",
          ", ".join(f"{k} {100 * v / total_typed:.1f}%" for k, v in split.most_common()))
    print("areas most on private types (step 3's first; share private, typed accesses):")
    ranked = sorted(((v["private"] / max(1, sum(v.values())), a, sum(v.values()))
                     for a, v in per_area.items() if sum(v.values()) >= 200), reverse=True)
    for share, a, n in ranked[:20]:
        print(f"  {a:34} {100 * share:5.1f}%  {int(n):6}")
    with open(os.path.join(COVERAGE, "type_map_split.tsv"), "w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["area", "typed_accesses", "private", "shared", "file_gpu_save"])
        for a, v in sorted(per_area.items()):
            w.writerow([a, int(sum(v.values())), int(v["private"]), int(v["shared"]),
                        int(v["file/gpu/save"])])
    print("heaviest edges:")
    for (a, b), wgt in edges.most_common(12):
        print(f"  {a} -- {b}: {wgt}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
