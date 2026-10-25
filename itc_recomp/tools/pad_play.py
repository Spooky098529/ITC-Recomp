#!/usr/bin/env python3
"""Replays a pad_record.py recording through a virtual pad (uinput); see
docs/pad-replay.md.

  itc_recomp/tools/pad_play.py <recording.jsonl> [--start-delay S]
                               [--keep | --exit-after S]

Makes the virtual pad, waits --start-delay seconds (1 by default: the game
picks a new pad up on SDL's hot-plug event), then plays the reports at their
recorded times from then on. At the end it releases everything and, after
--exit-after seconds (0 by default), goes away (unplugging it mid-game may
pause the game); with --keep it stays until the script is stopped.

The virtual pad has an Xbox 360 pad's USB ids (045E:028E), not the real
pad's, so the game can be told to ignore the real one:
SDL_GAMECONTROLLER_IGNORE_DEVICES=<real pad's ids> puts the virtual pad
alone in slot 0. SDL also drops pad events while no SDL window has focus
unless SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS=1. scripted_run.sh sets both.
"""

import argparse
import json
import signal
import sys
import time

import evdev
from evdev import AbsInfo, ecodes


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("recording")
    parser.add_argument("--start-delay", type=float, default=1.0)
    parser.add_argument("--keep", action="store_true")
    parser.add_argument("--exit-after", type=float, default=0.0)
    args = parser.parse_args()

    with open(args.recording) as f:
        header = json.loads(f.readline())
        if header.get("format") != "itc-pad-recording":
            sys.exit(f"pad_play: {args.recording} isn't a pad recording")
        reports = [json.loads(line) for line in f if line.strip()]

    caps = {
        ecodes.EV_KEY: header["keys"],
        ecodes.EV_ABS: [(int(code), AbsInfo(0, lo, hi, fuzz, flat, 0))
                        for code, (lo, hi, fuzz, flat) in header["abs"].items()],
    }
    pad = evdev.UInput(caps, name="ITC replay pad", vendor=0x045E, product=0x028E,
                       version=0x0114, bustype=ecodes.BUS_USB)
    # SIGTERM (from scripted_run.sh) closes the pad cleanly.
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(0))
    try:
        print(f"pad_play: virtual pad {pad.device.path}; {len(reports)} reports, "
              f"{reports[-1]['t'] if reports else 0:.1f} s", flush=True)
        time.sleep(args.start_delay)
        start = time.monotonic()
        for report in reports:
            wait = start + report["t"] - time.monotonic()
            if wait > 0:
                time.sleep(wait)
            for type_, code, value in report["ev"]:
                pad.write(type_, code, value)
            pad.syn()
        # Let go of everything: buttons up, sticks and triggers at rest.
        for code in header["keys"]:
            pad.write(ecodes.EV_KEY, code, 0)
        for code, (lo, hi, _, _) in header["abs"].items():
            pad.write(ecodes.EV_ABS, int(code), 0 if lo < 0 else lo)
        pad.syn()
        print(f"pad_play: done after {time.monotonic() - start:.1f} s", flush=True)
        if args.keep:
            signal.pause()
        time.sleep(args.exit_after)
    finally:
        pad.close()


if __name__ == "__main__":
    main()
