#!/usr/bin/env python3
"""B7's measure: the recompiled functions seen in play
(itc_recomp/coverage/seen_in_play.txt, from the coverage build: see
docs/coverage.md) joined with the function tables in docs/functions/ (read
with check_function_statuses.py's own code) and the overrides in
itc_recomp/src. Reports only; changes nothing.

usage: b7_status.py [repo root]   (default: this repo)

Each function in play is counted once, in the first state that fits:
  ours        an override in src/, or status Native or Dropped (an
              override's recompiled body no longer runs, so a function that
              is ours may be missing from the list: in play is the list
              plus what's ours)
  wrapped     status Wrapped: ours in front, the recompiled body does the work
  known       seen in play, with a row (named or described), still recompiled
  unknown     seen in play, no row yet
"wrapped", "known" and "unknown" are the work left. By table, a function
counts in the file its row is in; functions without a row count under
"(no row)".
Then the checks (listed, as there should be few): rows that say the
function no longer runs (Native, Dropped, or an override that never calls
__imp__, or "never called") whose function ran in play.
"""

import collections
import importlib.util
import os
import re
import sys

sys.dont_write_bytecode = True  # no __pycache__ from importing the status check

ROOT = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else
                       os.path.join(os.path.dirname(__file__), "..", ".."))
GENERATED = os.path.join(ROOT, "itc_recomp", "generated", "default")
SEEN = os.path.join(ROOT, "itc_recomp", "coverage", "seen_in_play.txt")
STATES = ["ours", "wrapped", "known", "unknown"]


def main():
    spec = importlib.util.spec_from_file_location(
        "statuses", os.path.join(os.path.dirname(__file__), "check_function_statuses.py"))
    statuses = importlib.util.module_from_spec(spec)
    saved, sys.argv = sys.argv, [sys.argv[0], ROOT]
    spec.loader.exec_module(statuses)
    sys.argv = saved
    overrides = statuses.overrides()

    defined = set()
    for name in os.listdir(GENERATED):
        if re.match(r"itc_recomp\.\d+\.cpp$", name):
            with open(os.path.join(GENERATED, name)) as f:
                defined.update(re.findall(r"^DEFINE_REX_FUNC\((sub_[0-9A-F]{8})\)", f.read(), re.M))
    with open(SEEN) as f:
        seen = {f"sub_{int(l, 16):08X}" for l in f if l.strip()}

    status, table, never, place = {}, {}, set(), {}
    for where, subs, st in statuses.rows():
        file, number = where.rsplit(":", 1)
        with open(os.path.join(statuses.DOCS, file)) as f:
            line = f.readlines()[int(number) - 1]
        for sub in subs:
            status.setdefault(sub, st)
            table.setdefault(sub, file)
            place.setdefault(sub, where)
            if "never called" in line.lower():
                never.add(sub)

    def ours(sub):
        return (sub in overrides and status.get(sub) != "Wrapped") or \
            status.get(sub) in ("Native", "Dropped")

    def state(sub):
        if status.get(sub) == "Wrapped":  # an override, but not ours yet
            return "wrapped"
        if ours(sub):
            return "ours"
        return "known" if sub in table else "unknown"

    in_play = (seen | {s for s in defined if ours(s)}) & defined
    totals = collections.Counter(state(s) for s in in_play)
    by_table = collections.defaultdict(collections.Counter)
    for s in in_play:
        by_table[table.get(s, "(no row)")][state(s)] += 1

    left = totals["wrapped"] + totals["known"] + totals["unknown"]
    print(f"recompiled sub_ functions: {len(defined)} (the register save and restore "
          f"helpers and xstart left out); seen in play: {len(seen & defined)}; "
          f"in play (seen, or ours): {len(in_play)}")
    print(f"  ours {totals['ours']}, work left {left} "
          f"(wrapped {totals['wrapped']}, known {totals['known']}, unknown {totals['unknown']})")
    rows_not_seen = sum(1 for s in table if s in defined and s not in in_play)
    print(f"rows: {len(table)}; for functions not seen in play: {rows_not_seen}")
    print()
    print(f"{'table':<24}" + "".join(f"{s:>10}" for s in STATES) + f"{'in play':>10}")
    for t in sorted(by_table, key=lambda t: (t == "(no row)", t)):
        c = by_table[t]
        print(f"{t:<24}" + "".join(f"{c[s]:>10}" for s in STATES) + f"{sum(c.values()):>10}")

    checks = []
    for s in sorted(seen & set(table)):
        st = status.get(s, "")
        if st in ("Native", "Dropped"):
            checks.append((s, f"{st}, but it ran"))
        elif s in overrides and not overrides[s][1] and st != "Wrapped":
            # Often a call to __imp__ from another file the status check
            # doesn't read (a fallback path the override hands work to).
            checks.append((s, f"overridden in {overrides[s][0]}, which never calls "
                              f"__imp__, but it ran"))
        elif s in never:
            checks.append((s, "its row says never called, but it ran"))
    print()
    print(f"checks: {len(checks)}")
    for s, why in checks:
        print(f"  check  {place[s]} {s}: {why}")


if __name__ == "__main__":
    main()
