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
        "hooks/guest_call.h", "hooks/lifted_call.h", "guest.h")
STEP_DEF = re.compile(r"\bvoid (\w+)\(PPCContext& ctx, uint8_t\* base\)(" + WS + r")\{")
STEP_DECL = re.compile(r"^([ \t]*)((?:static |inline )?)void (\w+)\(PPCContext& ctx, uint8_t\* base\);",
                       re.M)


def step_names(text):
    """The steps a file defines or declares (both forms once swept)."""
    names = set(m.group(1) for m in STEP_DEF.finditer(text))
    names |= set(m.group(3) for m in STEP_DECL.finditer(text))
    names |= set(re.findall(r"ITC_REGS_STEP\((\w+)\)", text))
    return names - {"name", "step"}


def value_uses(text, names, wrapper):
    """A step (`hooks::Name`, or `Name` inside namespace hooks) or an original
    (`lifted::original::cX`) named as a value (not called, declared or
    already wrapped) wrapped in `wrapper`, which picks the form."""
    if not wrapper:
        return text
    # An unqualified name an all-caps macro takes (pasted into a call there)
    # stays bare.
    macro_arg = re.compile(r"(\b[A-Z][A-Z0-9_]+\([^()\n]*?)ITC_(?:REGS|CONTEXT)_FN\((\w+)\)")
    while macro_arg.search(text):
        text = macro_arg.sub(r"\1\2", text)
    text = re.sub(r"ITC_(?:REGS|CONTEXT)_FN\(((?:hooks::)?\w+)\)(?=\s*[A-Za-z_&*])", r"\1", text)
    text = re.sub(r"\b(typename|class|struct) ITC_(?:REGS|CONTEXT)_FN\((\w+)\)", r"\1 \2", text)
    out = []
    pos = 0
    pattern = re.compile(r"(hooks::|lifted::original::)?\b([A-Za-z_]\w*)\b")
    for m in pattern.finditer(text):
        qual, name = m.group(1), m.group(2)
        if qual == "lifted::original::":
            if not re.fullmatch(r"c[0-9A-F]{8}", name):
                continue
        elif name not in names:
            continue
        before = text[max(0, m.start() - 24):m.start()]
        after = text[m.end():m.end() + 40].lstrip()
        if re.match(r"[A-Za-z_&*]", after):
            continue  # a declaration's type, not a value
        line_start = text.rfind("\n", 0, m.start()) + 1
        line = text[line_start:m.start()]
        if (after.startswith(("(", "<")) or before.endswith(("_FN(", ".", "->", "::", "void ", "&", "typename ", "class ", "struct "))
                or line.count('"') % 2 == 1 or line.lstrip().startswith(("//", "#", "ITC_REGS_STEP"))
                or re.search(r"\b(void|bool|uint\w+|auto)\s*$", before)
                or (not qual and re.search(r"\b[A-Z][A-Z0-9_]+\([^()]*$", line))):
            continue
        out.append(text[pos:m.start()])
        out.append(f"{wrapper}({m.group(0)})")
        pos = m.end()
    out.append(text[pos:])
    return "".join(out)


def to_regs(text, names=frozenset()):
    """Our register view in place of the context in the functions taking it."""
    text = STEP_DEF.sub(lambda m: f"ITC_REGS_STEP({m.group(1)}){m.group(2)}{{", text)
    def decl(m):
        regs = f"{m.group(1)}{m.group(2)}void {m.group(3)}(lifted::Regs& ctx, uint8_t* base);"
        after = text[m.end():m.end() + len(regs) + 2]
        if regs.strip() in after:
            return m.group(0)
        return f"{m.group(1)}{m.group(2)}void {m.group(3)}(PPCContext& ctx, uint8_t* base);\n" + regs
    text = STEP_DECL.sub(decl, text)
    # SDK functions and our overrides called on the view.
    text = SDK_CALL.sub(r"lifted::Sdk(ctx, base, \1)", text)
    text = re.sub(r"(?<![\w:])sub_([0-9A-F]{8})\(ctx, base\)", r"lifted::CallFromContext(ctx, base, 0x\1)", text)
    text = re.sub(r"\bPPCRegister\b", "lifted::Temp", text)
    # A helper's fallback to the recompiled original (`original`): an SDK
    # function, called on the view through lifted::Sdk.
    text = re.sub(r"void \(\*original\)\(lifted::Regs&, uint8_t\*\)", "lifted::ImportFunction* original", text)
    text = re.sub(r"using Original = void \(\*\)\(lifted::Regs&, uint8_t\*\);",
                  "using Original = lifted::ImportFunction*;", text)
    text = re.sub(r"(?<![\w:])original\(ctx, base\)", "lifted::Sdk(ctx, base, original)", text)
    text = value_uses(text, names, "ITC_REGS_FN")
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
    paths = sorted(p for p in root.rglob("*") if p.suffix in (".cpp", ".h"))
    names = set()
    for path in paths:
        names |= step_names(path.read_text())
    for path in paths:
        rel = path.relative_to(root).as_posix()
        if rel.startswith("harnesses/"):
            # The harnesses keep the context: a step named as a value is
            # its form on the context.
            text = path.read_text()
            new = value_uses(text, names, "ITC_CONTEXT_FN")
            if new != text:
                path.write_text(new)
            continue
        if rel.startswith(KEEP[:3]) or rel in KEEP[3:]:
            continue
        text = path.read_text()
        if "PPCContext" not in text and "REX_FUNC(" not in text and "lifted::Regs" not in text:
            continue
        new, done, left = convert(text, any_shape=True)
        new = to_regs(new, names)
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
