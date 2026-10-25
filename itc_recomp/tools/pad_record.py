#!/usr/bin/env python3
"""Records a gamepad's input for pad_play.py (see docs/pad-replay.md).

  itc_recomp/tools/pad_record.py <recording.jsonl> [--pad NAME]

Reads the pad's evdev events (the game still gets them: the pad isn't
grabbed) until Ctrl+C (or SIGTERM). Times start at the first input, so starting this
early doesn't matter. The recording is JSON lines: a header with the pad's
buttons and axis ranges, then one line per input report (the events between
two SYN_REPORTs): {"t": seconds, "ev": [[type, code, value], ...]}.
"""

import argparse
import json
import signal
import sys

import evdev
from evdev import ecodes

DEFAULT_PAD = "Generic X-Box pad"


def find_pad(name):
    for path in evdev.list_devices():
        device = evdev.InputDevice(path)
        if device.name == name:
            return device
    sys.exit(f"pad_record: no input device named \"{name}\"")


def stop(*_):
    raise KeyboardInterrupt


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("recording")
    parser.add_argument("--pad", default=DEFAULT_PAD)
    args = parser.parse_args()

    pad = find_pad(args.pad)
    # SIGTERM stops it too (background jobs ignore SIGINT).
    signal.signal(signal.SIGTERM, stop)
    caps = pad.capabilities(absinfo=True)
    header = {
        "format": "itc-pad-recording",
        "version": 1,
        "pad": pad.name,
        "keys": caps.get(ecodes.EV_KEY, []),
        "abs": {str(code): [info.min, info.max, info.fuzz, info.flat]
                for code, info in caps.get(ecodes.EV_ABS, [])},
    }
    print(f"pad_record: recording \"{pad.name}\" ({pad.path}) to {args.recording}; "
          "Ctrl+C to stop", file=sys.stderr)
    reports = 0
    with open(args.recording, "w") as out:
        out.write(json.dumps(header) + "\n")
        start = None
        report = []
        try:
            for event in pad.read_loop():
                if event.type == ecodes.EV_SYN:
                    if event.code == ecodes.SYN_REPORT and report:
                        t = event.timestamp()
                        if start is None:
                            start = t
                        out.write(json.dumps({"t": round(t - start, 4), "ev": report}) + "\n")
                        out.flush()
                        reports += 1
                        report = []
                    continue
                if event.type in (ecodes.EV_KEY, ecodes.EV_ABS):
                    report.append([event.type, event.code, event.value])
        except KeyboardInterrupt:
            pass
    print(f"pad_record: {reports} reports written", file=sys.stderr)


if __name__ == "__main__":
    main()
