# Pad replay

A recorded gamepad played back into the game through a virtual pad, so a
test run can go where attract races don't: the menus, a race the player
drives, saves. The game is unchanged; it's three scripts in
`itc_recomp/tools/` and the recordings in `itc_recomp/pad_recordings/`.

## Recording

Start the game with `--skip_intro`, as the replay does, then the recorder
from the repo root before pressing anything:

```sh
itc_recomp/tools/pad_record.py itc_recomp/pad_recordings/<name>.jsonl
```

It reads the pad's evdev events by name ("Generic X-Box pad" by default,
`--pad NAME` for another) without grabbing it, so the game gets them too,
until Ctrl+C or SIGTERM. Times start at the first input. At the title,
wait a couple of seconds before the first press, as the replay starts about
4 s after the boot's first draw. Starting the attract race's skip and the
game both take Start.

A recording is JSON lines: a header (the format, the pad's name, its
buttons and its axes' ranges), then one line per input report, the events
between two SYN_REPORTs: `{"t": seconds, "ev": [[type, code, value], ...]}`.
A two-minute run is about 240 KB.

## Replaying

```sh
itc_recomp/tools/scripted_run.sh <recording> [extra flags]
```

It starts the game with the standard flags, `--log_level=info` and
`--skip_intro`, waits for the boot's first draw (its first resolve, "Resolve (822AFA58)
called from 8258D9E8"; "called from 8248466C", the movie library's draw,
until the movies moved to our FFmpeg on 2026-10-25, in
the log; if it doesn't come in 30 s, the boot stalled and the game is
restarted once), waits `PAD_DELAY` seconds (3), and runs `pad_play.py`.
The game is closed 5 s after the last input (or, if it hangs, the
recording's length plus 2 minutes after launch). Then it prints the log's
path, how the run ended, the seconds at 30 ticks/s, error lines and harness
results, as `attract_race.sh` does. `--log_race_changes` shows the races
entered and left (the title screen's backdrop counts as one, a few seconds
long); `--log_gamepads` the buttons the game read.

`pad_play.py <recording>` alone makes the virtual pad, waits 1 s (the game
takes a new pad on SDL's hot-plug event), plays the reports at their
recorded times, then lets go of everything (`--keep` keeps the pad until
stopped, `--exit-after S` waits before closing it).

## How the virtual pad becomes the game's

The game reads all four slots once a tick (`src/input/`): pads take the
lowest free slot in the order SDL sees them. So the virtual pad isn't a
copy of the real pad's identity: it has a wired Xbox 360 pad's USB ids
(045E:028E, which SDL maps as an Xbox controller), and `scripted_run.sh`
runs the game with:

- `SDL_GAMECONTROLLER_IGNORE_DEVICES=0x2E24/0x072F` (the user's pad;
  `REAL_PAD` to change it): SDL never sees the real pad, which can stay
  plugged in, and the virtual pad is alone in slot 0 (the log:
  `gamepads: "Xbox 360 Controller" in slot 0 (... vendor 0x045E, product 0x028E)`).
- `SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS=1`: SDL drops pad events while no
  SDL window has keyboard focus, which a game started by a script may not
  have.

`/dev/uinput` must be writable by the user (an ACL here); the pad's event
node gets the user's ACL from udev.

## Limits

- Replay is by time from the replay's start: driving won't repeat exactly
  (loads and the race start vary), and a menu input can land on a load that
  ran longer. That's fine for reaching the menus and a race and getting
  functions to run; it doesn't reproduce a drive.
- The virtual pad is seen system-wide while a replay runs.
- `--native_renderer_dump` slows the frames it writes, which threw a replay
  off; replay without it.

## Recordings

- `race_test.jsonl` (2026-10-22, 121 s): the title, the menus, a race.
  Replayed, it reached the race ~78 s after the boot (`itc_738.log`).
