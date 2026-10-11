#!/usr/bin/env python3
"""Read a state capture written by --capture_dir (src/debug/state_capture.cpp).

Usage: read_capture.py <capture_dir>   prints a summary of what was recorded.
Import it and use read_records() for analysis.
"""

import struct
import sys
from pathlib import Path

MAGIC = 0x49544331


def read_records(capture_dir):
    """Yields (tick, microseconds, {address: bytes}) for each logic tick."""
    data = (Path(capture_dir) / 'capture.bin').read_bytes()
    pos = 0
    while pos + 16 <= len(data):
        magic, tick, us, count = struct.unpack_from('<4I', data, pos)
        if magic != MAGIC:
            raise ValueError(f'bad record at offset {pos}')
        pos += 16
        regions = {}
        for _ in range(count):
            if pos + 8 > len(data):
                return  # truncated by the game exiting
            address, size = struct.unpack_from('<2I', data, pos)
            pos += 8
            if pos + size > len(data):
                return  # truncated by the game exiting
            regions[address] = data[pos:pos + size]
            pos += size
        yield tick, us, regions


def read_referrers(capture_dir):
    """Returns [(tick, vehicle, address)] from each pointer scan."""
    path = Path(capture_dir) / 'referrers.txt'
    if not path.exists():
        return []
    rows = []
    for line in path.read_text().split('\n'):
        if line:
            tick, vehicle, address = line.split()
            rows.append((int(tick), int(vehicle, 16), int(address, 16)))
    return rows


def be_float(buf, offset):
    return struct.unpack_from('>f', buf, offset)[0]


def main():
    capture_dir = sys.argv[1]
    ticks = 0
    vehicles = {}
    for tick, us, regions in read_records(capture_dir):
        ticks += 1
        for address, buf in regions.items():
            if len(buf) == 0x2000:
                speeds = vehicles.setdefault(address, [])
                speeds.append(be_float(buf, 48) * 3.6)
    print(f'{ticks} ticks')
    for address, speeds in vehicles.items():
        print(f'vehicle {address:08x}: {len(speeds)} ticks, '
              f'max {max(speeds):.0f} km/h')
    scans = {}
    for tick, vehicle, address in read_referrers(capture_dir):
        scans.setdefault(tick, 0)
        scans[tick] += 1
    for tick, count in scans.items():
        print(f'scan at tick {tick}: {count} referrers')


if __name__ == '__main__':
    main()
