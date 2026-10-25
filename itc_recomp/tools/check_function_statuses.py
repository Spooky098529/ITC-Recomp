#!/usr/bin/env python3
"""Checks the Status column of the function tables (every file in
docs/functions/) against the code in itc_recomp/src, so the statuses stay
true on every branch (definitions at the top of each file).
Reports only; changes nothing.

usage: check_function_statuses.py [repo root]   (default: this repo)

For each table row, every sub_XXXXXXXX in its first column is checked:
  error    Native or Merged, but an override (extern "C" REX_FUNC(sub_X),
           or a macro defining one) exists:
           it should be Overridden or Wrapped
  error    Overridden or Wrapped, but no override exists
  error    no status, but an override exists
  check    Wrapped, but nothing calls __imp__sub_X
  check    Dropped, but src/ calls __imp__sub_X (a fallback flag
           while the drop is checked; remove it once checked)
  (Dropped with no override is fine: native callers leave the call out.)
  ("src/ calls __imp__sub_X": any file outside src/harnesses/, written out
  or built by a macro; see overrides().)
  note     Overridden, and src/ calls __imp__sub_X (a fallback flag or
           a check against the original is fine; calling it for the work
           would make it Wrapped)
  error    the same sub_X in rows of two files (a row moved to another
           file but left behind too); rows naming several
           functions may repeat one within a file
Then lists overrides in src/ that no row's first column names.
Exits 1 if there are errors.
"""

import os
import re
import sys

ROOT = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else
                       os.path.join(os.path.dirname(__file__), "..", ".."))
DOCS = os.path.join(ROOT, "docs", "functions")
SRC = os.path.join(ROOT, "itc_recomp", "src")

OVERRIDE = re.compile(r'extern\s+"C"\s+REX_FUNC\((sub_[0-9A-F]{8})\)')
# Overrides made by macros (ITC_INPUT_READ(8258CF10) and the like) are
# found too: see overrides().
SUB = re.compile(r"sub_[0-9A-F]{8}")
IMP = re.compile(r"__imp__(sub_[0-9A-F]{8})")


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
    """sub_X -> (file, whether src/ calls __imp__sub_X).

    The second is true when any source file outside src/harnesses/ calls
    __imp__sub_X: written out, or through a macro whose body builds the
    name (__imp__sub_##address, as NATIVE_OR_ORIGINAL in
    hooks/ui_movies_game.cpp and WATCH_MOVIE_CALL in
    debug/pause_movie_log.cpp do), from the override's own file or from a
    helper in another one it hands the work to (ui_movies_game_hooks::Insert
    for sub_8211B4D0's override in debug/pause_movie_log.cpp). The harnesses'
    calls to the originals are checks, not fallbacks, and don't count.
    """
    found = {}
    macros = {}  # name -> whether its body defines an override
    imp_macros = {}  # name -> index of the parameter it builds __imp__sub_## from
    files = list(sources())
    for path, text in files:
        for m in OVERRIDE.finditer(text):
            found[m.group(1)] = path
        for m in DEFINE.finditer(text):
            body = m.group(3)
            if re.search(r"REX_FUNC\(\s*sub_\s*##", body):
                macros[m.group(1)] = True
            params = [p.strip() for p in m.group(2).split(",")]
            built = re.search(r"__imp__sub_\s*##\s*(\w+)", body)
            if built and built.group(1) in params:
                imp_macros[m.group(1)] = params.index(built.group(1))

    def arguments(name, text):
        """Macro `name`'s uses in `text`, its arguments in group 1."""
        return re.finditer(r"^\s*" + name + r"\(([^)]*)\)", text, re.M)

    calls = set()
    for path, text in files:
        if path.startswith(os.path.join("itc_recomp", "src", "harnesses") + os.sep):
            continue
        calls.update(m.group(1) for m in IMP.finditer(text))
        for name, index in imp_macros.items():
            for m in arguments(name, text):
                args = [a.strip() for a in m.group(1).split(",")]
                if index < len(args) and re.fullmatch(r"(?:0x)?[0-9A-Fa-f]{8}", args[index]):
                    calls.add("sub_" + args[index][-8:].upper())
    for path, text in files:
        for name in macros:
            for m in arguments(name, text):
                arg = m.group(1).split(",")[0].strip()
                if re.fullmatch(r"(?:0x)?[0-9A-Fa-f]{8}", arg):
                    found["sub_" + arg[-8:].upper()] = path
    return {sub: (path, sub in calls) for sub, path in found.items()}


def tables():
    """The files in docs/functions/, functions.md first."""
    names = sorted(n for n in os.listdir(DOCS) if n.endswith(".md"))
    return sorted(names, key=lambda n: n != "functions.md")


def rows():
    """(place, [subs in the first column], status); place is file:line."""
    for table in tables():
        with open(os.path.join(DOCS, table)) as f:
            for number, line in enumerate(f, 1):
                if not line.startswith("| `sub_"):
                    continue
                cells = [c.strip() for c in line.strip().strip("|").split("|")]
                if len(cells) < 3:
                    continue
                yield f"{table}:{number}", SUB.findall(cells[0]), cells[2]


def main():
    found = overrides()
    named = {}  # sub_X -> where its first row is
    errors = checks = notes = 0
    for place, subs, status in rows():
        for sub in subs:
            table = place.split(":")[0]
            first = named.setdefault(sub, place)
            if first.split(":")[0] != table:
                print(f"error  {place} {sub}: also has a row at {first}")
                errors += 1
                continue
            override = found.get(sub)
            where = f"{place} {sub} ({status or 'no status'})"
            if status in ("Native", "Merged") and override:
                print(f"error  {where}: an override exists in {override[0]}")
                errors += 1
            elif status in ("Overridden", "Wrapped") and not override:
                print(f"error  {where}: no override in src/")
                errors += 1
            elif not status and override:
                print(f"error  {where}: an override exists in {override[0]}")
                errors += 1
            elif status == "Wrapped" and not override[1]:
                print(f"check  {where}: nothing in src/ calls __imp__{sub} "
                      f"(overridden in {override[0]})")
                checks += 1
            elif status == "Dropped" and override and override[1]:
                print(f"check  {where}: src/ calls __imp__{sub} (overridden in {override[0]})")
                checks += 1
            elif status == "Overridden" and override[1]:
                print(f"note   {where}: src/ calls __imp__{sub} (overridden in {override[0]})")
                notes += 1
    unnamed = sorted(set(found) - set(named))
    for sub in unnamed:
        print(f"no row {sub}: overridden in {found[sub][0]}")
    print(f"{errors} errors, {checks} to check, {notes} notes, {len(unnamed)} overrides "
          f"without a row ({len(found)} overrides in src/)")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
