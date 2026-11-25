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
  off; replay without it. To dump a menu frame anyway, start the replay
  after the dump the renderer makes on its own in the title's background
  race (~4 s after the boot mark): `PAD_DELAY=8` (the title waits for the
  input), then `touch <dir>/now` at the moment wanted. With
  `race_test.jsonl`, 24.5 s after the boot mark is the tuning shop with the
  car and its floor reflection (2026-11-04, the front end's car draw).

## By tick (the state digest)

The virtual pad presses by the clock, so a press lands a tick early or late
from run to run: two runs of the same build drift apart within seconds of
a race, and a missed menu press sometimes leaves a run in the front end.
For checks that need the same game twice (E5's step 3, `docs/e5-native.md`
"Checks"), the pad is replayed by tick instead (2026-11-22):

- `--pad_ticks_record=<file>` writes every pad read the game makes (once a
  tick per user, `input::PhysicalPad_Read` through `XInput_GetState`) as a
  line, and a line `race <n>` when the game enters a race
  (`src/input/pad_ticks.cpp`).
- `--pad_ticks_play=<file>` answers the game's reads from such a file, in
  order, with no virtual pad (only user 0 connected, rumble ignored). The
  number of ticks before a race varies with load times, so playback lines
  up on each race's marks: reads left before one are skipped when the game
  gets there sooner, the last read repeated while it hasn't yet. Each race
  has two: `race <n>`, entered (its draw step seen), and `start <n>`, the
  race world loaded (the course set-up's message 4,
  `CourseSetup::WaitDraw`, after the race's loads; since 2026-11-22). The
  second is needed because the race's loads vary in length too: with only
  the first, a build whose loads ended one tick later started the race a
  tick later, and the recorded input then fell a tick early for the whole
  race (the world the same, one tick late, until the input differed). A
  file without start marks plays as before.
- `--state_digest` logs a line each race tick from the race's start (its
  first `start` mark; the still ticks of its loading left out)
  (`src/debug/state_digest.cpp`):
  the racers' count, a hash of each racer's body matrix and squared speed,
  a hash of user 0's last pad read, and the player's position.
- `itc_recomp/tools/tick_replay.sh [ticks] [baseline]` plays
  `itc_recomp/pad_recordings/race_test.ticks`, stops after it ends, and
  compares the digest with `race_test.digest`, printing each race's first
  differing tick. A change meant to change the game makes a new baseline
  (the run's `<log>.digest`).

`race_test.ticks` was recorded from `race_test.jsonl` with
`scripted_run.sh ... --pad_ticks_record --state_digest` (user 0's lines and
the marks kept); recorded again 2026-11-22 with the start marks, on E5 step
3's batch 1 build with `FloatPair`'s move left out (that build had matched
the old baseline, from the build before step 3, in both races), the
recording run's digest the baseline (`race_test.digest`: 112 and 1,183
ticks); two playbacks matched it, one lined up a read later at race 1's
start. Before that: Checked
2026-11-22: the recording run and three playbacks matched in both races,
all 124 and 1,196 ticks (a playback skipped 47 or 58 reads at the first
race, 0 at the second). Tried and dropped: waiting for the archive's
loads before each logic tick (so loads end on a fixed tick) deadlocked the
boot (the main thread waiting on the frame driver while its draw thread
looped in the render replay); the race marks made it unnecessary.

The game's own random generator is lined up too (2026-11-22, E5 step 4):
at each race's start mark, recording and playback alike set its seed
(`0x82638188`) to a fixed value per race (`PadTicks_OnRaceStarted`). The
camera shake draws from it on every tick of a race's loading, whose length
varies, so the next race's traffic (`Car::SetModel`'s random parts) moved
with the load times before. With that and `--game_rand_by_tick`
(`docs/frame-interpolation.md`), the baseline was made again (2026-11-22,
from E5 step 4's pilot build, `ae77718d`, with both; batch 1 matched it in
both races, twice).

## Recordings

- `race_test.jsonl` (2026-10-22, 121 s): the title, the menus, a race.
  Replayed, it reached the race ~78 s after the boot (`itc_738.log`).
