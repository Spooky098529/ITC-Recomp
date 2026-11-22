#!/usr/bin/env python3
"""The game's functions under their names (docs/e5-native.md, step 0): reads
the rows of docs/functions/*.md and writes src/lifted/names.h, a name in
namespace `game` for every function of ours whose row gives one, referring
to the function itself (a lifted function's typed `lifted::f<address>`, a
hand port's entry `lifted::entry::e<address>`), so code calls the game's
functions by name. lift.py calls by these names (names_by_address()).

The names, from the row's Name cell (the user's call, 2026-11-16: names
marked "probably" are taken):
  - `A::B::C` becomes `game::A::B::C` (the parts as namespaces);
  - a constructor `X::X` becomes `X::Ctor`, a destructor `X::~X` `X::Dtor`;
  - `std` as a part becomes `Std` (not to be read as the standard library);
  - a part that's a C++ keyword gets a trailing `_`;
  - operators and anything else that isn't such a name keep `sub_X`;
  - a name given to several functions, or both a function and a namespace
    of others, gets `_<address>` added to each function's.
Rows without a name keep `sub_X` (no entry in names.h).

usage: function_names.py        (writes src/lifted/names.h)
"""
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
DOCS = os.path.join(ROOT, "docs", "functions")
LIFTED = os.path.join(ROOT, "itc_recomp", "src", "lifted")
HEADER = os.path.join(LIFTED, "names.h")

ROW = re.compile(r"^\|\s*`?(?:sub_)?(8[0-9A-Fa-f]{7})`?\s*\|\s*([^|]*)\|")
NAME = re.compile(r"^`([A-Za-z_~][A-Za-z0-9_]*(?:::[A-Za-z_~][A-Za-z0-9_]*)*)`")
KEYWORDS = set("""alignas alignof and and_eq asm auto bitand bitor bool break case catch
char char8_t char16_t char32_t class compl concept const consteval constexpr constinit
const_cast continue co_await co_return co_yield decltype default delete do double
dynamic_cast else enum explicit export extern false float for friend goto if inline int
long mutable namespace new noexcept not not_eq nullptr operator or or_eq private
protected public register reinterpret_cast requires return short signed sizeof static
static_assert static_cast struct switch template this thread_local throw true try typedef
typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq
game lifted base guest main""".split())


def row_names():
    """The first name each address's rows give (files in order)."""
    names = {}
    for path in sorted(glob.glob(os.path.join(DOCS, "*.md"))):
        with open(path) as f:
            for line in f:
                m = ROW.match(line)
                if not m:
                    continue
                address = m.group(1).upper()
                cell = m.group(2).strip()
                if address in names or not cell:
                    continue
                n = NAME.match(cell)
                names[address] = n.group(1) if n else None
    return names


def cpp_parts(name):
    parts = name.split("::")
    if any(p.startswith("~") for p in parts[:-1]):
        return None
    if len(parts) >= 2 and parts[-1] == parts[-2]:
        parts[-1] = "Ctor"
    elif parts[-1].startswith("~"):
        if len(parts) < 2 or parts[-1][1:] != parts[-2]:
            return None
        parts[-1] = "Dtor"
    if parts[0] == "operator" or any(p == "operator" or p.startswith("operator") for p in parts):
        return None
    parts = ["Std" if p == "std" else p for p in parts]
    return [p + "_" if p in KEYWORDS else p for p in parts]


def ours():
    """Our functions by address: 'lifted' (typed) or 'entry' (a hand port's)."""
    kinds = {}
    with open(os.path.join(LIFTED, "functions.h")) as f:
        for a in re.findall(r"^\S.* f(8[0-9A-F]{7})\(", f.read(), re.M):
            kinds[a] = "lifted"
    with open(os.path.join(LIFTED, "entries.h")) as f:
        for a in re.findall(r"Result e(8[0-9A-F]{7})\(", f.read()):
            kinds[a] = "entry"
    return kinds


def names_by_address():
    """address -> the C++ name (`game::A::B`), for our functions with one."""
    kinds = ours()
    wanted = {}
    for address, name in row_names().items():
        if address not in kinds or not name:
            continue
        parts = cpp_parts(name)
        if parts:
            wanted[address] = parts
    # Clashes: one name for several functions, or a function's name also
    # a namespace (a prefix of another's).
    by_name = {}
    for address, parts in wanted.items():
        by_name.setdefault(tuple(parts), []).append(address)
    prefixes = {tuple(p[:i]) for p in wanted.values() for i in range(1, len(p))}
    result = {}
    for address, parts in wanted.items():
        key = tuple(parts)
        if len(by_name[key]) > 1 or key in prefixes:
            parts = parts[:-1] + [f"{parts[-1]}_{address}"]
        result[address] = "game::" + "::".join(parts)
    return result


def main(argv):
    kinds = ours()
    names = names_by_address()
    groups = {}
    for address, full in names.items():
        parts = full.split("::")
        groups.setdefault("::".join(parts[:-1]), []).append((parts[-1], address))
    lines = [
        "// The game's functions by their names (docs/e5-native.md, step 0):",
        "// written by itc_recomp/tools/function_names.py from the rows of",
        "// docs/functions/. Don't edit it by hand: run the tool again. Each name",
        "// refers to the function: a lifted one's typed `lifted::f<address>`, a",
        "// hand port's entry `lifted::entry::e<address>`.",
        "",
        "#pragma once",
        "",
        '#include "lifted/entries.h"',
        '#include "lifted/functions.h"',
        "",
    ]
    for ns in sorted(groups):
        lines.append(f"namespace {ns} {{")
        for leaf, address in sorted(groups[ns]):
            target = f"lifted::f{address}" if kinds[address] == "lifted" else f"lifted::entry::e{address}"
            lines.append(f"inline constexpr auto& {leaf} = {target};  // sub_{address}")
        lines.append(f"}}  // namespace {ns}")
        lines.append("")
    with open(HEADER, "w") as f:
        f.write("\n".join(lines))
    lifted = sum(1 for a in names if kinds[a] == "lifted")
    print(f"names.h: {len(names)} names ({lifted} lifted, {len(names) - lifted} entries), "
          f"{len(kinds) - len(names)} of ours keep sub_X")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
