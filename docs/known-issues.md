# Known issues

The game's known issues on PC. Moved out of
`docs/functions/functions.md` on 2026-10-20.

- **Menus run at 30 fps** (the front end's; the pause menu in a race runs
  at the race's frame rate since `pause-menu`, its input repeat fixed by
  updating the action mappers once a tick, see `docs/pause-menu-fix.md`):
  they record their UI and read input during Update, so render-only passes
  made them flicker and repeat presses. Unlocking them would mean skipping
  Update only for the handlers that run game logic.
- **The race HUD updates at 30 fps.** Accepted for now; its animations
  advance once per tick (see "Per-frame logic").
- **More per-frame logic may exist.** Anything that runs too fast or
  flickers only with frames uncapped is probably another case; see
  "Per-frame logic" for how they were found.
- **Some uploads still step on purpose**: the shadow lookup matrices (c70 and
  c74, orthographic, kept at the tick to match the shadow map), c14 x4 (a
  texture projection, built by `sub_82598768`; zeroing it changes nothing
  visible). c100 x4 used to be listed here too: it's the motion blur's
  previous view-projection (zeroing it disturbs the blur), now blended on
  every frame (`--motion_blur_by_tick`; see "Motion blur reads the 30 Hz
  tick" in `docs/native-renderer.md`). Nothing visible steps in testing; see
  "Finding what still steps" if something does.
- **The cockpit judders left to right far from the map's origin**
  (x ≈ −8,000 to −10,000 m), with interpolation off too (seen under Wine,
  uncapped, `--interpolate_draw_state=false`, 2026-10-19), so it's the
  game's own: its per-tick camera offset from the car wobbles by 1–4 mm a
  tick out there (`--log_cockpit_judder`), the recurring 3.9 mm maximum
  being 2⁻⁸ m, a float step at that distance. Our blend's own jerk is
  ~0.01 mm with `--double_precision_blending`. Left as is: the eye/target/up
  the game's camera pass stores are float too, and smoothing the camera's
  offset from the car across ticks would make head movement lag the car.
- **Projective matrices from `0x82596D9C`** (the per-object recorder's
  other caller seen moving in races) can't be split into position and
  rotation, so they're drawn at the tick (`--log_moving_products` lists them
  as not blended, moving 1-8 m a tick). What they draw isn't known yet.
- **One tick of latency**: drawing shows between the previous and the
  current tick, as usual for this kind of interpolation.

## Calls by address with no dispatch entry (found 2026-11-26)

Lifted code calls some functions by a constant address through the dispatch
table, and an address with no entry stops the program ("lifted: a call to
X, which has no entry in the dispatch table"). Hand ports whose adapters were
removed (only native callers were left in play) have no entry, so a lifted
original that a native caller falls back to can reach one. Two were hit by
pad replays that took a rarer menu path (slower runs, on `pc-renderer`):
`BuildScreen::RestoreBuildList` (`8219BB80`) and `CarDealer::SetScrollArrows`
(`82182DD0`); both have entries now.

`itc_recomp/tools/call_targets.py > <file>` lists every constant call target
in `src/`, and `itc --check_call_targets=<file>` logs those with no entry and
exits: 441 of 1,720 left. 438 are called only from `src/lifted/originals.cpp`
(the fallback originals); 3 from live lifted code (`825B0BB0`, `825B0BE4`,
`825B8778`: the C runtime's floating-point exception paths, not reached in
play). Giving every hand port an entry (or the originals direct calls) is
left for later.

**A new career stops in the car dealer (found 2026-11-27, Linux and
Windows alike).** With no save (`saves/` next to the executable empty), the
career's first visit to the car dealer goes through lifted `sub_82185518`
and OpenColors `sub_821847A0`, which call hand ports by address. Entries
were added for `CarDealer::SetColorBar` (`82182770`), `ShowSwatches`
(`821826C0`), `SetKindBar` (`82182158`) and `BuildScreen::ValueText`
(`8219BCC0`, the build screen stop seen in drifted pad replays); the run then
stops at `821824A8`, so more are missing on that path. Every new player
would hit it: to fix by giving the hand ports on the list above their
entries, then checking a no-save run (the tick replay's input from an empty
folder) gets through the dealer. Left for later by the user's call
(2026-11-27, during the D3D11 work).
