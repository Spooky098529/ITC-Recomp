# Known functions: the race HUD's movies and the rival logo

The race's **HUD object** (`RaceHud::`, probably; the code at about
`82211B98`-`82215820`, mapped in `race-hud-object.md`: set-up
`sub_822122B0` from `sub_8216D450`, update `sub_82214718`, teardown
`sub_822141A0`, which make, update and free the track zones,
`track-zones.md`) and, just before it, the rival intro's logo
(`sub_82211A28`). Both run UI movies (`ui-movies-game.md`): the HUD at
the race object's `[+9608] + 16`, from four callers in `8216C330` to
`8216D0C8` (probably the race's draw steps; not read), which hand
`RaceHud::RunMovie` r4 = 1 (draw only) or 0; by its state it runs or
draws one of its movies (+604, +624, +632). The HUD's movies run from
Draw (`822128C8`, `8221293C`, `822129C8`; "The rival logo at the race
start" in `../frame-interpolation.md`), so they get render-only updates,
replayed. The logo's draw is noted during the cutscene and repeated on
render-only passes (`--redraw_rival_logo`, its override in
`itc_recomp/src/frame_pacing/per_frame_logic.cpp`, kept).

Made native by the B7 loop (the DrawTexts lead's second migration,
2026-10-25) under `--native_race_hud` (on by default;
`itc_recomp/src/race_hud/`, adapters in `itc_recomp/src/hooks/race_hud.cpp`,
harness `--run_harnesses=race_hud`): the choice of movie and the logo's
lookup as plain C++, the movies' calls (`UiMovie::Run` `sub_8211E1B8`,
`UiMovie::Draw` `sub_8211D280`, `UiMovie::FinishNamed` `sub_8211DF20`)
through their overrides with the originals' return addresses. Neither
runs in an attract race (the HUD isn't set up; no rival), so no frame
dump shows them; the harness answers the movies' calls with a stand-in.
`sub_82211A28`'s row moved here from `functions.md`. Never seen in play,
so no row: `sub_82212958` (`RaceHud::RunMovie`'s state 3, left to the
original).

Status:

- **Native**: plain C++ in `itc_recomp/src/` that doesn't use the
  PowerPC context (`ctx`): normal arguments and host structs (it may
  still read and write guest memory through `src/guest.h` while
  recompiled code shares the data). The game reaches it without `ctx`,
  from native or overridden callers, and the recompiled original is no
  longer called (except behind a fallback flag or to check against it).
  No `REX_FUNC` override of it exists, not even an adapter kept only for
  a harness (that's Overridden until the adapter goes).
  The end state for every function.
- **Overridden**: an `extern "C" REX_FUNC(sub_XXXXXXXX)` override in
  `itc_recomp/src/` (or a macro defining one) does the work instead of
  the original: a thin adapter in front of native code, or a body that
  still uses `ctx` (calling guest code, reading registers). It may call
  the original when its flag is off, or to check itself against it.
- **Wrapped**: an override that does something of its own (skipping
  render-only passes, logging, timing) and still calls the original
  (`__imp__sub_XXXXXXXX`) for the work.
- **Dropped**: its work isn't needed on PC (hardware set-up, kernel calls
  the SDK already stubbed), so it no longer runs: an override in
  `itc_recomp/src/` replaces it with nothing (or a fixed return value), or
  its callers, made native, leave the call out (no override then; the row
  says which caller). Logs, a harness or the renderer's frame dumps have
  shown that nothing reads what it did. The original is no longer called,
  except behind a fallback flag while the drop is being checked. A
  function the game never calls in the first place isn't Dropped: its
  status stays blank and its row says it's never called.
- **Merged**: its work is done inside a native caller's plain C++ (a
  few lines only that caller needs), so it has no function or override of
  its own in `itc_recomp/src/`. The row names the caller(s) and the
  harness that covers the work. The original is no longer called, except
  behind the caller's fallback flag.
- **Patched**: the generated code was edited in place (search for
  `PATCH:`).

A function usually moves from recompiled or Wrapped to Overridden to
Native as the `ctx` edge moves up through its callers, or straight to
Dropped when its work isn't needed.
`itc_recomp/tools/check_function_statuses.py` checks this column, in
every file of the tables, against `src/` (an override marked Native, a
status with no override, an override with no row, a row in two files);
run it after changing either, on any branch.

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_822141E8` | `RaceHud::RunMovie` (probably) | Overridden | r3 = the HUD, r4 = `UiMovie::Run`'s (0 advance, else draw only), passed on. By the state (+448): 0, its movie at +604 run (`sub_8211E1B8`) when +456 isn't 0, else drawn (`UiMovie::Draw`, `sub_8211D280`); 1, `sub_82212908`; 2 and 4, its movie at +632 run; 3, `sub_82212958` (never seen in play); 5 and above, nothing. All tail calls. From `sub_8216C330`, `sub_8216CE00`, `sub_8216CFE0` (r4 = 1, after `sub_822128A8`), `sub_8216D0C8` (not read). Read in full (B7 loop, 2026-10-25). Native (`--native_race_hud`): `race_hud::RunMovie` (`src/race_hud/race_hud.cpp`), its adapter (`src/hooks/race_hud.cpp`) making the tail calls through their overrides (lr, r1 and r4 as they came), state 1 as `sub_82212908`'s from its frame, state 3 the original `sub_82212958`. Harness `race_hud` (400 cases; state 3 left out). Seen in play; not in attract races. |
| `sub_82212908` | `RaceHud::RunTimerMovie` (probably) | Merged | r3 = the HUD, r4 passed on: `RaceTime::FromTicks` (`sub_82101760`) on the word +3932 of the object at +444 (probably the race's tick count; its result unused), then the movie at +624 run (`sub_8211E1B8`, from `0x8221293C`; probably the race timer's). A 112-byte frame (r30, r31, lr saved). Only caller `sub_822141E8` (a tail call, state 1). Read in full (B7 loop, 2026-10-25). Merged into `sub_822141E8`'s adapter (`--native_race_hud`): the run from a 112-byte frame with its return address; `FromTicks` not called (a pure leaf whose result nothing reads). Harness `race_hud` (`RaceHud::RunMovie`'s state 1 cases). |
| `sub_82211A28` | `RivalLogo::Draw` (probably) | Overridden | Draws the rival's logo; noted during the cutscene for the render-only redraw (`--redraw_rival_logo`, its override in `src/frame_pacing/per_frame_logic.cpp`). r3 = the logo's entry (+4 flags: bit 8 shown, bit 16 finished; +8 the movie's id). Read in full (B7 loop, 2026-10-25): a 112-byte frame; while shown, the id stored at its +80 and the movie found in the movie table (`UiMovies::Find`, `sub_8211B3F0`, on `0x826C5CCC`), run (`sub_8211E1B8`, r4 = 0, from `0x82211A74`); when that returns 0, `UiMovie::FinishNamed` (`sub_8211DF20`, r4 = the id) and bit 16 set. From `sub_82155A28` (the cutscene) and `sub_821594B8`'s override (the redraw). Native (`--native_race_hud`): `race_hud::RivalLogoMovie` and `RivalLogoFinished` (`src/race_hud/race_hud.cpp`), `race_hud_hooks::DrawRivalLogo` making the calls through their overrides from a frame like the original's; the override's noting unchanged. Harness `race_hud` (400 cases: a made-up movie table, put back after each). Not in attract races. |
