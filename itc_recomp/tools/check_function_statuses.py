#!/usr/bin/env python3
"""Checks the Status column of docs/functions.md against the code in
itc_recomp/src, so the statuses stay true on every branch (definitions at
the top of docs/functions.md). Reports only; changes nothing.

usage: check_function_statuses.py [repo root]   (default: this repo)

For each table row, every sub_XXXXXXXX in its first column is checked:
  error    Native, but an override (extern "C" REX_FUNC(sub_X), or a macro
           defining one) exists:
           it should be Overridden or Wrapped
  error    Overridden or Wrapped, but no override exists
  error    no status, but an override exists
  check    Wrapped, but the override never calls __imp__sub_X
  check    Dropped, but its file calls __imp__sub_X (a fallback flag
           while the drop is checked; remove it once checked)
  (Dropped with no override is fine: native callers leave the call out.)
  note     Overridden, and its file calls __imp__sub_X (a fallback flag or
           a check against the original is fine; calling it for the work
           would make it Wrapped)
Then lists overrides in src/ that no row's first column names.
Exits 1 if there are errors.
"""

import os
import re
import sys

ROOT = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else
                       os.path.join(os.path.dirname(__file__), "..", ".."))
DOC = os.path.join(ROOT, "docs", "functions.md")
SRC = os.path.join(ROOT, "itc_recomp", "src")

OVERRIDE = re.compile(r'extern\s+"C"\s+REX_FUNC\((sub_[0-9A-F]{8})\)')
# Overrides made by macros (ITC_INPUT_READ(8258CF10) and the like) are
# found too: see overrides().
SUB = re.compile(r"sub_[0-9A-F]{8}")


# A macro that defines overrides: #define NAME(address, ...) with
# REX_FUNC(sub_##address) in its body (continued lines included).
DEFINE = re.compile(r"^#define\s+(\w+)\(([^)]*)\)((?:.*\\\n)*.*)$", re.M)


def sources():
    for directory, _, files in os.walk(SRC):
        for name in files:
            if name.endswith((".cpp", ".h")):
                path = os.path.join(directory, name)
                with open(path, errors="replace") as f:
                    yield os.path.relpath(path, ROOT), f.read()


def overrides():
    """sub_X -> (file, whether it calls __imp__sub_X)."""
    found = {}
    macros = {}  # name -> whether its body calls __imp__sub_##
    files = list(sources())
    for path, text in files:
        for m in OVERRIDE.finditer(text):
            sub = m.group(1)
            found[sub] = (path, "__imp__" + sub in text)
        for m in DEFINE.finditer(text):
            body = m.group(3)
            if re.search(r"REX_FUNC\(\s*sub_\s*##", body):
                macros[m.group(1)] = bool(re.search(r"__imp__sub_\s*##", body))
    for path, text in files:
        for name, calls in macros.items():
            for m in re.finditer(r"^\s*" + name + r"\(\s*(?:0x)?([0-9A-Fa-f]{8})\b", text, re.M):
                found["sub_" + m.group(1).upper()] = (path, calls)
    return found


def rows():
    """(line number, [subs in the first column], status)."""
    with open(DOC) as f:
        for number, line in enumerate(f, 1):
            if not line.startswith("| `sub_"):
                continue
            cells = [c.strip() for c in line.strip().strip("|").split("|")]
            if len(cells) < 3:
                continue
            yield number, SUB.findall(cells[0]), cells[2]


def main():
    found = overrides()
    named = set()
    errors = checks = notes = 0
    for number, subs, status in rows():
        for sub in subs:
            named.add(sub)
            override = found.get(sub)
            where = f"functions.md:{number} {sub} ({status or 'no status'})"
            if status == "Native" and override:
                print(f"error  {where}: an override exists in {override[0]}")
                errors += 1
            elif status in ("Overridden", "Wrapped") and not override:
                print(f"error  {where}: no override in src/")
                errors += 1
            elif not status and override:
                print(f"error  {where}: an override exists in {override[0]}")
                errors += 1
            elif status == "Wrapped" and not override[1]:
                print(f"check  {where}: {override[0]} never calls __imp__{sub}")
                checks += 1
            elif status == "Dropped" and override and override[1]:
                print(f"check  {where}: {override[0]} calls __imp__{sub}")
                checks += 1
            elif status == "Overridden" and override[1]:
                print(f"note   {where}: {override[0]} calls __imp__{sub}")
                notes += 1
    unnamed = sorted(set(found) - named)
    for sub in unnamed:
        print(f"no row {sub}: overridden in {found[sub][0]}")
    print(f"{errors} errors, {checks} to check, {notes} notes, {len(unnamed)} overrides "
          f"without a row ({len(found)} overrides in src/)")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
