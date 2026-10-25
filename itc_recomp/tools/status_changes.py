#!/usr/bin/env python3
"""The functions whose Status changed in docs/functions/ since a commit, as a
Markdown list (for a B7 loop run's list, docs/loops/b7-loop.md). Reports
only; changes nothing.

usage: status_changes.py <commit> [<commit>]
  The statuses at the first commit against the second (default: the
  working tree). A function counts when its Status differs, including a new
  row with a status; a row only moved between files, or added with a blank
  status, doesn't.
"""

import os
import re
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
DOCS = "docs/functions"
SUB = re.compile(r"sub_[0-9A-F]{8}")


def git(*args):
    return subprocess.run(["git", "-C", ROOT, *args], check=True,
                          capture_output=True, text=True).stdout


def files(commit):
    """{name: text} of the function files at a commit (None: the tree)."""
    if commit is None:
        path = os.path.join(ROOT, DOCS)
        return {n: open(os.path.join(path, n)).read()
                for n in sorted(os.listdir(path)) if n.endswith(".md")}
    names = git("ls-tree", "--name-only", f"{commit}:{DOCS}").split()
    return {n: git("show", f"{commit}:{DOCS}/{n}") for n in names if n.endswith(".md")}


def statuses(commit):
    """{sub: (name, status, file)} from each function's first row."""
    found = {}
    for table, text in files(commit).items():
        for line in text.splitlines():
            if not line.startswith("| `sub_"):
                continue
            cells = [c.strip() for c in line.strip().strip("|").split("|")]
            if len(cells) < 3:
                continue
            for sub in SUB.findall(cells[0]):
                found.setdefault(sub, (cells[1].replace("`", ""), cells[2], table))
    return found


def main():
    if len(sys.argv) not in (2, 3):
        print(__doc__.strip())
        sys.exit(2)
    before = statuses(sys.argv[1])
    after = statuses(sys.argv[2] if len(sys.argv) == 3 else None)
    changes = []
    for sub, (name, status, table) in after.items():
        old = before.get(sub, ("", "", ""))[1]
        if status != old:
            changes.append((table, sub, name, old or "blank", status or "blank"))
    for table in sorted({c[0] for c in changes}):
        print(f"\n## {table}\n")
        for _, sub, name, old, new in sorted(c for c in changes if c[0] == table):
            print(f"- `{sub}`{' ' + name if name else ''}: {old} -> {new}")
    print(f"\n{len(changes)} functions changed status.")


if __name__ == "__main__":
    main()
