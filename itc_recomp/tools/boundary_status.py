#!/usr/bin/env python3
"""Measures the SDK boundary: every recompiled function that calls a kernel
import directly (the pipeline under "The boundary" in
docs/rexglue-removal.md), and where each stands, from the code in
itc_recomp/src and the Status column of docs/functions/ (read with
check_function_statuses.py's own code). Reports only; changes nothing.

usage: boundary_status.py [repo root]   (default: this repo)

Each boundary function is counted once, in the first state that fits:
  overridden    an override in src/ (REX_FUNC, or a macro defining one)
  native        status Native (no override: its callers are native)
  dropped       status Dropped (no override: its callers leave it out)
  never called  its row says "Never called" (in play)
  live          still called by recompiled code
By area, a function calling imports of several areas counts in each.
Areas come from the import's name (AREAS below; the first match wins).
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
DOCS = os.path.join(ROOT, "docs", "functions")

# Import name patterns by plan item (first match wins).
AREAS = [
    ("input (B1)", r"XamInput"),
    ("audio (B2)", r"XAudio|XMA"),
    ("graphics (B3)", r"Vd|XGetVideoMode"),
    ("files and saves (B4)",
     r"Nt\w*File|NtQueryVolume|NtQueryDirectory|NtFlush|Ob\w*SymbolicLink|XamContent"
     r"|XamShowDeviceSelector|XamEnumerate|IoDismount|NtDeviceIoControl"),
    ("memory (core)",
     r"NtAllocateVirtual|NtFreeVirtual|NtQueryVirtual|NtProtect|Mm|XamAlloc|XamFree"
     r"|ExAllocate|ExFree"),
    ("system services (B5)",
     r"Xam|XMsg|XNotify|NetDll|Xex|ExGetXConfig|XGetLanguage|XGetAVPack|Hal|RtlImageXex"),
    ("pure helpers (B6)",
     r"RtlInit|RtlUnicode|RtlMultiByte|RtlFree|RtlCompareMemory|RtlFill|RtlTime"
     r"|RtlNtStatus|sprintf|vswprintf|DbgPrint|KeQuerySystemTime|KeQueryPerformance"
     r"|XGet|KeGetCurrentProcessType"),
    ("threads, sync and the rest (core)", r""),
]
STATES = ["overridden", "native", "dropped", "never called", "live"]


def boundary():
    """{function: set of imports}, as the pipeline under "The boundary"."""
    calls = collections.defaultdict(set)
    function = None
    define = re.compile(r"DEFINE_REX_FUNC\(([A-Za-z_][A-Za-z0-9_]*)")
    imp = re.compile(r"__imp__([A-Za-z][A-Za-z0-9_]+)")
    for name in sorted(os.listdir(GENERATED)):
        if not (name.startswith("itc_recomp.") and name.endswith(".cpp")):
            continue
        with open(os.path.join(GENERATED, name)) as f:
            for line in f:
                m = define.search(line)
                if m:
                    function = m.group(1)
                    continue
                m = imp.search(line)
                if m and not m.group(1).startswith("sub_") and function:
                    calls[function].add(m.group(1))
    return calls


def area(name):
    for label, pattern in AREAS:
        if re.match(pattern, name):
            return label
    return AREAS[-1][0]


def main():
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
    never = set()
    row = re.compile(r"\| `(sub_[0-9A-F]{8})`")
    for name in os.listdir(DOCS):
        with open(os.path.join(DOCS, name)) as f:
            for line in f:
                m = row.match(line)
                if m and "never called" in line.lower():
                    never.add(m.group(1))

    def state(function):
        if function in overrides:
            return "overridden"
        if status.get(function) in ("Native", "Merged"):
            return "native"
        if status.get(function) == "Dropped":
            return "dropped"
        if function in never:
            return "never called"
        return "live"

    calls = boundary()
    totals = collections.Counter()
    by_area = collections.defaultdict(collections.Counter)
    live_imports = set()
    for function, imports in calls.items():
        s = state(function)
        totals[s] += 1
        for a in {area(i) for i in imports}:
            by_area[a][s] += 1
        if s == "live":
            live_imports |= imports
    all_imports = set().union(*calls.values())

    print(f"boundary: {len(calls)} functions, "
          f"{sum(len(i) for i in calls.values())} function-import pairs")
    print("  " + ", ".join(f"{s} {totals[s]}" for s in STATES))
    print(f"imports: {len(all_imports)} called, {len(live_imports)} with a live caller")
    print()
    width = max(len(a) for a, _ in AREAS)
    print(f"{'area':{width}}  " + "  ".join(f"{s:>12}" for s in STATES))
    for label, _ in AREAS:
        if label in by_area:
            print(f"{label:{width}}  " +
                  "  ".join(f"{by_area[label][s]:>12}" for s in STATES))


if __name__ == "__main__":
    main()
