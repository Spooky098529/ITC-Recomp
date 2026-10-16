#!/usr/bin/env python3
"""Add symlinks to xwin's Windows SDK for headers included with another case.

Windows' filesystem ignores case, so code includes e.g. <ObjBase.h> while the
SDK ships objbase.h. xwin adds links for the spellings it knows; this adds
the ones the SDK and our code use that it missed.

Usage: xwin_case_links.py <xwin output dir> <source dir>...
"""

import os
import re
import sys

INCLUDE = re.compile(rb'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)
SOURCE_EXTS = ('.h', '.hpp', '.hh', '.inl', '.c', '.cc', '.cpp', '.cxx', '.m', '.rc')


def include_dirs(xwin):
    """Every directory under the CRT's and the SDK's include directories."""
    dirs = []
    for root, subdirs, _ in os.walk(xwin):
        if os.path.basename(root).lower() == 'include':
            for inner, _, _ in os.walk(root):
                dirs.append(inner)
            subdirs[:] = []
    return dirs


def wanted_includes(source_dirs):
    """Flat names included by the sources, less the sources' own headers."""
    names = set()
    own = set()
    for top in source_dirs:
        for root, _, files in os.walk(top):
            own.update(files)
            for name in files:
                if not name.endswith(SOURCE_EXTS):
                    continue
                try:
                    with open(os.path.join(root, name), 'rb') as f:
                        text = f.read()
                except OSError:
                    continue
                for m in INCLUDE.finditer(text):
                    names.add(m.group(1).decode('utf-8', 'replace').replace('\\', '/'))
    return {n for n in names if '/' not in n and n not in own}


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    dirs = include_dirs(sys.argv[1])
    # Lower-cased file name -> real file name, per include directory.
    listings = {d: {n.lower(): n for n in os.listdir(d)} for d in dirs}
    added = 0
    for include in sorted(wanted_includes(sys.argv[2:])):
        for d in dirs:
            if os.path.lexists(os.path.join(d, include)):
                break
        else:
            for d in dirs:
                real = listings[d].get(include.lower())
                if real:
                    os.symlink(real, os.path.join(d, include))
                    print(f'{d}/{include} -> {real}')
                    added += 1
                    break
    print(f'{added} links added')


if __name__ == '__main__':
    main()
