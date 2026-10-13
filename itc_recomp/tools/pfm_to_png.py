#!/usr/bin/env python3
"""Converts the native renderer's dumps (--native_renderer_dump=<dir>, PFM
files) into PNGs beside them. Each image is scaled so its 99.5th percentile
is white; single-channel images (depth) are stretched from their minimum to
their maximum. Usage: pfm_to_png.py <dir>"""

import pathlib
import sys

import numpy as np
from PIL import Image


def read_pfm(path):
    data = path.read_bytes()
    header, rest = data.split(b"\n", 1)
    size, rest = rest.split(b"\n", 1)
    scale, rest = rest.split(b"\n", 1)
    width, height = map(int, size.split())
    pixels = np.frombuffer(rest, dtype="<f4", count=width * height * 3)
    return np.flipud(pixels.reshape(height, width, 3))


def main():
    for path in sorted(pathlib.Path(sys.argv[1]).glob("*.pfm")):
        image = np.nan_to_num(read_pfm(path))
        gray = np.allclose(image[..., 0], image[..., 1]) and np.allclose(image[..., 0], image[..., 2])
        if gray:
            low, high = image.min(), image.max()
            image = (image - low) / max(high - low, 1e-9)
            note = f"range {low:.4f}..{high:.4f}"
        else:
            high = np.percentile(image, 99.5)
            image = image / max(high, 1e-9)
            note = f"99.5th percentile {high:.4f}, max {image.max() * high:.4f}"
        out = path.with_suffix(".png")
        Image.fromarray((np.clip(image, 0, 1) * 255 + 0.5).astype(np.uint8)).save(out)
        print(f"{out.name}: {note}")


if __name__ == "__main__":
    main()
