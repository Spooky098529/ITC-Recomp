#!/usr/bin/env python3
"""Which draws put the rainbow effect's pixels on screen: reads a native
renderer dump (shown.pfm, draw_ids.pfm, draws.txt), takes the strongly
coloured pixels of the main view, and prints the draws covering most of
them, with their lines from draws.txt. Usage: rainbow_draws.py <dump dir>"""

import collections
import pathlib
import sys

import numpy as np


def read_pfm(path):
    data = path.read_bytes()
    header, rest = data.split(b"\n", 1)
    size, rest = rest.split(b"\n", 1)
    scale, rest = rest.split(b"\n", 1)
    width, height = map(int, size.split())
    pixels = np.frombuffer(rest, dtype="<f4", count=width * height * 3)
    return np.flipud(pixels.reshape(height, width, 3))


def main():
    folder = pathlib.Path(sys.argv[1])
    shown = read_pfm(folder / "shown.pfm")[:576, :1024]
    ids = read_pfm(folder / "draw_ids.pfm")[..., 0].astype(np.int64)
    draws = (folder / "draws.txt").read_text().splitlines()
    # Saturated and not dark: the rainbow effect's colours.
    high, low = shown.max(2), shown.min(2)
    rainbow = (high > 0.05) & ((high - low) > 0.6 * high)
    print(f"{rainbow.mean() * 100:.1f}% of pixels look like the rainbow effect")
    counts = collections.Counter(ids[rainbow].tolist())
    total = sum(counts.values())
    for draw, count in counts.most_common(8):
        line = draws[draw] if 0 <= draw < len(draws) else "(no draw)"
        print(f"{count * 100 / total:5.1f}%  {line[:700]}")


if __name__ == "__main__":
    main()
