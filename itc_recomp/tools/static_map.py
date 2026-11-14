#!/usr/bin/env python3
"""B7's static map: the call graph of every recompiled function, what's
reachable from the game's `main`, and what's left to make native in it
(docs/loops/b7-loop.md, "B7's goal"). Reports only; changes nothing in
the code. Writes docs/loops/static-map.md (made fresh on every run) and
itc_recomp/coverage/static_map.tsv (one line per function).

usage: static_map.py [--no-write] [--image <path>]

Reads:
  generated/   every sub_ body in itc_recomp/generated/default/*.cpp:
               direct calls (bl, and branches or tail calls to another
               function), the kernel and XAM imports each calls (__imp__X),
               the jump-table cases the recompiler split off as their own
               functions (a switch case that is a call), and address
               constants from the disassembly comments: lis followed by
               addi, ori or addis (any register, the value kept until the
               register is written, so addresses built across registers
               count), and D-form loads (lwz) off such a base
  the image    itc/xex_image/default.bin (docs/known-data.md; not
               committed): a constant that lands on a function's start is
               an edge; one that lands in .rdata or .data starts a table
               scan there (vtables, function tables, state machine
               tables): its words that are function starts are edges from
               the function that built the address, words that point into
               data are scanned once more (depth 2), and the scan stops
               at the next address any code or data points at, or after 8
               words with no pointer. A load from such an address reads
               the one word there. Without the image, only direct calls
               and constants landing on function starts count
  statuses     docs/functions/ and src/ overrides, with
               check_function_statuses.py's code; ours as b7_status.py
               decides it (an override not Wrapped, or Native, Merged or
               Dropped)
  coverage     itc_recomp/coverage/seen_in_play.txt (docs/coverage.md)
  src/         our C++ (src_edges.py): for each function with C++ (its
               override, or a definition holding ITC_NATIVE_RUNNING("sub_X
               ..."), followed through the plain C++ it calls), the guest
               functions it still reaches: sub_X calls, __imp__sub_X calls
               (not its own: the flag-off fallback), constants landing on a
               function's start, and tables or vtables at constants in data,
               scanned as above; a call into another native function's C++
               is an edge to that function

The graph:
  roots     main (sub_82090108); then every function seen in play that
            the original's graph (below) doesn't reach from main (xstart,
            the C runtime's start-up, thread entries, callbacks the SDK
            calls) is an extra root; and the guest functions named by C++
            that no native function's C++ reaches (host code)
  edges     an unported function's are the original's (generated/); an
            ours function's are its C++'s when it has C++, reached through
            the function table (its override; one whose adapter was removed
            is taken as dispatched to its C++) or from other C++, and the
            original's when it's reached through an __imp__ call or has no
            C++ (Merged). Dropped functions aren't followed
  the original's graph  the original's edges followed through ours too (the
            measure before 2026-11-14): what it reaches outside the replaced
            libraries that the real graph doesn't is "originals no longer
            called", grouped by the native function that left the call out
  folded    a jump-table case split off as its own function, referenced
            only by its parent (a switch case in the parent, or a table
            the parent builds whose entries lie just after it), is folded
            into the parent: its edges and lines are the parent's
  excluded  the libraries replaced wholesale ("Rules that still hold" in
            docs/loops/b7-loop.md): sub_822B8F10 (the HLSL compiler) and
            the movie library's ranges 82481A70-82575B07 and
            825DC200-825FC1B8, with everything reachable only through them;
            a function there that game code still running as PPC calls
            directly, or that our C++ calls, is the game's
  unported  reachable, not ours, not excluded (Wrapped counts here)

The frontier: unported functions (or call cycles of them) whose callees
are all ours, excluded or imports. A function's "unblocks" is how many
unported functions above it, followed up through unported callers only,
wait on it; "frees" is how many unported callers would join the frontier
if it alone became ours.
"""

import collections
import datetime
import importlib.util
import os
import re
import struct
import subprocess
import sys

sys.dont_write_bytecode = True

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(TOOLS, "..", ".."))
GENERATED = os.path.join(ROOT, "itc_recomp", "generated", "default")
SEEN = os.path.join(ROOT, "itc_recomp", "coverage", "seen_in_play.txt")
IMAGE = os.path.join(ROOT, "itc", "xex_image", "default.bin")
REPORT = os.path.join(ROOT, "docs", "loops", "static-map.md")
TSV = os.path.join(ROOT, "itc_recomp", "coverage", "static_map.tsv")

MAIN = "sub_82090108"
HLSL = "sub_822B8F10"
EXCLUDED_RANGES = [(0x82481A70, 0x82575B07), (0x825DC200, 0x825FC1B8)]
BASE = 0x82000000
TEXT = (0x82090000, 0x825DC064)
DATA_RANGES = [(0x82000600, 0x820773C4),  # .rdata
               (0x82600000, 0x8297833C)]  # .data
HELPER = re.compile(r"__(save|rest)(gpr|fpr|vmx)")
# Edge kinds in the TSV: the original's direct call, address constant or
# data table; our C++'s call to a native function's C++, call or address
# through the function table, __imp__ call, or table it names.
KIND = {"direct": "d", "const": "c", "data": "t", "plain": "p", "native": "n",
        "imp": "i", "table": "t"}

# Disassembly comment parsing.
COMMENT = re.compile(r"^\s*// ([a-z.]+)\s*(.*)$")
REG = re.compile(r"^r(\d+)$")
MEM = re.compile(r"^(-?\d+)\(r(\d+)\)$")
NO_DEST = ("st", "cmp", "b", "mt", "dcb", "tw", "td", "sync", "isync", "eieio",
           "lwsync", "nop", "icbi", "fcmp", "vcmp")
VOLATILE = [0] + list(range(3, 13))


def in_data(v):
    return any(lo <= v < hi for lo, hi in DATA_RANGES)


def excluded_address(a):
    return any(lo <= a <= hi for lo, hi in EXCLUDED_RANGES)


def load_statuses():
    spec = importlib.util.spec_from_file_location(
        "statuses", os.path.join(TOOLS, "check_function_statuses.py"))
    statuses = importlib.util.module_from_spec(spec)
    saved, sys.argv = sys.argv, [sys.argv[0], ROOT]
    spec.loader.exec_module(statuses)
    sys.argv = saved
    overrides = statuses.overrides()
    status, table = {}, {}
    for where, subs, st in statuses.rows():
        file = where.rsplit(":", 1)[0]
        for sub in subs:
            status.setdefault(sub, st)
            table.setdefault(sub, file)
    return overrides, status, table


def parse_generated():
    """Per function: direct callees, tail-branch callees, switch-case
    callees, imports, lines, indirect calls, address constants and loads."""
    addr_of = {}
    with open(os.path.join(GENERATED, "itc_register.cpp")) as f:
        for m in re.finditer(r"SetFunction\((0x[0-9A-Fa-f]+), (\w+)\)", f.read()):
            addr_of[m.group(2)] = int(m.group(1), 16)

    fn = {}
    define = re.compile(r"^DEFINE_REX_FUNC\((\w+)\)")
    call = re.compile(r"\b((?:sub_[0-9A-F]{8})|__(?:save|rest)(?:gpr|fpr|vmx)\w*|xstart)\(ctx")
    imp = re.compile(r"__imp__([A-Za-z]\w+)\(")
    case = re.compile(r"^\s*case \d+:\s*$")
    for name in sorted(os.listdir(GENERATED)):
        if not re.match(r"itc_recomp\.\d+\.cpp$", name):
            continue
        cur = None
        with open(os.path.join(GENERATED, name)) as f:
            for line in f:
                m = define.match(line)
                if m:
                    cur = m.group(1)
                    d = fn[cur] = {"calls": set(), "tails": set(), "cases": set(),
                                   "imports": set(), "lines": 0, "indirect": 0,
                                   "addrs": set(), "loads": set(), "bctr": 0}
                    val = {}
                    last_mn = ""
                    prev_case = False
                    continue
                if cur is None:
                    continue
                s = line.lstrip()
                if s.startswith("//"):
                    cm = COMMENT.match(line)
                    if not cm:
                        continue
                    mn, ops = cm.group(1), cm.group(2).replace(" ", "")
                    last_mn = mn
                    if mn.endswith("ctr") and mn.startswith("b"):  # bctr, bnectr: a jump
                        d["bctr"] += 1
                    ops = ops.split(",") if ops else []
                    track(mn, ops, val, d)
                    continue
                d["lines"] += 1
                if "REX_CALL_INDIRECT_FUNC" in s:
                    d["indirect"] += 1
                if "(ctx" in s:
                    for c in call.findall(s):
                        if c == cur:
                            continue
                        if prev_case:
                            d["cases"].add(c)
                        elif last_mn.startswith("bl") and not last_mn.startswith("blr"):
                            d["calls"].add(c)
                        else:
                            d["tails"].add(c)
                    if "__imp__" in s:
                        for i in imp.findall(s):
                            if not i.startswith("sub_"):
                                d["imports"].add(i)
                prev_case = bool(case.match(line))
    return fn, addr_of


def track(mn, ops, val, d):
    """One disassembly comment: keeps the registers holding known constants."""
    if mn == "lis" and len(ops) == 2:
        r = REG.match(ops[0])
        if r:
            val[int(r.group(1))] = (int(ops[1]) << 16) & 0xFFFFFFFF
        return
    if mn in ("addi", "ori", "addis", "addic") and len(ops) == 3:
        rd, ra = REG.match(ops[0]), REG.match(ops[1])
        if rd and ra:
            a = int(ra.group(1))
            if a in val and not (mn.startswith("addi") and a == 0):
                imm = int(ops[2])
                if mn == "ori":
                    v = val[a] | (imm & 0xFFFF)
                elif mn == "addis":
                    v = val[a] + (imm << 16)
                else:
                    v = val[a] + imm
                v &= 0xFFFFFFFF
                val[int(rd.group(1))] = v
                d["addrs"].add(v)
                return
            val.pop(int(rd.group(1)), None)
        return
    if mn == "mr" and len(ops) == 2:
        rd, rs = REG.match(ops[0]), REG.match(ops[1])
        if rd and rs:
            s = int(rs.group(1))
            if s in val:
                val[int(rd.group(1))] = val[s]
            else:
                val.pop(int(rd.group(1)), None)
        return
    if mn.startswith("bl") and not mn.startswith("blr"):  # a call: volatiles gone
        for r in VOLATILE:
            val.pop(r, None)
        return
    if mn in ("lwz", "lwzu", "ld", "ldu") and len(ops) == 2:
        m = MEM.match(ops[1])
        rd = REG.match(ops[0])
        if m and rd and m.group(2) == "1" and ("s", int(m.group(1))) in val:
            # A constant spilled to the stack and read back.
            val[int(rd.group(1))] = val[("s", int(m.group(1)))]
            return
        if m and int(m.group(2)) in val:
            d["loads"].add((val[int(m.group(2))] + int(m.group(1))) & 0xFFFFFFFF)
        if m and mn.endswith("u"):
            val.pop(int(m.group(2)), None)
    elif mn.startswith("st") and len(ops) == 2:
        m = MEM.match(ops[1])
        rs = REG.match(ops[0])
        if m and m.group(2) == "1":  # a stack slot: keep what it holds
            key = ("s", int(m.group(1)))
            if mn in ("stw", "std") and rs and int(rs.group(1)) in val:
                val[key] = val[int(rs.group(1))]
            else:
                val.pop(key, None)
        if m and mn.endswith("u"):
            val.pop(int(m.group(2)), None)
        return
    if mn.startswith(NO_DEST) and not mn.startswith("bl"):
        return
    if ops:
        r = REG.match(ops[0])
        if r:
            val.pop(int(r.group(1)), None)


class Image:
    def __init__(self, path):
        with open(path, "rb") as f:
            self.data = f.read()

    def word(self, a):
        o = a - BASE
        if o < 0 or o + 4 > len(self.data):
            return None
        return struct.unpack_from(">I", self.data, o)[0]


def build(image_path=IMAGE, quiet=True):
    """The whole map, as a dict (b7_groups.py --static uses it too)."""
    fn, addr_of = parse_generated()
    defined = set(fn)
    name_of = {a: n for n, a in addr_of.items() if n in defined}
    starts = set(name_of)

    image = Image(image_path) if os.path.exists(image_path) else None
    scan = None

    # Edges: kind per (caller, callee): direct, const, data.
    edges = collections.defaultdict(dict)
    for f, d in fn.items():
        for c in d["calls"] | d["tails"] | d["cases"]:
            if c in defined:
                edges[f][c] = "direct"
        for v in d["addrs"]:
            if v in starts and name_of[v] != f:
                edges[f].setdefault(name_of[v], "const")

    table_refs = collections.defaultdict(set)  # data address -> functions building it
    orphan_data_pointers = 0
    data_scanned = 0
    if image:
        # Boundaries: every data address code builds (not loads: code reads
        # single vtable slots by address), and every data word pointing
        # into data.
        bounds = set()
        for f, d in fn.items():
            for v in d["addrs"]:
                if in_data(v):
                    bounds.add(v)
        data_func_words = set()
        for lo, hi in DATA_RANGES:
            for a in range(lo, hi - 3, 4):
                w = image.word(a)
                if w is None:
                    continue
                if in_data(w):
                    bounds.add(w)
                elif w in starts:
                    data_func_words.add(a)
        bounds_sorted = sorted(bounds)
        import bisect

        scan_cache = {}

        def scan(t, depth):  # noqa: F811 (the one used below)
            key = (t, depth)
            if key in scan_cache:
                return scan_cache[key]
            found, words = set(), set()
            i = bisect.bisect_right(bounds_sorted, t)
            stop = bounds_sorted[i] if i < len(bounds_sorted) else t + 4 * 512
            stop = min(stop, t + 4 * 512)
            gap, a = 0, t
            while a < stop and in_data(a):
                w = image.word(a)
                if w in starts:
                    found.add(name_of[w])
                    words.add(a)
                    gap = 0
                elif w is not None and in_data(w) and w != t:
                    gap = 0
                    if depth > 0:
                        f2, w2 = scan(w, depth - 1)
                        found |= f2
                        words |= w2
                else:
                    gap += 1
                    if gap >= 8:
                        break
                a += 4
            scan_cache[key] = (found, words)
            return found, words

        covered = set()
        for f, d in fn.items():
            for v in d["addrs"]:
                if in_data(v):
                    found, words = scan(v, 1)
                    covered |= words
                    data_scanned += 1
                    for c in found:
                        if c != f:
                            edges[f].setdefault(c, "data")
                            table_refs[c].add(f)
            for v in d["loads"]:
                if in_data(v):
                    w = image.word(v)
                    if w in starts and name_of[w] != f:
                        edges[f].setdefault(name_of[w], "data")
                        covered.add(v)
                    elif w is not None and in_data(w):
                        found, words = scan(w, 1)
                        covered |= words
                        for c in found:
                            if c != f:
                                edges[f].setdefault(c, "data")
        orphan_data_pointers = len(data_func_words - covered)
        orphan_targets = {name_of[image.word(a)] for a in data_func_words - covered}
    else:
        orphan_targets = set()

    # Fold the jump-table cases split off as functions.
    referrers = collections.defaultdict(set)
    for f, cs in edges.items():
        for c in cs:
            referrers[c].add(f)
    parent = {}
    for f, d in fn.items():
        for c in d["cases"]:
            if c in defined and referrers[c] == {f} and c not in orphan_targets:
                parent[c] = f
    # A table the parent builds, with a jump through ctr (bctr, not bctrl),
    # whose entries lie in the parent's body: every function between the
    # parent and the entry is such an entry too.
    cand = collections.defaultdict(set)
    for c, fs in table_refs.items():
        if c in parent or c not in defined or len(referrers[c]) != 1 or c in orphan_targets:
            continue
        p = next(iter(referrers[c]))
        if p in fs and edges[p].get(c) == "data" and fn[p]["bctr"] and \
                0 < addr_of[c] - addr_of[p] < 0x8000:
            cand[p].add(c)
    by_addr = sorted(defined, key=lambda n: addr_of[n])
    position = {n: i for i, n in enumerate(by_addr)}
    for p, cs in cand.items():
        i = position[p] + 1
        while i < len(by_addr) and (by_addr[i] in cs or parent.get(by_addr[i]) == p):
            parent.setdefault(by_addr[i], p)
            i += 1

    def top(c):
        seen_ = set()
        while c in parent and c not in seen_:
            seen_.add(c)
            c = parent[c]
        return c

    folded = {c: top(c) for c in parent}
    nodes = defined - set(folded)
    callees = {f: {} for f in nodes}
    lines = {f: fn[f]["lines"] for f in nodes}
    indirect = {f: fn[f]["indirect"] for f in nodes}
    imports = {f: set(fn[f]["imports"]) for f in nodes}
    members = {f: [f] for f in nodes}
    for c, p in folded.items():
        lines[p] += fn[c]["lines"]
        indirect[p] += fn[c]["indirect"]
        imports[p] |= fn[c]["imports"]
        members[p].append(c)
    for f, cs in edges.items():
        src = folded.get(f, f)
        for c, kind in cs.items():
            dst = folded.get(c, c)
            if dst != src:
                prev = callees[src].get(dst)
                if prev is None or (prev != "direct" and kind == "direct"):
                    callees[src][dst] = kind

    # States.
    overrides, status, table = load_statuses()
    with open(SEEN) as f:
        seen_raw = {f"sub_{int(l, 16):08X}" for l in f if l.strip()}
    name_by_addr = {a: n for n, a in addr_of.items()}
    seen = set()
    for s in seen_raw:
        a = int(s[4:], 16)
        n = name_by_addr.get(a, s)
        n = folded.get(n, n)
        if n in nodes:
            seen.add(n)

    def ours(f):
        if HELPER.match(f):
            return True
        return (f in overrides and status.get(f) != "Wrapped") or \
            status.get(f) in ("Native", "Merged", "Dropped")

    def expands(f):
        return status.get(f) != "Dropped"

    # Our C++'s edges (src_edges.py): what each native function's C++ still
    # reaches in the guest.
    spec = importlib.util.spec_from_file_location("src_edges", os.path.join(TOOLS, "src_edges.py"))
    src_edges = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(src_edges)

    def classify_hex(v):
        if v in starts:
            return [(name_of[v], "native")]
        if scan is not None and in_data(v):
            return [(c, "table") for c in sorted(scan(v, 1)[0])]
        return []

    nat, via, host, cdefs, entries, src_stats = src_edges.native_edges(
        lambda s: s in defined, classify_hex)
    rank = {"plain": 0, "native": 1, "imp": 2, "table": 3}
    native = {}  # f -> {callee: kind}, from its C++
    native_via = {}  # f -> {callee: the C++ definition naming it}
    for f, cs in nat.items():
        f2 = folded.get(f, f)
        if f2 not in nodes:
            continue
        d = native.setdefault(f2, {})
        dv = native_via.setdefault(f2, {})
        for c, k in cs.items():
            c2 = folded.get(c, c)
            if c2 == f2 or c2 not in nodes:
                continue
            prev = d.get(c2)
            if prev is None or rank[k] < rank[prev]:
                d[c2] = k
                dv[c2] = via[f][c]
    orig = callees  # the original's edges (generated/), per function

    # A state is (function, variant): "n" runs our C++ (native[f]), "o" the
    # original's body (generated/). Only functions that are ours have both.
    def target(c, kind):
        if not ours(c):
            return (c, "o")
        if kind == "plain":  # our C++ calling the native function's C++
            return (c, "n")
        if kind == "imp":  # __imp__sub_X: the original's body
            return (c, "o")
        # Through the function table: the override when there is one. A
        # function with C++ but no override (its adapter removed once its
        # callers were native) is taken as run natively: native code that
        # names its address dispatches to the C++ (RunInit, say), and a
        # recompiled caller left is unported work of its own.
        return (c, "n") if c in overrides or c in native else (c, "o")

    def out_edges(state):
        f, v = state
        if not expands(f):
            return ()
        return (native.get(f, {}) if v == "n" else orig[f]).items()

    def walk(starts_, block=None, expand=None):
        out, stack = set(), list(starts_)
        while stack:
            st_ = stack.pop()
            if st_ in out or st_[0] not in nodes or (block and block(st_[0])):
                continue
            out.add(st_)
            if expand and not expand(st_):
                continue
            for c, k in out_edges(st_):
                if c in nodes:
                    t = target(c, k)
                    if t not in out:
                        stack.append(t)
        return out

    # The original's graph (its edges followed through ours too, as before
    # 2026-11-14): the extra roots, and what's no longer called.
    def reach_orig(roots, block=None):
        out, stack = set(), list(roots)
        while stack:
            f = stack.pop()
            if f in out or f not in nodes or (block and block(f)):
                continue
            out.add(f)
            if not expands(f):
                continue
            stack.extend(c for c in orig[f] if c not in out)
        return out

    from_main_orig = reach_orig([MAIN])
    # Dropped functions seen before they were dropped no longer run.
    extra = sorted((f for f in seen - from_main_orig if status.get(f) != "Dropped"),
                   key=lambda n: addr_of[n])
    orig_reach_all = reach_orig([MAIN] + extra)
    extra_set = set(extra)
    under_others = reach_orig([c for r in extra if expands(r) for c in orig[r]]) & extra_set
    extra_top = [r for r in extra if r not in under_others]

    # Host code no native function's C++ reaches (the app's start-up, our
    # threads) that names guest functions: roots too.
    host_targets = collections.defaultdict(set)
    for did, ts in host.items():
        for c, k in ts:
            c2 = folded.get(c, c)
            if c2 in nodes:
                host_targets[c2].add(cdefs[did])
    root_states = [target(MAIN, "direct")] + [target(r, "direct") for r in extra]
    host_states = [target(c, "native") for c in sorted(host_targets)]
    all_roots = root_states + host_states

    from_main_states = walk([target(MAIN, "direct")])
    from_main = {f for f, _ in from_main_states}
    reach_states = walk(all_roots)
    reach_all = {f for f, _ in reach_states}
    without_host = {f for f, _ in walk(root_states)}
    host_only = reach_all - without_host

    seeds = {HLSL} | {f for f in nodes if excluded_address(addr_of[f])}
    seeds &= nodes
    outside = reach_all
    # Reach without entering the seeds: what the game's own code reaches. A
    # function in the ranges that game code still running as PPC calls
    # directly, or that our C++ calls, is the game's ("checked against its callers first": the
    # linker shares small functions between the library and the game), and
    # so is what it reaches; repeated until nothing changes.
    promoted = set()
    while True:
        game_states = walk(all_roots, block=lambda f: f in seeds and f not in promoted)
        new = set()
        for g, v in game_states:
            if not expands(g):
                continue
            if v == "o":
                calls = (c for c, kind in orig[g].items() if kind == "direct")
            else:  # our C++ calling it (not a table it names)
                calls = (c for c, kind in native.get(g, {}).items() if kind != "table")
            for c in calls:
                if c in seeds and c != HLSL and c not in promoted:
                    new.add(c)
        if not new:
            break
        promoted |= new
    game = {f for f, _ in game_states}
    native_into_seeds = sorted({(g, c) for g, v in game_states if v == "n"
                                for c in native.get(g, {}) if c in seeds and c not in promoted})
    excluded = outside - game
    range_called = sorted(promoted, key=lambda n: addr_of[n])

    def state(f):
        if f in excluded:
            return "excluded"
        if ours(f):
            return "ours"
        return "unported"

    reachable = reach_all
    st = {f: state(f) for f in reachable}
    unported = {f for f in reachable if st[f] == "unported"}

    # Callers along the edges the walk took.
    callers_state = collections.defaultdict(set)
    for s_ in reach_states:
        for c, k in out_edges(s_):
            if c in nodes:
                t = target(c, k)
                if t in reach_states:
                    callers_state[t].add(s_)
    callers_all = collections.defaultdict(set)
    for t, ss in callers_state.items():
        for s_ in ss:
            callers_all[t[0]].add(s_[0])

    # Ours functions whose original body still runs: reached in the "o"
    # variant (an unported caller, a guest call or vtable with no override
    # behind it, or an __imp__ call from our C++).
    ours_orig = {f for f, v in game_states if v == "o" and st.get(f) == "ours"
                 and expands(f) and not HELPER.match(f)}
    ours_orig_from_native = {f for f in ours_orig
                             if any(v == "n" for _, v in callers_state[(f, "o")])}

    # The callees each function has in the graph: our C++'s for ours with
    # C++ (an override or ITC_NATIVE_RUNNING), the original's otherwise.
    eff = {}
    for f in nodes:
        if ours(f) and (f in overrides or f in native):
            eff[f] = native.get(f, {})
        else:
            eff[f] = orig[f]
    callees = eff

    # Unported subgraph: SCCs, frontier, ancestors.
    succ = {f: [c for c in callees[f] if c in unported] for f in unported}
    pred = collections.defaultdict(list)
    for f, cs in succ.items():
        for c in cs:
            pred[c].append(f)
    comp = tarjan(unported, succ)
    comp_members = collections.defaultdict(list)
    for f, c in comp.items():
        comp_members[c].append(f)
    csucc = collections.defaultdict(set)
    cpred = collections.defaultdict(set)
    for f, cs in succ.items():
        for c in cs:
            if comp[f] != comp[c]:
                csucc[comp[f]].add(comp[c])
                cpred[comp[c]].add(comp[f])
    frontier = {f for f in unported if not csucc[comp[f]]}

    # Ancestors (unported, through unported callers) as bitsets over SCCs.
    order = topo(comp_members.keys(), cpred)  # callers before callees
    bit = {c: 1 << i for i, c in enumerate(order)}
    size_of = {c: len(comp_members[c]) for c in order}
    anc = {}
    for c in order:
        a = 0
        for p in cpred[c]:
            a |= anc[p] | bit[p]
        anc[c] = a
    # Most components have one member: popcount, plus the extra members of
    # the few cycles.
    multi = [(bit[c], size_of[c] - 1) for c in order if size_of[c] > 1]

    def fast_count(bits):
        n = bin(bits).count("1")
        for b, extra_n in multi:
            if bits & b:
                n += extra_n
        return n

    unblocks = {}
    for c in order:
        n = fast_count(anc[c]) + size_of[c] - 1
        for f in comp_members[c]:
            unblocks[f] = n
    # frees: unported callers whose only unported callee (outside their
    # cycle) is this one's component.
    frees = collections.Counter()
    for c in order:
        if len(csucc[c]) == 1:
            frees[next(iter(csucc[c]))] += size_of[c]
    frees_of = {f: frees[comp[f]] for f in unported}

    # Unported functions reached only through ours (the walk stopped at
    # every ours function other than the roots): our C++ still calls them
    # (a guest call, a fallback for a branch, a vtable it stores), or an ours
    # function's original body does (no override behind a guest call, or an
    # unported caller of a function whose adapter was removed).
    root_set = set(all_roots)
    under_unported = {f for f, _ in walk(
        all_roots, expand=lambda s_: s_ in root_set or st.get(s_[0]) == "unported")}
    behind_ours = unported - under_unported
    behind_native = {f for f in behind_ours
                     if any(v == "n" for _, v in callers_state[(f, "o")])}

    # Originals no longer called: reachable through the original's graph
    # (outside the replaced libraries) but not through ours. Grouped by the
    # native function whose C++ left the call out: its original's callees
    # no longer reached, and what hangs only under them.
    orig_game = reach_orig([MAIN] + extra, block=lambda f: f in seeds)
    lost = orig_game - reachable
    lost_groups = []
    for f, v in sorted(reach_states, key=lambda s_: addr_of[s_[0]]):
        if v != "n" or not expands(f):
            continue
        direct = sorted((c for c in orig[f] if c in lost), key=lambda n: addr_of[n])
        if not direct:
            continue
        under = reach_orig(direct, block=lambda c: c not in lost)
        lost_groups.append({"caller": f, "direct": direct, "under": under,
                            "not_ours": sum(1 for c in under if not ours(c)),
                            "lines": sum(lines[c] for c in under),
                            "seen": sum(1 for c in under if c in seen)})
    lost_groups.sort(key=lambda g: (-len(g["under"]), addr_of[g["caller"]]))

    # 82575B08-8257FFFF: the movie library's end disagrees between
    # b7-loop.md (82575B07) and movies.md; what's reached there, by whom.
    gap = sorted((f for f in reachable if 0x82575B08 <= addr_of[f] <= 0x8257FFFF),
                 key=lambda n: addr_of[n])
    gap_callers = {f: sorted(callers_all[f], key=lambda n: addr_of[n]) for f in gap}

    return {
        "fn": fn, "addr_of": addr_of, "nodes": nodes, "callees": callees,
        "orig_callees": orig, "native": native, "native_via": native_via,
        "lines": lines, "indirect": indirect, "imports": imports,
        "members": members, "folded": folded, "seen": seen, "status": status,
        "table": table, "ours": ours, "from_main": from_main, "extra": extra,
        "extra_top": extra_top, "reachable": reachable, "excluded": excluded,
        "state": st, "unported": unported, "comp": comp,
        "comp_members": comp_members, "csucc": csucc, "frontier": frontier,
        "anc": anc, "bit": bit, "order": order, "fast_count": fast_count,
        "unblocks": unblocks, "frees": frees_of, "callers": callers_all,
        "succ": succ, "pred": pred, "range_called": range_called,
        "image": image is not None, "orphan_data_pointers": orphan_data_pointers,
        "orphan_targets": orphan_targets, "data_scanned": data_scanned,
        "behind_ours": behind_ours, "behind_native": behind_native,
        "ours_orig": ours_orig, "ours_orig_from_native": ours_orig_from_native,
        "orig_callers": {f: callers_state[(f, "o")] for f in ours_orig},
        "host_targets": host_targets, "host_only": host_only,
        "orig_reach_all": orig_reach_all, "lost": lost, "lost_groups": lost_groups,
        "gap": gap, "gap_callers": gap_callers, "native_into_seeds": native_into_seeds,
        "src_stats": src_stats, "overrides": overrides,
    }


def tarjan(nodes, succ):
    index, low, comp, stack, on = {}, {}, {}, [], set()
    counter = [0]
    ncomp = [0]
    for v0 in sorted(nodes):
        if v0 in index:
            continue
        work = [(v0, iter(succ[v0]))]
        index[v0] = low[v0] = counter[0]
        counter[0] += 1
        stack.append(v0)
        on.add(v0)
        while work:
            v, it = work[-1]
            pushed = False
            for w in it:
                if w not in index:
                    index[w] = low[w] = counter[0]
                    counter[0] += 1
                    stack.append(w)
                    on.add(w)
                    work.append((w, iter(succ[w])))
                    pushed = True
                    break
                if w in on:
                    low[v] = min(low[v], index[w])
            if pushed:
                continue
            work.pop()
            if work:
                low[work[-1][0]] = min(low[work[-1][0]], low[v])
            if low[v] == index[v]:
                while True:
                    w = stack.pop()
                    on.discard(w)
                    comp[w] = ncomp[0]
                    if w == v:
                        break
                ncomp[0] += 1
    return comp


def topo(comps, cpred):
    """Components ordered callers first (Kahn over the condensation)."""
    comps = list(comps)
    indeg = {c: len(cpred[c]) for c in comps}
    csucc = collections.defaultdict(set)
    for c in comps:
        for p in cpred[c]:
            csucc[p].add(c)
    ready = sorted(c for c in comps if indeg[c] == 0)
    out = []
    while ready:
        c = ready.pop()
        out.append(c)
        for s in csucc[c]:
            indeg[s] -= 1
            if indeg[s] == 0:
                ready.append(s)
    return out


class DSU:
    def __init__(self):
        self.p = {}

    def find(self, x):
        self.p.setdefault(x, x)
        while self.p[x] != x:
            self.p[x] = self.p[self.p[x]]
            x = self.p[x]
        return x

    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a != b:
            self.p[max(a, b)] = min(a, b)


def clusters(m, which):
    """Groups functions of `which` by shared unported callers (within 0x4000
    bytes of each other), neighbors in address order, and call cycles."""
    addr = m["addr_of"]
    dsu = DSU()
    for f in which:
        dsu.find(f)
    allsorted = sorted(m["nodes"], key=lambda n: addr[n])
    for a, b in zip(allsorted, allsorted[1:]):
        if a in which and b in which:
            dsu.union(a, b)
    for f in which:
        for g in m["comp_members"][m["comp"][f]] if f in m["comp"] else []:
            if g in which:
                dsu.union(f, g)
    by_caller = collections.defaultdict(list)
    for f in which:
        for c in m["pred"].get(f, []):
            by_caller[c].append(f)
    for c, fs in by_caller.items():
        fs.sort(key=lambda n: addr[n])
        for x, y in zip(fs, fs[1:]):
            if addr[y] - addr[x] <= 0x4000:
                dsu.union(x, y)
    groups = collections.defaultdict(list)
    for f in which:
        groups[dsu.find(f)].append(f)
    out = []
    for fs in groups.values():
        fs.sort(key=lambda n: addr[n])
        bits = 0
        own = 0
        for f in fs:
            c = m["comp"][f]
            bits |= m["anc"][c]
            own |= m["bit"][c]
        n = m["fast_count"](bits & ~own)
        out.append({"members": fs, "unblocks": n,
                    "lines": sum(m["lines"][f] for f in fs),
                    "seen": sum(1 for f in fs if f in m["seen"]),
                    "files": sorted({m["table"][g] for f in fs for g in m["members"][f]
                                     if g in m["table"]})})
    out.sort(key=lambda c: (-c["unblocks"], -len(c["members"]), addr[c["members"][0]]))
    return out


def fmt_funcs(m, fs, limit=12):
    parts = []
    for f in fs[:limit]:
        tag = "" if f in m["seen"] else "*"
        parts.append(f"`{f}`{tag} {m['lines'][f]}")
    more = f", ... {len(fs) - limit} more" if len(fs) > limit else ""
    return ", ".join(parts) + more


def write_report(m, path, tsv_path):
    addr = m["addr_of"]
    reachable, st = m["reachable"], m["state"]
    unported = m["unported"]
    ours_n = sum(1 for f in reachable if st[f] == "ours")
    excl_n = sum(1 for f in reachable if st[f] == "excluded")
    seen_unp = sum(1 for f in unported if f in m["seen"])
    from_main = m["from_main"]
    unp_main = sum(1 for f in unported if f in from_main)
    calling_imports = sum(1 for f in reachable if m["imports"][f])
    unp_imports = sum(1 for f in unported if m["imports"][f])
    try:
        commit = subprocess.run(["git", "-C", ROOT, "rev-parse", "--short", "HEAD"],
                                capture_output=True, text=True).stdout.strip()
        branch = subprocess.run(["git", "-C", ROOT, "rev-parse", "--abbrev-ref", "HEAD"],
                                capture_output=True, text=True).stdout.strip()
    except OSError:
        commit, branch = "?", "?"

    L = []
    w = L.append
    w("# B7's static map")
    w("")
    w("Made by `itc_recomp/tools/static_map.py` on every run; never edit it by "
      "hand (run the tool again). What it reads, how edges are found and its "
      "gaps: the tool's docstring and \"The static map\" in `docs/coverage.md`.")
    w("")
    w(f"Made {datetime.date.today().isoformat()} at commit `{commit}` (`{branch}`).")
    w("")
    w("## Totals")
    w("")
    w(f"- Recompiled `sub_` functions: {len(m['fn'])}; {len(m['folded'])} jump-table "
      f"cases folded into their parents, leaving {len(m['nodes'])}.")
    w(f"- Reachable: {len(reachable)} ({len(from_main)} from `main`, "
      f"{len(reachable) - len(from_main)} more from {len(m['extra'])} extra roots: functions "
      f"seen in play that `main` doesn't reach, {len(m['extra_top'])} of them not under another).")
    w(f"- Ours: {ours_n}. Excluded (replaced libraries): {excl_n}.")
    w(f"- **Unported: {len(unported)}** ({unp_main} under `main`): seen in play {seen_unp}, "
      f"never seen {len(unported) - seen_unp}; {sum(m['lines'][f] for f in unported)} "
      f"generated lines.")
    bo = m["behind_ours"]
    w(f"- Of the unported, {len(unported) - len(bo)} hang under unported code (or a root); "
      f"{len(bo)} are reached only through ours ({sum(1 for f in bo if f in m['seen'])} "
      f"seen in play), {len(m['behind_native'])} of them called by our C++ itself (a "
      f"guest call by name or address, a fallback for a branch, a table or vtable it "
      f"stores) and the rest under those; the TSV's `behind_ours` column lists them.")
    ss = m["src_stats"]
    nat_fs = [f for f in m["native"] if st.get(f) == "ours"]
    w(f"- Ours functions' edges come from our C++ (`src_edges.py`): {ss['defs']} "
      f"definitions in {ss['files']} files of `src/` (harnesses left out), "
      f"{ss['entries']} functions with C++ (an override or `ITC_NATIVE_RUNNING`), "
      f"{len(nat_fs)} of them reachable. Ours functions without C++ (Merged, or "
      f"whose print names no address) keep the original's edges where something "
      f"still reaches them.")
    oo, oon = m["ours_orig"], m["ours_orig_from_native"]
    w(f"- Ours functions whose original body is still reachable: {len(oo)} ({len(oon)} "
      f"from our C++: an `__imp__` fallback when the native C++ declines, or a guest "
      f"call to a function with neither override nor C++); listed below.")
    w(f"- Host code no native function's C++ reaches but that names guest functions "
      f"(constants defined but not resolved to a user, a few hooks' helpers): "
      f"{sum(len(v) for v in [m['host_targets']])} functions named, taken as roots; they "
      f"add {len(m['host_only'])} reachable functions.")
    w(f"- Before 2026-11-14's fix the graph followed the original's edges through ours "
      f"too: {len(m['orig_reach_all'])} reachable. **Originals no longer called: "
      f"{len(m['lost'])}** ({sum(1 for f in m['lost'] if not m['ours'](f))} not ours, "
      f"{sum(1 for f in m['lost'] if m['ours'](f))} ours whose work our C++ took in; "
      f"{sum(m['lines'][f] for f in m['lost'])} lines, "
      f"{sum(1 for f in m['lost'] if f in m['seen'])} seen in play): reachable through the "
      f"original's graph (outside the replaced libraries), not through ours; below, "
      f"by the native function that left the call out.")
    w(f"- Frontier (ready now): {len(m['frontier'])} functions.")
    w(f"- Reachable functions calling imports: {calling_imports} ({unp_imports} unported).")
    if m["image"]:
        w(f"- Image read: {m['data_scanned']} code-built data addresses scanned; "
          f"{m['orphan_data_pointers']} function pointers in data that no code-built "
          f"address reaches ({len(m['orphan_targets'])} functions; not edges).")
    else:
        w("- **The image (`itc/xex_image/default.bin`) wasn't found: vtables and "
          "function tables in data are missing.**")
    w("")
    w("A `*` after a function: never seen in play. Sizes are generated lines.")
    w("")

    w("## Blockers, by what they unblock")
    w("")
    w("Unported functions ranked by how many unported functions above them "
      "(through unported callers) wait on them; \"frees\" is how many would "
      "join the frontier if this one alone became ours.")
    w("")
    w("| Function | Unblocks | Frees | Lines | Seen | Ready | File |")
    w("|---|---|---|---|---|---|---|")
    ranked = sorted(unported, key=lambda f: (-m["unblocks"][f], -m["frees"][f], addr[f]))
    for f in ranked[:40]:
        w(f"| `{f}` | {m['unblocks'][f]} | {m['frees'][f]} | {m['lines'][f]} | "
          f"{'yes' if f in m['seen'] else 'no'} | {'yes' if f in m['frontier'] else 'no'} | "
          f"{m['table'].get(f, 'unmapped')} |")
    w("")

    fr = clusters(m, m["frontier"])
    w("## Frontier clusters, by what they unblock")
    w("")
    w(f"{len(fr)} clusters (frontier functions sharing an unported caller within "
      f"0x4000 bytes, neighbors in address order, or a call cycle). Each: what "
      f"it unblocks, its functions with their sizes, its function files.")
    w("")
    for i, c in enumerate(fr[:40], 1):
        fs = c["members"]
        files = ", ".join(c["files"]) if c["files"] else "unmapped"
        w(f"{i}. **{fs[0]}**-{fs[-1][4:]}: unblocks {c['unblocks']}; {len(fs)} functions, "
          f"{c['lines']} lines, {c['seen']} seen; {files}. {fmt_funcs(m, fs)}")
    w("")

    allc = clusters(m, unported)
    unmapped = [c for c in allc if not c["files"]]
    w("## Unmapped areas")
    w("")
    w(f"Clusters of unported reachable functions with no rows in any function "
      f"file: {len(unmapped)} ({sum(len(c['members']) for c in unmapped)} functions, "
      f"{sum(c['lines'] for c in unmapped)} lines). Largest first; the callers "
      f"named are outside the cluster.")
    w("")
    unmapped.sort(key=lambda c: (-len(c["members"]), addr[c["members"][0]]))
    for c in unmapped[:40]:
        fs = c["members"]
        mem = set(fs)
        outside_callers = sorted({g for f in fs for g in m["callers"][f] if g not in mem},
                                 key=lambda n: addr[n])
        named = []
        for g in outside_callers[:4]:
            named.append(f"`{g}` ({m['table'].get(g, 'unmapped')}, {m['state'][g]})")
        w(f"- **{fs[0]}**-{fs[-1][4:]}: {len(fs)} functions, {c['lines']} lines, "
          f"{c['seen']} seen, unblocks {c['unblocks']}; called from "
          + (", ".join(named) + (" ..." if len(outside_callers) > 4 else "")
             if named else "nothing outside it") + ".")
    if len(unmapped) > 40:
        rest = unmapped[40:]
        w(f"- ... and {len(rest)} smaller ones ({sum(len(c['members']) for c in rest)} "
          f"functions).")
    w("")

    w("## Extra roots")
    w("")
    w("Seen in play but not reachable from `main` (static edges miss them, or "
      "they run outside `main`: start-up, threads, callbacks), not under "
      "another extra root:")
    w("")
    tops = m["extra_top"]
    w(", ".join(f"`{f}` ({m['state'].get(f, '?')})" for f in tops[:80])
      + (f", ... {len(tops) - 80} more" if len(tops) > 80 else ""))
    w("")
    addr_ = addr
    w("## Originals no longer called")
    w("")
    w("Reachable through the original's graph but not through ours: the native "
      "function's C++ left the call out (plugged-in data, a dropped call, work "
      "merged into the C++). Grouped by that native function: its original's "
      "callees no longer reached, then how many functions hang only under them "
      "(a function under several groups counts in each; \"not ours\" leaves out "
      "those ours already, mostly Merged). Candidates for Dropped "
      "or \"Not called\" rows, for the user's review; the TSV's `lost` column "
      "lists them all.")
    w("")
    w("| Native caller | File | Status | Lost callees | Under them | Not ours | Lines | Seen |")
    w("|---|---|---|---|---|---|---|---|")
    for g in m["lost_groups"]:
        f = g["caller"]
        direct = ", ".join(f"`{c}`" for c in g["direct"][:8]) + \
            (f", ... {len(g['direct']) - 8} more" if len(g["direct"]) > 8 else "")
        w(f"| `{f}` | {m['table'].get(f, 'unmapped')} | {m['status'].get(f, '')} | "
          f"{direct} | {len(g['under'])} | {g['not_ours']} | {g['lines']} | {g['seen']} |")
    w("")
    w("## Ours functions whose original body is still reachable")
    w("")
    w("Ours by their rows, but something reaches their recompiled body: an "
      "`__imp__` fallback in our C++ (taken when the native C++ declines: a "
      "block not in our heap, a flag of the callee's off), a guest call to a "
      "function with no override and no C++, or an unported caller of one "
      "without an override. Each with its callers in that variant (native ones "
      "marked `n`):")
    w("")
    items = []
    for f in sorted(m["ours_orig"], key=lambda n: addr_[n]):
        cs = sorted(c + (" n" if v == "n" else "") for c, v in m["orig_callers"][f])
        items.append(f"`{f}` ({m['status'].get(f, '')}; from {', '.join(cs[:3])}"
                     + (" ..." if len(cs) > 3 else "") + ")")
    w(", ".join(items))
    w("")
    w("## Reached past the movie library's end (82575B08-8257FFFF)")
    w("")
    gap = m["gap"]
    gap_set = set(gap)
    entered = collections.defaultdict(set)
    for f in gap:
        for c in m["gap_callers"][f]:
            if c not in gap_set:
                entered[c].add(f)
    w(f"`b7-loop.md` ends the movie library's first range at 82575B07; "
      f"`movies.md` at 8257FFFF. Reachable there: {len(gap)} functions "
      f"({sum(1 for f in gap if st[f] == 'ours')} ours, "
      f"{sum(1 for f in gap if st[f] == 'unported')} unported, "
      f"{sum(1 for f in gap if st[f] == 'excluded')} reached only through the "
      f"replaced libraries). Entered from {len(entered)} functions outside it:")
    w("")
    for c in sorted(entered, key=lambda n: addr[n]):
        fs = sorted(entered[c], key=lambda n: addr[n])
        w(f"- `{c}` ({st.get(c, '?')}, {m['table'].get(c, 'unmapped')}): "
          + ", ".join(f"`{f}` ({st[f]})" for f in fs[:6])
          + (f", ... {len(fs) - 6} more" if len(fs) > 6 else "") + ".")
    w("")
    w("Each function there, with its callers: "
      + "; ".join(f"`{f}` ({st[f]}) from "
                  + ", ".join(f"`{c}`" for c in m["gap_callers"][f][:3])
                  + (" ..." if len(m["gap_callers"][f]) > 3 else "")
                  for f in gap) + ".")
    w("")
    if m["native_into_seeds"]:
        w("## Our C++ naming replaced-library functions")
        w("")
        w("Tables and vtables our C++ stores whose words land in the replaced "
          "libraries, and calls into `sub_822B8F10` (the HLSL compiler's entry, "
          "overridden by the stored shaders); not counted as the game's:")
        w("")
        w(", ".join(f"`{g}` -> `{c}`" for g, c in m["native_into_seeds"]))
        w("")
    if m["range_called"]:
        w("## Replaced-library functions with direct callers in the game's code")
        w("")
        w("In the excluded ranges but called directly by unported game code "
          "(\"checked against its callers first\"), so counted as the game's "
          "(ours or unported), with what they reach:")
        w("")
        w(", ".join(f"`{f}`" for f in m["range_called"][:60])
          + (f", ... {len(m['range_called']) - 60} more" if len(m['range_called']) > 60 else ""))
        w("")

    with open(path, "w") as f:
        f.write("\n".join(L))

    cluster_of = {}
    for i, c in enumerate(allc):
        for f in c["members"]:
            cluster_of[f] = i
    with open(tsv_path, "w") as out:
        out.write("function\taddress\tstate\treach\tseen\tfile\tlines\tunblocks\tfrees\t"
                  "frontier\tcluster\tbehind_ours\tlost\tfolded\timports\tcallees\torig_callees\n")
        for f in sorted(m["nodes"], key=lambda n: addr[n]):
            r = "main" if f in from_main else ("extra" if f in reachable else "no")
            out.write("\t".join([
                f, f"{addr[f]:08X}", m["state"].get(f, "unreachable"), r,
                "1" if f in m["seen"] else "0", m["table"].get(f, ""),
                str(m["lines"][f]), str(m["unblocks"].get(f, "")),
                str(m["frees"].get(f, "")), "1" if f in m["frontier"] else "0",
                str(cluster_of.get(f, "")), "1" if f in m["behind_ours"] else "0",
                "1" if f in m["lost"] else "0", ",".join(m["members"][f][1:]),
                ",".join(sorted(m["imports"][f])),
                ",".join(f"{c}:{KIND[k]}" for c, k in sorted(m["callees"][f].items())),
                ",".join(f"{c}:{KIND[k]}" for c, k in sorted(m["orig_callees"][f].items()))
                if m["callees"][f] is not m["orig_callees"][f] else "",
            ]) + "\n")


def main():
    args = sys.argv[1:]
    write = "--no-write" not in args
    image = IMAGE
    if "--image" in args:
        image = args[args.index("--image") + 1]
    m = build(image)
    reachable, st = m["reachable"], m["state"]
    print(f"reachable {len(reachable)} (from main {len(m['from_main'])}), "
          f"ours {sum(1 for f in reachable if st[f] == 'ours')}, "
          f"excluded {sum(1 for f in reachable if st[f] == 'excluded')}, "
          f"unported {len(m['unported'])} (seen {sum(1 for f in m['unported'] if f in m['seen'])}), "
          f"frontier {len(m['frontier'])}, folded {len(m['folded'])}")
    if not m["image"]:
        print(f"warning: {image} not found: no vtables or tables from data")
    if write:
        write_report(m, REPORT, TSV)
        print(f"wrote {os.path.relpath(REPORT, ROOT)} and {os.path.relpath(TSV, ROOT)}")


if __name__ == "__main__":
    main()
