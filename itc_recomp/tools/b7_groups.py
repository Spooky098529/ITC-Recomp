#!/usr/bin/env python3
"""B7's groups: which recompiled functions can be made native together
next (docs/loops/b7-loop.md, "Groups"). Reports only; changes nothing.

usage: b7_groups.py [--cap <lines>] [root ...]
  root   a function (sub_XXXXXXXX or its address): the groups in its subtree
         (what it calls, followed down, seen in play and not ours). With no
         root, the whole game: the count of ready groups and the blockers.
  --cap  the most generated lines a group should have (default 1500)

Native code is plain C++ without ctx, so it can only call what's already
ours: functions become native from the bottom of the call graph up. For a
function F, its group is F plus the callees only the group calls (seen in
play, not ours), followed down. Callees outside the group that run in play
and aren't ours are its blockers; with none, the group is ready.

  ours      an override in src/ (not Wrapped), or status Native or Dropped;
            the register save and restore helpers count as ours
  callers   only callers seen in play count (a caller that never runs
            doesn't need the callee)
  unseen    callees that never ran in play: native code replaces the whole
            function, so their branches need a decision (port them, or keep
            the recompiled path); listed, not counted as blockers
  indirect  calls through a register (vtables, callbacks): their targets
            aren't in the static graph; the survey reads which run
  imports   kernel imports the group calls directly (mostly the core's
            locks and memory); listed, not counted as blockers

Uses itc_recomp/coverage/seen_in_play.txt (docs/coverage.md), the statuses
in docs/functions/ (check_function_statuses.py's code) and the overrides in
itc_recomp/src. Direct calls only, as in the generated code.
"""

import collections
import importlib.util
import os
import re
import sys

sys.dont_write_bytecode = True  # no __pycache__ from importing the status check

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
GENERATED = os.path.join(ROOT, "itc_recomp", "generated", "default")
SEEN = os.path.join(ROOT, "itc_recomp", "coverage", "seen_in_play.txt")
HELPER = re.compile(r"__(save|rest)(gpr|fpr|vmx)")


def load():
    spec = importlib.util.spec_from_file_location(
        "statuses", os.path.join(os.path.dirname(__file__), "check_function_statuses.py"))
    statuses = importlib.util.module_from_spec(spec)
    saved, sys.argv = sys.argv, [sys.argv[0], ROOT]
    spec.loader.exec_module(statuses)
    sys.argv = saved
    overrides = statuses.overrides()
    status = {}
    for _, subs, st in statuses.rows():
        for sub in subs:
            status.setdefault(sub, st)

    calls, lines, indirect, imports = {}, {}, {}, {}
    define = re.compile(r"^DEFINE_REX_FUNC\((\w+)\)")
    ident = re.compile(r"\b([A-Za-z_]\w*)\(")
    imp = re.compile(r"__imp__([A-Za-z]\w+)")
    for name in sorted(os.listdir(GENERATED)):
        if not re.match(r"itc_recomp\.\d+\.cpp$", name):
            continue
        cur = None
        with open(os.path.join(GENERATED, name)) as f:
            for line in f:
                m = define.match(line)
                if m:
                    cur = m.group(1)
                    calls[cur], lines[cur], indirect[cur], imports[cur] = set(), 0, 0, set()
                    continue
                if cur is None or line.lstrip().startswith("//"):
                    continue
                lines[cur] += 1
                if "REX_CALL_INDIRECT_FUNC" in line:
                    indirect[cur] += 1
                for c in ident.findall(line):
                    if c != cur and (c.startswith("sub_") or HELPER.match(c) or c == "xstart"):
                        calls[cur].add(c)
                for m in imp.finditer(line):
                    if not m.group(1).startswith("sub_"):
                        imports[cur].add(m.group(1))
    for f in calls:
        calls[f] &= calls.keys()

    with open(SEEN) as f:
        seen = {f"sub_{int(l, 16):08X}" for l in f if l.strip()} & calls.keys()

    def ours(f):
        if HELPER.match(f):
            return True
        return (f in overrides and status.get(f) != "Wrapped") or \
            status.get(f) in ("Native", "Dropped")

    return calls, lines, indirect, imports, seen, ours


def main():
    args = sys.argv[1:]
    cap = 1500
    if "--cap" in args:
        i = args.index("--cap")
        cap = int(args[i + 1])
        del args[i:i + 2]
    roots = []
    for a in args:
        a = a.upper().removeprefix("SUB_").removeprefix("0X")
        roots.append(f"sub_{a}")

    calls, lines, indirect, imports, seen, ours = load()
    open_ = {f for f in seen if not ours(f)}  # what's left: ran, not ours
    callers = collections.defaultdict(set)
    for f in seen:
        for c in calls[f]:
            callers[c].add(f)

    group_of = {}

    def group(f):
        if f in group_of:
            return group_of[f]
        g = {f}
        changed = True
        while changed:
            changed = False
            for m in list(g):
                for c in calls[m]:
                    if c in open_ and c not in g and callers[c] <= g:
                        g.add(c)
                        changed = True
        outside = {c for m in g for c in calls[m] if c in open_ and c not in g}
        unseen = {c for m in g for c in calls[m]
                  if c not in seen and not ours(c) and c not in g}
        info = {
            "members": g,
            "blockers": outside,
            "unseen": unseen,
            "lines": sum(lines[m] for m in g),
            "indirect": sum(indirect[m] for m in g),
            "imports": set().union(*(imports[m] for m in g)),
        }
        group_of[f] = info
        return info

    def describe(f, info):
        flags = []
        if info["lines"] > cap:
            flags.append(f"over the {cap}-line cap")
        if info["indirect"]:
            flags.append(f"{info['indirect']} indirect calls")
        if info["unseen"]:
            flags.append(f"{len(info['unseen'])} unseen callees")
        if info["imports"]:
            flags.append("imports " + ", ".join(sorted(info["imports"])))
        others = sorted(info["members"] - {f})
        return (f"{f}: {len(info['members'])} functions, {info['lines']} lines"
                + (f" ({', '.join(others)})" if others else "")
                + (f"; {'; '.join(flags)}" if flags else ""))

    if not roots:
        ready = [f for f in open_ if not group(f)["blockers"]]
        blocking = collections.Counter(c for f in open_ for c in group(f)["blockers"])
        print(f"functions left (seen in play, not ours): {len(open_)}")
        print(f"ready groups (no blockers): {len(ready)}, "
              f"{sum(1 for f in ready if group(f)['lines'] <= cap)} within the cap")
        print("\nblockers holding the most groups back:")
        for c, n in blocking.most_common(15):
            print(f"  {c}: {n} groups wait on it; its own group: "
                  + describe(c, group(c)).split(": ", 1)[1])
        return

    for r in roots:
        if r not in calls:
            print(f"{r}: not a recompiled function")
            continue
        if r not in seen:
            print(f"{r}: never ran in play")
            continue
        if ours(r):
            print(f"{r}: already ours")
            continue
        subtree, stack = set(), [r]
        while stack:
            f = stack.pop()
            if f in subtree:
                continue
            subtree.add(f)
            stack.extend(c for c in calls[f] if c in open_ and c not in subtree)
        info = group(r)
        print(f"root {describe(r, info)}")
        print(f"  {'ready' if not info['blockers'] else 'blocked by ' + str(len(info['blockers']))}; "
              f"its subtree: {len(subtree)} functions left, "
              f"{sum(lines[f] for f in subtree)} lines")
        # The ready groups under it, largest first, not counting a group
        # inside another ready one.
        ready = [f for f in subtree if not group(f)["blockers"]]
        inside = set()
        for f in ready:
            inside |= group(f)["members"] - {f}
        top = sorted((f for f in ready if f not in inside),
                     key=lambda f: -group(f)["lines"])
        print(f"  ready groups under it: {len(top)}")
        for f in top[:20]:
            print(f"    {describe(f, group(f))}")
        if len(top) > 20:
            print(f"    ... and {len(top) - 20} more")
        blocking = collections.Counter(c for f in subtree for c in group(f)["blockers"])
        if blocking:
            print("  blockers under it, by groups waiting:")
            for c, n in blocking.most_common(10):
                print(f"    {c}: {n}")


if __name__ == "__main__":
    main()
