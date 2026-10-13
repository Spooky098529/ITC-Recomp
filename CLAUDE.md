# Working on this repo

A static recompilation of Import Tuner Challenge (Xbox 360) built on
ReXGlue (`rexglue-sdk/`, a submodule patched from `patches/rexglue-sdk/`).
`docs/functions.md` is the reference: what's known about the game's code,
how frame interpolation works, every diagnostic option, and what didn't
work. `docs/gpu-functions.md` covers how the game drives the GPU (the
render command replay and the graphics library), for the native renderer
that's to replace the emulated GPU. Read the relevant section before changing anything, and keep it up to
date as you learn things.

## Rules from the user

- Only search inside this repo. Ask for the location of anything outside it.
- Commit often. Never revert or `git checkout` uncommitted work without
  committing it first.
- Push work branches to `origin` (github.com/Spooky098529/ITC-Recomp) after
  each commit. Ask before pushing to `main` or pushing tags.
- No `-march=native`.
- US English spelling is fine.
- Find problems with logs, not guesses: add logging, have the user run the
  game, read the log, then fix what it shows. Guessing at causes cost many
  runs on the marker above opponents (see "World-space sprites").

## Build and run

From the repo root:

```sh
cmake --build itc_recomp/out/build/linux-amd64-release -j$(nproc)
G="$PWD/itc/Import Tuner Challenge (USA)"; B=itc_recomp/out/build/linux-amd64-release
XDG_DATA_DIRS=/usr/local/share:/usr/share LD_LIBRARY_PATH=$B:rexglue-sdk/out/linux-amd64 \
  $B/itc --gpu_plugin=xenos --game_data_root="$G" --mnk_mode=true --max_fps=144 \
  --log_fps=true --motion_blur=false --interpolate_draw_state=true <diagnostic flags>
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
```

- Launch from the repo root; other directories fail (exit 127).
- Run the game in the background. The user plays: they start a race, drive
  near an opponent and quit, then say what they saw.
- Logs go to `itc_recomp/out/build/linux-amd64-release/logs/itc_<n>.log`,
  newest first by `ls -t`. A big log rotates into `itc_<n>.1.log`, so check
  both. The race starts where `fps` jumps above 30.
- `generated/` is the recompiled code. Find a function with
  `grep -n "DEFINE_REX_FUNC(sub_XXXXXXXX)" itc_recomp/generated/default/*.cpp`,
  or a call site with `grep -ln "lr = 0xXXXXXXXX;"`. Override a function by
  defining `extern "C" REX_FUNC(sub_XXXXXXXX)` in `src/` and calling
  `__imp__sub_XXXXXXXX`. Only one override per function can exist (the link
  fails with "multiple definition" otherwise), so `grep -rn sub_XXXXXXXX
  itc_recomp/src` first.

## Test builds for the user

Package them into `itc_recomp/out/dist` with Python's zipfile:

```sh
name=itc-<label>-linux-amd64; D=itc_recomp/out/dist/$name
rm -rf $D $D.zip; mkdir -p $D
cp itc_recomp/out/build/linux-amd64-release/itc itc_recomp/out/build/linux-amd64-release/librexgpu-xenos.so rexglue-sdk/out/linux-amd64/librexruntime.so $D/
install -m 755 itc_recomp/run.sh $D/run.sh
install -m 755 itc_recomp/run-debug.sh $D/run-debug.sh
(cd $D && python3 -I -m zipfile -c ../$name.zip itc librexruntime.so librexgpu-xenos.so run.sh run-debug.sh)
```

## Status (`main`, 2026-10-12)

Frame interpolation for races is working: logic runs at 30 Hz and frames are
blended between ticks. The world, cameras, wheels (other cars' too, paired
by position), lights, shadows, headlights (including their light on other
cars' bodies), other cars' lighting passing street lamps, tunnels, the race
start countdown, the HUD, the marker above opponents, the light trails, the
texture animations (flashing lights, signs, the off-ramp chevron), the
minimap triangle's flashing, the car's reflections, buildings' lighting and
play time are all done. `run.sh` turns
interpolation on (marked experimental) with logging off; `run-debug.sh` is
the same with logs; `./run.sh --no-interpolation` gives the original 30 fps
with motion blur. Known issues are at the end of `docs/functions.md`.

Releases are made by pushing a `v*` tag: CI builds the zip and publishes a
GitHub release with generated notes, which are then replaced with notes
written for players (title "vX.Y.Z: <summary>", same layout as earlier
releases).

Not done yet:

- Races don't hold 144 fps in busy areas (about 105-150). The render thread
  is the limit, and about half of it is the game busy-waiting on the
  emulated GPU, whose command thread is the real bottleneck (see
  "Performance" in `docs/functions.md`). Investigation on branch
  `perf-investigation`.
- Menus still run at 30 fps (see Known issues).
- Other per-object draws (caller `0x82596D9C`) use projective matrices that
  aren't blended, so they step at 30 Hz; what they draw isn't known yet
  (see Known issues; `--log_moving_products` lists them).
- Recheck whether the player's beam on the road still jumps in sharp turns
  (Known issues); if it's gone, drop it from the next release's limits.
