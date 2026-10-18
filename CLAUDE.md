# Working on this repo

A static recompilation of Import Tuner Challenge (Xbox 360) built on
ReXGlue (`rexglue-sdk/`, a submodule patched from `patches/rexglue-sdk/`).
`git status` showing `rexglue-sdk` as modified (`m`) is expected: those are
our patches applied in the submodule, not something to look into
(patch 0003, Windows SDL as a DLL, only changes Windows builds).
`docs/functions.md` is the reference: what's known about the game's code,
how frame interpolation works, every diagnostic option, and what didn't
work. `docs/gpu-functions.md` covers how the game drives the GPU (the
render command replay and the graphics library), and
`docs/native-renderer.md` is the plan and progress of the native renderer
that replaced the emulated GPU, and (its last section) the investigation
of the render thread's performance. `docs/native-audio.md` is the same for
the SDK's audio (the Audio Worker's spin, the game's sounds, the plan).
`docs/unlocked-res.md` covers drawing at other resolutions (`--width`,
`--height`, `--fullscreen`, black bars) and what's left to try.
`docs/windows.md` is the Windows build: cross-compiled on Linux, tested
under Wine (the user has no Windows machine).
`docs/old-hardware.md` is the plan (not started) for running below
Vulkan 1.3, and why not OpenGL.
`docs/tick-replay.md` is a lead (not started): translating draws once a
tick and replaying our own list on in-between frames.
`docs/pause-menu-fix.md` is the pause menu's too-fast input repeat: fixed
(the action mappers updated once a tick, `--input_mappers_by_tick`), and a
native Input::Update for later. Its last section, "The sub-menus' arrow",
is fixed on branch `pause-menu-arrow` (not merged yet): UI movie tracks
that add to values the update doesn't put back were re-added on every
render-only pass; now skipped there (`--per_tick_ui_movie_tracks`, a
workaround for frame rates above 30).
Read the relevant section before changing anything, and keep it up to
date as you learn things.

## Rules from the user

- Only search inside this repo. Ask for the location of anything outside it.
- Commit often. Never revert or `git checkout` uncommitted work without
  committing it first.
- Don't create branches (local or pushed) without asking the user first;
  work on the branch you're on unless they say otherwise.
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
  $B/itc --native_renderer=true --native_renderer_exclusive=true --native_audio=true \
  --native_renderer_mirror_by_tick=true --native_renderer_mirror_to_both=true \
  --game_data_root="$G" --mnk_mode=true --max_fps=144 --fullscreen=false \
  --log_fps=true --interpolate_draw_state=true <diagnostic flags>
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
```

- Test runs use what `run.sh` passes: always `--native_audio=true`
  (without it the SDK's Audio Worker spins on a core and frame rates come
  out lower than players get) and the mirrors updated once a frame
  (`--native_renderer_mirror_by_tick=true
  --native_renderer_mirror_to_both=true`; `=false` on the first for the
  old per-draw mirror). `profile_render_thread.sh` does the same.

- If the build fails with "file ... has been modified since the precompiled
  header ... was built" (clang was updated), delete the stale headers:
  `find itc_recomp/out/build/linux-amd64-release -name 'cmake_pch*.pch' -delete`.
- Compare frame rates uncapped (`--max_fps=0`), alternating runs: this PC
  holds the 144 cap, which hides differences. An attract race under
  `timeout 20` is enough: the race starts ~2 s after launch (the first
  `fps` line with `ticks/s 30`); take the mean over its seconds 3-18.
  Runs vary ~±1.5%, so for changes smaller than a few percent compare
  profiles instead (the user's call): `itc_recomp/tools/profile_render_thread.sh
  <dir holding itc> <name> [symbol regex]` gives the render thread's and
  each matching function's cycles a frame; two or three runs per build,
  alternating. A run pinned to one CPU
  (`taskset -c 0`) stands in for the Steam Deck but varies by ~15% from run
  to run.
- The release preset builds with ThinLTO (`ITC_LTO`, game and SDK), which
  needs lld (`ld.lld`). Measured 2026-10-14, two alternating uncapped attract
  races each: 236 and 237 fps mean without, 246 and 239 with; the user kept
  it. `-DITC_LTO=OFF` turns it off.
- That's what `run.sh` runs: our own renderer, no GPU plugin. For the
  emulated GPU (to compare), replace the two `--native_renderer` flags
  with `--gpu_plugin=xenos`; the plugin is built but no longer shipped.

- Launch from the repo root; other directories fail (exit 127).
- `--skip_intro --attract_after_s=1` starts an attract race (AI cars) about
  1 s after launch with no input, so races can be checked without the user:
  run under `timeout`, read the log, and look at frames with the
  renderer's dump (`--native_renderer_dump=<dir>`, 10 s into a race or
  when `<dir>/now` appears). The attract mode puts a random sepia or
  greyscale filter over the screen: a grey or brown frame from an attract
  race is the game's, not a rendering bug. Never screenshot the user's desktop. Without the GPU plugin, `--skip_intro` sometimes
  stalls the boot (no "called from 8248466C" in the log): restart it.
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
cp itc_recomp/out/build/linux-amd64-release/itc rexglue-sdk/out/linux-amd64/librexruntime.so $D/
install -m 755 itc_recomp/run.sh $D/run.sh
install -m 755 itc_recomp/run-debug.sh $D/run-debug.sh
(cd $D && python3 -I -m zipfile -c ../$name.zip itc librexruntime.so run.sh run-debug.sh)
```

## Status (`main`, 2026-10-17, v0.4.4)

Frame interpolation for races is working: logic runs at 30 Hz and frames are
blended between ticks. The world, cameras, wheels (other cars' too, paired
by position), lights, shadows, headlights (including their light on other
cars' bodies), other cars' lighting passing street lamps, tunnels, the race
start countdown, the HUD, the marker above opponents, the light trails, the
texture animations (flashing lights, signs, the off-ramp chevron), the
minimap triangle's flashing, the car's reflections, buildings' lighting,
play time and the glow (bloom; `src/interpolation/glow.cpp`, v0.3.9) are all done. `run.sh` turns
interpolation on (marked experimental) with logging off; `run-debug.sh` is
the same with logs; `./run.sh --no-interpolation` gives the original 30 fps.
Our renderer always draws motion blur and glow (`--motion_blur` and
`--glow` only affect the emulated GPU). Known issues are at the end of
`docs/functions.md`.

Other resolutions (v0.4.0, merged from `unlocked-resolution`):
`--width`/`--height` and `--fullscreen`, off by default (a 1280x720 window
at the game's own resolution out of the box); at other sizes every pass is
drawn scaled and the 3D view at the full resolution, with black bars on
shapes other than 16:9. See `docs/unlocked-res.md` (a more native frame
without the game's upscale draw is for later).

Pause menu (merged from `pause-menu`, 2026-10-18, not released yet): the
action mappers' update (`sub_8210B018`) runs on logic passes only
(`--input_mappers_by_tick`, on by default, `src/hooks/input.cpp`), so the
pause menu's input repeat counts ticks at any frame rate; the old
workaround (`--pause_workaround`, the pause screen held at 30 fps) is
removed. See `docs/pause-menu-fix.md`.

Xbox 360 gamma (merged from `xbox-gamma`, 2026-10-16; released in v0.4.3):
`--xbox_gamma=off|hdtv|power`, off by default (`run.sh` passes `off` and
lists the choices). The display gamma tables the game writes at boot,
copied into `src/native/xbox_gamma_table.h` (not read from the SDK), are
applied to the frame by a compute pass; `hdtv` looked pretty close to the
360 to the user. See "Gamma" in `docs/native-renderer.md`'s known issues.

Windows (merged from `windows`, 2026-10-16; released in v0.4.1, experimental):
cross-compiled on Linux with xwin, ThinLTO (`ITC_WIN_LTO`), `run.bat`
with `run.sh`'s defaults, the Visual C++ runtime DLLs in the zip, and a
CI job beside Linux's. Tested only under Wine: ~26% slower than Linux
there before ThinLTO, which then gave +3.1%. See `docs/windows.md`.

The game draws with our own Vulkan renderer (the native renderer, exclusive
mode; merged from `native-renderer`): the emulated GPU plugin isn't loaded
or shipped. Uncapped attract race: ~330 fps mean against the emulated GPU's
155 (~320 before `replay-allocations`: interpolation's constant replay
no longer allocates per upload, finds products in a flat table and
blends lighting vectorized, see "Profile of 2026-10-15, after v0.3.7" in
`docs/native-renderer.md`; 258 before `redundant-state`: draws no longer record state calls that
repeat the previous draw's, and the constant replay's diagnostics run only
with their flags; see "Performance: repeated state calls" in
`docs/native-renderer.md`; ~285 before `descriptor-reuse`: constant blocks
the same as the last written are reused, and constants are copied from
guest memory run by run with a vectorized byte swap, see "Performance:
reading constants from guest memory"). Since then (`blend-math`, `read-registers`): blends planned once a tick,
cameras blended once a frame, guest words byte-swapped with `pshufb`, and
the register shadow read only where the library's dirty masks say
("Performance: the render thread" in `docs/native-renderer.md`: "Done on
`blend-math`", "Register reads", "Against v0.3.7"); then (`mirrors-fix`)
the guest memory mirrors given memcpy's writes once a frame instead of
checked draw by draw (opt-in, on in `run.sh`; "Hypothesis to test: the
mirrors updated once a tick"). All released in v0.3.8: ~430 fps uncapped
in an attract race with `run.sh`'s flags (2026-10-15). Translated shaders and pipelines are kept on disk (`cache/` next to
the executable), and textures are reloaded when the game's memcpy writes
them instead of being hashed (`docs/texture-handling-optimisation.md`);
merged from `perf-debug` for Steam Deck hitches. Its known issues (boot movie, gamma, textures, window resizing,
remaining packet writers, speed) are milestone 7 in
`docs/native-renderer.md`.

Sound goes through our own audio output (`--native_audio`, on in `run.sh`,
since v0.3.2): our thread runs the game's mixer, replacing the
SDK's Audio Worker, whose polling took ~80% of a core. The SDK's XMA
decoder is kept. See `docs/native-audio.md`.

Releases are made by pushing a `v*` tag: CI builds the zip and publishes a
GitHub release with generated notes, which are then replaced with notes
written for players: title "vX.Y.Z: <summary>", and just a couple of
bullet points naming the changes (not the long layout of v0.3.5 and
earlier). `build-linux.yml` configures with `clang-20`/`clang++-20` by
name: since 2026-10-16 its build dependencies pull in Ubuntu 22.04's clang
14 package, which replaces `/usr/bin/clang` (the SDK needs 18 or newer).
A tag runs both `build-linux.yml` and `build-windows.yml`, and
both zips go on the release (see "CI" in `docs/windows.md`). Linux
release zips (`v*` tags only) are fully stripped (`strip --strip-all`: no
debug info or symbol table, nothing for gdb to go on, as the user wants;
the dynamic symbols stay, as loading needs them); CI artifacts from other
runs, local builds and test zips keep both for debugging.
Stripping doesn't change speed (2026-10-16: 431.5 against 432.0 fps mean,
two alternating uncapped attract races each).

Not done yet:

- Menus still run at 30 fps (the front end's; the pause menu runs at the
  race's rate; see Known issues).
- Other per-object draws (caller `0x82596D9C`) use projective matrices that
  aren't blended, so they step at 30 Hz; what they draw isn't known yet
  (see Known issues; `--log_moving_products` lists them).
- General renderer load: the render thread is the limit (flat out on the
  Steam Deck, at ~7 µs a draw), so any cut in the cost of a draw helps.
  Since v0.3.7, `blend-math` and `read-registers` cut
  ~12% of the render thread's cycles a draw (by profile, attract race;
  not yet checked on the Deck). Where the render thread's time goes, what's
  done and what's next: "Performance: the render thread" in
  `docs/native-renderer.md`. Since `mirrors-fix` (merged): the
  mirrors are updated at frame starts instead of checked draw by draw
  (`--native_renderer_mirror_by_tick` and `--native_renderer_mirror_to_both`,
  off by default, on in `run.sh`): -14.7% render thread cycles a frame,
  checked in attract races and a race the user drove ("Hypothesis to test:
  the mirrors updated once a tick" and what follows it). Then (2026-10-16)
  `Buffers_OnCommand`, `ReplayStats_OnCommand` and `GetRenderer` stopped
  setting up big stack frames on every call (ThinLTO had inlined their
  logging or start-up into them; ~-2% and ~150k cycles a frame, see
  "`Buffers_OnCommand`'s cost"). Merged from `texture-binding`
  (2026-10-16): texture views and samplers answered from a table by the
  fetch constant's words, and each stage's last texture set reused when
  unchanged: -6.9% render thread cycles a frame and +2.8% uncapped fps
  against the `main` before it ("Texture binding" in
  `docs/native-renderer.md`). Merged from `shader-lookup` (2026-10-16):
  shader lookups kept by what draws read from the shader objects instead
  of hashing the microcode every draw: -5.2% render thread cycles a
  frame; the main thread is now about as busy as the render thread on
  this PC, so `profile_render_thread.sh` skips `Main XThread` ("Shader
  lookup" in `docs/native-renderer.md`). Merged from `allocate-slots`
  (2026-10-16): the main thread had become the limit on this PC (99% of
  a core, the render thread 87%); `CommandList::AllocateSlots` (~51,000
  calls a frame) done natively with its diagnostics out of line: -14.4%
  main thread cycles a frame, +3.6% uncapped fps ("The main thread" in
  `docs/native-renderer.md`; `ITC_THREAD=main profile_render_thread.sh`
  profiles it). Merged from `perf-investigate` (2026-10-16): the lights
  nearest a point (`sub_820F4128`, ~6,240 lights tested a call, ~3 calls
  a frame) found natively, four lights tested at once with the same
  results: -14.1% main thread cycles a frame, +0.9% fps; the render
  thread (94%) limits again, the main thread at 84% ("Lights::FindNearest"
  in `docs/native-renderer.md`). Merged from `camera-ids` (2026-10-17):
  `CameraIds::Of` (~3,300 calls a frame, ~250 distinct matrices) keeps
  each matrix's camera number in a 1,024-entry table by its bytes
  (`--cache_camera_ids`), 98% hits, same numbers: -16.7% main thread
  cycles a frame, +0.7% fps (render thread limits; "CameraIds::Of" in
  `docs/native-renderer.md`). Merged from `drawstate-investigate`
  (2026-10-17, released in v0.4.4 with the three before it: main thread
  -41.7% cycles a frame against v0.4.3): the tick's products worked out once a tick and shared by
  its frames (`--publish_by_tick`), `Note`'s per-frame maps as flat tables
  and its lookups in the order noted, the billboards' cameras blended once a
  frame (`--cache_billboard_cameras`), guest matrices loaded and stored four
  words at a time: -22.0% main thread cycles a frame, render thread
  unchanged ("DrawState::Note and AfterDraw" in `docs/native-renderer.md`;
  next leads there: the game's `sub_821298C0` and `sub_8209C6E0`). A larger lead, translating
  draws once a tick: `docs/tick-replay.md`. Still to decide: the old
  per-draw path's future, and a Steam Deck test.
- Recheck whether the player's beam on the road still jumps in sharp turns
  (Known issues); if it's gone, drop it from the next release's limits.
