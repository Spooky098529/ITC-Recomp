#!/usr/bin/env python3
"""Names the code in --watch_register_shadow and --watch_gpu_data_reads lines
(docs/conventional-renderer.md, step 0).

usage: resolve_watch_reads.py <build dir holding itc> <sdk lib dir> <log> [<log>...]

Each "render watch: <module>+0x<offset> <tag> +<offset> read|write count <n>
thread <name> phases ..." line's host offset goes through addr2line (itc, or
librexruntime.so); a line of generated/ is one PowerPC instruction, so the
guest address is the function's start plus 4 for each instruction comment
before it (counted from the last loc_ label). The last count logged for each
entry wins ("render watch: count" lines, every 60 s).
"""

import os
import re
import subprocess
import sys

LINE = re.compile(r"render watch: (\S+)\+0x([0-9A-F]+) (\S+) \+(\d+) (read|write) count (\d+) "
                  r"thread (.*?) phases(.*)$")
COUNT = re.compile(r"render watch: count (\S+)\+0x([0-9A-F]+) (\S+) \+(\d+) (read|write) (\d+)$")


def guest_address(path, line):
    """The guest function and instruction address at path:line."""
    try:
        with open(path) as f:
            lines = f.read().split("\n")
    except OSError:
        return None, None
    function = None
    base = None
    count = 0
    for i in range(line - 1, -1, -1):
        text = lines[i]
        m = re.match(r"DEFINE_REX_FUNC\(sub_([0-9A-F]+)\)", text)
        if m:
            function = "sub_" + m.group(1)
            if base is None:
                base = int(m.group(1), 16)
            break
        m = re.match(r"loc_([0-9A-F]+):", text)
        if m and base is None:
            base = int(m.group(1), 16)
            continue
        if base is None and re.match(r"\t// ", text):
            count += 1
    if base is None:
        return function, None
    return function, base + 4 * (count - 1)


def main():
    build, sdk = sys.argv[1], sys.argv[2]
    modules = {"itc": os.path.join(build, "itc"),
               "librexruntime.so": os.path.join(sdk, "librexruntime.so")}
    entries = {}
    for log in sys.argv[3:]:
        with open(log, errors="replace") as f:
            for text in f:
                text = text.rstrip("\n")
                m = LINE.search(text)
                if m:
                    module, offset, tag, off, kind, count, thread, phases = m.groups()
                    key = (module, offset, tag, int(off), kind)
                    entries[key] = [int(count), thread, phases.strip()]
                    continue
                m = COUNT.search(text)
                if m:
                    module, offset, tag, off, kind, count = m.groups()
                    key = (module, offset, tag, int(off), kind)
                    if key in entries:
                        entries[key][0] = int(count)
    names = {}
    for module, path in modules.items():
        offsets = sorted({k[1] for k in entries if k[0] == module})
        if not offsets:
            continue
        out = subprocess.run(["addr2line", "-f", "-C", "-e", path] + ["0x" + o for o in offsets],
                             capture_output=True, text=True).stdout.split("\n")
        for i, o in enumerate(offsets):
            symbol, where = out[2 * i], out[2 * i + 1]
            guest = ""
            m = re.match(r"(.*generated/.*\.cpp):(\d+)", where)
            if m:
                function, address = guest_address(m.group(1), int(m.group(2)))
                if address is not None:
                    guest = f" {function} @ {address:08X}"
            else:
                guest = f" ({os.path.basename(where)})"
            names[(module, o)] = f"{symbol}{guest}"
    for key in sorted(entries, key=lambda k: (k[2], names.get((k[0], k[1]), k[1]), k[3])):
        module, offset, tag, off, kind = key
        count, thread, phases = entries[key]
        name = names.get((module, offset), f"{module}+0x{offset}")
        print(f"{tag} +{off} {kind} x{count} [{thread}; {phases}]: {name}")


if __name__ == "__main__":
    main()
