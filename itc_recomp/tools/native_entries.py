#!/usr/bin/env python3
"""Turns hand ports' adapters into dispatch table entries (docs/rexglue-removal.md,
"Before E1: the rewiring", stage 3).

usage:
  native_entries.py <file> ...   rewrites the adapters in each file (src/)

An adapter of the usual shape, written out or inside a macro:

  extern "C" REX_FUNC(sub_X) {
    REX_FUNC_PROLOGUE();
    if (!ITC_FLAG(flag)) {
      __imp__sub_X(ctx, base);
      return;
    }
    BODY
  }

becomes `ITC_NATIVE_ENTRY(X, ITC_FLAG(flag)) { BODY }` (hooks/native_entry.h):
BODY then runs on our register view (lifted::Regs) as the entry, and the
compiler reports what it uses that the view doesn't have. In BODY, a call of
an SDK function on the context (`__imp__X(ctx, base)`) becomes
`lifted::Sdk(ctx, base, __imp__X)`. Adapters of other shapes are listed and
left as they are.
"""

import re
import sys

# Whitespace, including a macro's line continuations.
WS = r"(?:\s|\\\n)*"
HEAD = re.compile(
    r'extern "C" REX_FUNC\((sub_(?:##)?\w+)\)' + WS + r"\{" + WS
    + r"(?:REX_FUNC_PROLOGUE\(\);" + WS + r")?"
    + r"if" + WS + r"\(!(ITC_FLAG\(\w+\))\)" + WS + r"\{" + WS
    + r"__imp__(sub_(?:##)?\w+)\(ctx, base\);" + WS + r"return;" + WS + r"\}")
ANY = re.compile(r'extern "C" REX_FUNC\((sub_(?:##)?\w+)\)')
SDK_CALL = re.compile(r"\b(__imp__\w+)\(ctx, base\)")


def body_end(text, start):
    """The index just past the brace that closes the block open before `start`."""
    depth = 1
    i = start
    while depth:
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        elif c == '"':
            i = text.index('"', i + 1)
            while text[i - 1] == "\\":
                i = text.index('"', i + 1)
        elif text.startswith("//", i):
            i = text.index("\n", i) - 1
        i += 1
    return i


def convert(text):
    out = []
    pos = 0
    done = 0
    left = []
    for m in ANY.finditer(text):
        if m.start() < pos:
            continue
        h = HEAD.match(text, m.start())
        name = m.group(1)
        if not h or h.group(3) != name:
            left.append(name)
            continue
        address = name[len("sub_##"):] if name.startswith("sub_##") else name[len("sub_"):]
        end = body_end(text, h.end())
        body = SDK_CALL.sub(r"lifted::Sdk(ctx, base, \1)", text[h.end():end])
        body = re.sub(r"REX_FUNC_PROLOGUE\(\);[ \t]*(\\?)", r"\1", body)
        body = re.sub(r"^[ \t]*\\\n", " \\\n", body)
        out.append(text[pos:m.start()])
        out.append(f"ITC_NATIVE_ENTRY({address}, {h.group(2)}) {{")
        out.append(body)
        pos = end
        done += 1
    out.append(text[pos:])
    return "".join(out), done, left


def add_include(text):
    if '#include "hooks/native_entry.h"' in text:
        return text
    incs = list(re.finditer(r'^#include "[^"]+"$', text, re.M))
    at = incs[0].end() if incs else 0
    return text[:at] + '\n#include "hooks/native_entry.h"' + text[at:]


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 1
    total = 0
    for path in argv[1:]:
        with open(path) as f:
            text = f.read()
        new, done, left = convert(text)
        if done:
            new = add_include(new)
            with open(path, "w") as f:
                f.write(new)
        total += done
        print(f"{path}: {done} converted" + (f", {len(left)} left: {' '.join(left[:8])}" if left else ""))
    print(f"total {total}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
