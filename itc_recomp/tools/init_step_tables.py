#!/usr/bin/env python3
"""State machine Inits as step tables: decodes generated Init functions
(a frame of functors built word by word and StateMachine::Add calls, as the
type 6 and 7 modes' and the options screens' Inits are) into the step tables
src/type67_modes/init_tables.h and src/options_screen/init_tables.h hold.
Reports only; prints the tables.

usage: init_step_tables.py <sub_XXXXXXXX>=<name> ...

It follows the registers through the generated code (constants, the object
r3's offsets, the frame's offsets), runs the six-word copy loops (ctr) word
by word, and prints, in the original's order:
  kStore [a] = b        a store of a known value (a constant, an object or
                        frame address)
  kCopy  [a] = [b]      a store of a frame word the original never wrote
                        (copied as the frame holds it)
  kAdd   StateMachine::Add(machine c, key a, functor part b)
  kBase  NetScreen::InitBase(r3 c, index a, r5 b) (sub_8227B3E0: the type
         12 mode's machines 6 to 13 call it first, for their base Init's
         transitions, run as a nested call with its own frame below)
Values: C(constant), O(offset from r3, the object), A(offset from r4, a
second object: the base Inits' functors' object) and F(offset in the frame).
A doubleword loaded from the frame (ld, as Type8PanelScreen::InitMachine
reloads each functor's two words) gives its high word through the rotate
that takes it and its low word where it is stored as a word.
Anything else in the function (another call, a load from outside the
frame, a branch other than the copy loops) stops it with an error: the
function isn't an Init of this shape. The back chain the prologue stores
isn't a step (the table's runner writes it).
"""

import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
GENERATED = os.path.join(ROOT, "itc_recomp", "generated", "default")
ADD = "sub_82106158"  # StateMachine::Add
BASE = "sub_8227B3E0"  # NetScreen::InitBase


def body(name):
    head = "DEFINE_REX_FUNC(%s)" % name
    for file in sorted(os.listdir(GENERATED)):
        if not file.endswith(".cpp"):
            continue
        with open(os.path.join(GENERATED, file)) as f:
            text = f.read()
        at = text.find(head + " {")
        if at < 0:
            continue
        end = text.find("\nDEFINE_REX_FUNC(", at + 1)
        lines = text[at:end if end >= 0 else len(text)].split("\n")[1:]
        return [l.strip() for l in lines if l.strip() and not l.strip().startswith("//")]
    sys.exit("%s: not found in generated/" % name)


def u32(v):
    return v & 0xFFFFFFFF


def add(value, n):
    kind, v = value
    if kind in ("C", "O", "A", "F"):
        return (kind, u32(v + n) if kind == "C" else v + n)
    raise ValueError("arithmetic on %r" % (value,))


def decode(name):
    lines = body(name)
    labels = {l[:-1]: i for i, l in enumerate(lines) if re.fullmatch(r"loc_[0-9A-F]+:", l)}
    regs = {"r1": ("SP", 0), "r3": ("O", 0), "r4": ("A", 0)}
    frame_words = {}
    steps = []
    frame = None
    saved = None
    pc = 0
    patterns = [
        (r"ctx\.(r\d+|ctr)\.[su]64 = (-?\d+);", "const"),
        (r"ctx\.(r\d+)\.[su]64 = ctx\.(r\d+)\.[su]64 \+ (-?\d+);", "add"),
        (r"ctx\.(r\d+|ctr)\.u64 = ctx\.(r\d+)\.u64;", "move"),
        (r"REX_STORE_U32\(ctx\.(r\d+)\.u32 \+ (-?\d+), ctx\.(r\d+)\.u32\);", "store"),
        (r"ctx\.(r\d+)\.u64 = REX_LOAD_U32\(ctx\.(r\d+)\.u32 \+ (-?\d+)\);", "load"),
        (r"ctx\.(r\d+)\.u64 = REX_LOAD_U64\(ctx\.(r\d+)\.u32 \+ (-?\d+)\);", "load64"),
        (r"ctx\.(r\d+)\.u64 = __builtin_rotateleft64\(ctx\.(r\d+)\.u64, 32\) & 0xFFFFFFFF;",
         "high"),
        (r"ea = -(\d+) \+ ctx\.r1\.u32;", "frame"),
        (r"__savegprlr_(\d+)\(ctx, base\);", "saved"),
        (r"if \(ctx\.ctr\.u32 != 0\) goto (loc_[0-9A-F]+);", "loop"),
        (r"(sub_[0-9A-F]+)\(ctx, base\);", "call"),
    ]
    skip = {"REX_FUNC_PROLOGUE();", "uint32_t ea{};", "ctx.r12.u64 = ctx.lr;",
            "REX_STORE_U32(ea, ctx.r1.u32);"}
    while pc < len(lines):
        line = lines[pc]
        pc += 1
        if line in skip or line.endswith(":") or line.startswith("ctx.lr = 0x"):
            continue
        if line == "ctx.r1.u32 = ea;":
            regs["r1"] = ("F", 0)
            continue
        if line == "--ctx.ctr.u64;":
            regs["ctr"] = ("C", u32(regs["ctr"][1] - 1))
            continue
        if line.startswith("ctx.r1.s64 = ctx.r1.s64 +") or line == "return;":
            break
        for pattern, kind in patterns:
            m = re.fullmatch(pattern, line)
            if m:
                break
        else:
            sys.exit("%s: can't follow %r" % (name, line))
        g = m.groups()
        if kind == "const":
            regs[g[0]] = ("C", u32(int(g[1])))
        elif kind == "add":
            regs[g[0]] = add(regs[g[1]], int(g[2]))
        elif kind == "move":
            regs[g[0]] = regs[g[1]]
        elif kind == "frame":
            frame = int(g[0])
        elif kind == "saved":
            saved = int(g[0])
        elif kind == "store":
            where = add(regs[g[0]], int(g[1]))
            value = regs[g[2]]
            if value[0] == "P":
                value = value[1][1]  # a doubleword's low word
            if where[0] not in ("F", "O", "A"):
                sys.exit("%s: a store to %r" % (name, where))
            if value[0] == "LF":
                steps.append(("kCopy", where, ("F", value[1])))
            elif value[0] in ("C", "O", "A", "F"):
                steps.append(("kStore", where, value))
            else:
                sys.exit("%s: a store of %r" % (name, value))
            if where[0] == "F":
                frame_words[where[1]] = value
        elif kind == "load":
            where = add(regs[g[1]], int(g[2]))
            if where[0] != "F":
                sys.exit("%s: a load from %r" % (name, where))
            regs[g[0]] = frame_words.get(where[1], ("LF", where[1]))
        elif kind == "load64":
            # A doubleword from the frame (ld): its high and low words.
            where = add(regs[g[1]], int(g[2]))
            if where[0] != "F":
                sys.exit("%s: a load from %r" % (name, where))
            words = tuple(frame_words.get(where[1] + k, ("LF", where[1] + k)) for k in (0, 4))
            regs[g[0]] = ("P", words)
        elif kind == "high":
            if regs[g[1]][0] != "P":
                sys.exit("%s: the high word of %r" % (name, regs[g[1]]))
            regs[g[0]] = regs[g[1]][1][0]
        elif kind == "loop":
            if regs["ctr"][1] != 0:
                pc = labels[g[0]]
        elif kind == "call":
            if g[0] == ADD:
                steps.append(("kAdd", regs["r4"], regs["r5"], regs["r3"]))
            elif g[0] == BASE:
                if regs["r4"][0] != "C":
                    sys.exit("%s: InitBase's index %r" % (name, regs["r4"]))
                steps.append(("kBase", regs["r4"], regs["r5"], regs["r3"]))
            else:
                sys.exit("%s: a call to %s" % (name, g[0]))
            for r in ["r0"] + ["r%d" % i for i in range(3, 13)]:
                regs[r] = ("X", 0)
    return frame, saved, steps


def value(v):
    kind, x = v
    if kind == "C":
        return "C(0x%08x)" % x
    return "%s(%d)" % (kind, x)


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    for arg in sys.argv[1:]:
        name, title = arg.split("=", 1)
        frame, saved, steps = decode(name)
        adds = sum(1 for s in steps if s[0] == "kAdd")
        print("// %s %s: a %d-byte frame, %d transitions (r%d to r31 saved)."
              % (name, title, frame, adds, saved))
        print("constexpr Step k%s[] = {" % name[4:])
        for s in steps:
            print("    {Op::%s, %s}," % (s[0], ", ".join(value(v) for v in s[1:])))
        print("};")
        print()


if __name__ == "__main__":
    main()
