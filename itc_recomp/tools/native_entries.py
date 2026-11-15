#!/usr/bin/env python3
"""Turns hand ports' adapters into dispatch table entries (docs/rexglue-removal.md,
"Before E1: the rewiring", stage 3).

usage:
  native_entries.py <file> ...   rewrites the adapters in each file (src/)
  native_entries.py sweep        the same for every file of src/ but those
                                 holding the SDK's context on purpose, and
                                 our register view in place of the context
                                 in every function taking it (below)

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
left as they are, or with `sweep` become `ITC_NATIVE_ENTRY_ALWAYS(X) { BODY }`
(their checks in BODY).

`sweep` also makes every function of ours taking the SDK's context take our
register view: a step, `void Name(PPCContext& ctx, uint8_t* base)`, becomes
`ITC_REGS_STEP(Name)` (both forms: the harnesses still pass a context), its
declarations both; any other `PPCContext&` becomes `lifted::Regs&`.
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


def convert(text, any_shape=False):
    out = []
    pos = 0
    done = 0
    left = []
    for m in ANY.finditer(text):
        if m.start() < pos:
            continue
        h = HEAD.match(text, m.start())
        name = m.group(1)
        address = name[len("sub_##"):] if name.startswith("sub_##") else name[len("sub_"):]
        if not h or h.group(3) != name:
            if not any_shape:
                left.append(name)
                continue
            open_brace = re.compile(WS + r"\{").match(text, m.end())
            if not open_brace:
                left.append(name)
                continue
            start = open_brace.end()
            end = body_end(text, start)
            body = SDK_CALL.sub(r"lifted::Sdk(ctx, base, \1)", text[start:end])
            body = re.sub(r"REX_FUNC_PROLOGUE\(\);[ \t]*(\\?)", r"\1", body)
            out.append(text[pos:m.start()])
            out.append(f"ITC_NATIVE_ENTRY_ALWAYS({address}) {{")
            out.append(body)
            pos = end
            done += 1
            continue
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


# Files that hold the SDK's context on purpose: the lift code's boundary, our
# start-up, the harnesses, native audio's thread, and the helpers below.
KEEP = ("lifted/", "app/", "harnesses/", "audio/native_audio.cpp", "hooks/native_entry.h",
        "hooks/guest_call.h", "hooks/lifted_call.h")
STEP_DEF = re.compile(r"\bvoid (\w+)\(PPCContext& ctx, uint8_t\* base\)(" + WS + r")\{")
STEP_DECL = re.compile(r"^([ \t]*)((?:static |inline )?)void (\w+)\(PPCContext& ctx, uint8_t\* base\);",
                       re.M)


def to_regs(text):
    """Our register view in place of the context in the functions taking it."""
    text = STEP_DEF.sub(lambda m: f"ITC_REGS_STEP({m.group(1)}){m.group(2)}{{", text)
    text = STEP_DECL.sub(
        lambda m: (f"{m.group(1)}{m.group(2)}void {m.group(3)}(PPCContext& ctx, uint8_t* base);\n"
                   f"{m.group(1)}{m.group(2)}void {m.group(3)}(lifted::Regs& ctx, uint8_t* base);"),
        text)
    # Any other function, class member or lambda taking or keeping the
    # context (the steps' declarations above keep theirs).
    lines = text.split("\n")
    for i, line in enumerate(lines):
        if "PPCContext& ctx, uint8_t* base);" in line and line.lstrip().startswith(("void ", "static void ", "inline void ")):
            continue
        lines[i] = re.sub(r"\bPPCContext\s*&", "lifted::Regs&", line)
    return "\n".join(lines)


def sweep():
    import pathlib
    root = pathlib.Path(__file__).resolve().parents[1] / "src"
    total = 0
    for path in sorted(root.rglob("*")):
        if path.suffix not in (".cpp", ".h"):
            continue
        rel = path.relative_to(root).as_posix()
        if rel.startswith(KEEP[:3]) or rel in KEEP[3:]:
            continue
        text = path.read_text()
        if "PPCContext" not in text and "REX_FUNC(" not in text:
            continue
        new, done, left = convert(text, any_shape=True)
        new = to_regs(new)
        if new != text:
            if done or "lifted::Regs" in new:
                new = add_include(new)
            path.write_text(new)
        total += done
        if left:
            print(f"{rel}: left {' '.join(left)}")
    print(f"total {total} adapters")
    return 0


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 1
    if argv[1] == "sweep":
        return sweep()
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
