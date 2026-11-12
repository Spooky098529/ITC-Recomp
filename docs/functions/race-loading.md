# Known functions: the race's loading

The **race's loading**: the race mode's first state machine (race mode
+ 848, `CourseSetup::`, set up by `CourseSetup::Init` `sub_821517D8`
from `RaceMode::Init`, `game-modes.md`; names here are ours, from the
debug prints "CRS SETUP WAITING." and "DRAW SETUP WAITING." its two
waits pass to the empty `sub_82106540`), the race mode's tick that runs
every machine (`RaceMode::NextMode`, the mode's slot 6), and the
**loading screen** the race and the front end show while loading (three
UI movies, `LoadingScreen::`). The machine starts the race's load: it
fades the screen in, waits for the course (its scenery, collision mesh
and route loaded, the text and the data tables ready), then for the
archive's queue to empty, fades out and posts message 4, which the race's
other machines take as the race world loaded (`race-states.md`,
`race-sounds.md`); a restart (message 44) goes back through the course
wait, and at the race's end (message 49) it waits for the scene's loads
and the fades to finish, running the car scene's update meanwhile, then
posts 50 (the unload). While it waits for the course or the archive
(states 3 and 4), `RaceMode::NextMode` runs the loading screen a pass,
and once it is unloaded (state 35) the mode's next-mode answer comes
from `sub_82150E00` (the career's side, a lead in `../loops/leads.md`).
The loading screen's movies are loaded at boot (`LoadingScreen::Load`,
`AddMovie`, `data-load.md`); `LoadingScreen::Show` picks one, resets it,
sets its frames and text (movie 0 shows one of 14 tips picked by the
local time's milliseconds) and plays its animation looped;
`LoadingScreen::Run` runs it a pass and `LoadingScreen::Finish` finishes
it. Read by the B7 loop's mapping agent, 2026-11-04, from the two leads
"the loading screen" and what was left of "the race mode's set-ups"
(`../loops/leads.md`). The fields are in `../known-data.md` ("course
set-up", "race mode", `0x82815E60`-`0x82815E73`, `0x8263799C`).

The machine's words (relative to it, race mode + 848): +9604 the state
(1 at set-up), +9608, +9616, +9620, +9628, +9632, +9636 cleared by
`Begin` (+9608 also by `WaitDraw`; not read here otherwise), +9640
cleared by `OnCourseReady` (not read here), +9644 the end's tick count
(0 at set-up, counted by `EndTick`), +9648 `WaitDraw`'s step (0 before
the fade-out, 1 fading; cleared by `WaitCourse`). The handlers get r3 =
the machine and r4 = the race mode's dispatcher (+36), and post their
messages into its ring (+204 from it: 50 entries of 12 bytes, the count
at +804 and the start at +808, as `GameMessages::Dispatch` reads them,
`game-modes.md`): the message's word, then two zero words.

The transitions `CourseSetup::Init` adds ((state, message) → next
state, handler; 0 = stay; handlers are functors of vtable `0x82011918`,
a member function on the machine (`Delegate::Invoke`, `functions.md`),
or `0x820069BC`, a state change only): (1, 0) `Start`; (1, 2) → 3
`Begin`; (3, 0) `WaitCourse`; (3, 3) → 4 `OnCourseReady`; (4, 0)
`WaitDraw`; (4, 4) → 7; (7, 44) → 32; (32, 0) `Restart`; (7, 49) → 37;
(37, 0) `EndTick`; (37, 50) → 35 with the empty `sub_82106540`
(`audio.md`). 11 transitions. So: 1 started, 3 waiting for the course,
4 waiting for the archive and the fade, 7 the race (the machine idle),
32 restarting (`Restart` sets 3 and posts 3, so the course wait runs
again and message 4 is posted again: 44 is probably the race's restart,
as `RaceSounds::OnAbort` and `OnAbortDone` read it), 37 the end, 35
unloaded. Messages 2 and 3 are its own (posted by `Start` and
`WaitCourse`; no other machine's set-up read so far takes them); 4 the
race world loaded, 49 the race's end, 50 the unload (as the other
machines' files read them).

`Init` is 379 generated lines (an 864-byte frame): each functor is built
on the stack (the vtable, the machine and the member function: three
words, one for a state change only) and its six storage words copied,
so the words not set are stack leftovers, copied into the transitions as
the original leaves them (the precedent of the race HUD machine's and
the race sounds' `Init`); the key is (state, message, 0, 0) and the next
state the storage's +24.

The loading screen's three movies are at `0x82815E68` (movie 0 by
`RaceMode::Init` with a tip, movie 2 frame 671 by the race's states'
loads, `race-states.md`; movie 1 by the front end's callers, not read);
the animation each plays is a word of the table at `0x8263799C` (0, 13
and 0 in the image; the lead called it names, but `UiMovie::FinishNamed`
and `PlayNamed` take an animation index). Its callers: `RaceMode::Init`,
`RaceMode::NextMode` and 12 of the race's states (`race-states.md`), and
front-end modes' set-ups and ticks (`sub_8222FD68`, `sub_82236880`,
`sub_8223D5D8`, `sub_82246C90`, `sub_8225D6D0`, `sub_82278058`,
`sub_82197278`, `sub_82197B00`, `sub_82230050`, `sub_82235F28`,
`sub_8223DBC0`, `sub_822463D8`, `sub_8225D918`, `sub_8227B440`,
`sub_82288CA0`; in leads, other files' notes or none, not read here).
Its `UiMovie::SetText` calls leave r7 to r10 unset (a varargs callee):
native callers pass 0 for them (the user's call for the parking area,
2026-10-30).

Shared with other files, which keep their rows: `RaceMode::Init`
(`sub_82151D20`, `game-modes.md`), `StateMachine::Add` (`sub_82106158`,
`game-modes.md`), `GameMessages::Dispatch` (`sub_821062E8`,
`game-modes.md`), `Mode::NextModeThunk` (`sub_82235C08`, the race mode's
delegate's member function, `boot-mode.md`: it calls the mode's slot 6,
`RaceMode::NextMode` here), `Delegate::Invoke` (`sub_8223A3F0`) and
`GetLocalTime` (`sub_822968B8`, B6's time helpers beside
`GetTimeZoneBias` and `GetTimeZoneInformation`, kept with them in
`functions.md`), the empty `sub_82106540` (`audio.md`),
`ScreenFades::FadeIn` and `FadeOut` (`sub_8209D038`, `sub_8209D0C8`,
`mode-tick.md`), `Archive::QueueLength` (`sub_8209D7B0`, `archive.md`),
`Scenery::IsLoaded` (`sub_820F1F68`, `course-scenery.md`),
`Text::IsLoaded` (`sub_82211C90`, `strings-and-font.md`),
`DataTables::IsLoading` (`sub_82117158`, `data-tables.md`),
`CarScene::LoadsDone` and `CarScene::Update` (`sub_820C5798`,
`sub_820C9608`, `scene.md`), the UI movie calls (`UiMovie::ResetPose`
`sub_8211ABA8`, `ShowNode` `sub_8211AC00`, `HideNode` `sub_8211AC48`,
`SetText` `sub_8211C1F8`, `FinishNamed` `sub_8211DF20`, `PlayNamed`
`sub_8211DAC8`, `LoopLast` `sub_8211DDE8`, `Run` `sub_8211E1B8`,
`ui-movies-game.md`), `LoadingScreen::Load`, `AddMovie` and `Free`
(`sub_82247B98`, `sub_82247AA0`, `sub_82247B28`, `data-load.md`),
`Stages::Copy` (`sub_820FFD88`, `vehicle-accessors.md`) and the in-race
screens' helpers `sub_82211CE0`, `sub_82211CF8`, `sub_82211E90`,
`sub_82211F18` (`race-hud-object.md`). Not here: `sub_82150E00` (the
race's end, `RaceMode::ModeAfterRace` in [`race-end.md`](race-end.md)), `sub_82293358` and
`sub_82292828` under `sub_82293460` (a lead), `sub_82260800` (under the
race mode's +32 slot 0, never run; not read), and the race mode's slots
8 and 9 (`sub_8223D960`, `sub_8223DA00`, in the front end's range, not
seen in play; not read).

Ready (by `b7_groups.py`, 2026-11-04): `Init`, `Start`, `Begin`,
`OnCourseReady`, `WaitDraw`, `LoadingScreen::Show` (with
`UiMovie::SetFrames`), `Run`, `Finish` and `sub_82293460`. All made native by the B7 loop's thirty-first run, 2026-11-04
(`src/race_loading/`, `--native_race_loading`, harness `race_loading`):
the handlers' adapters stay (reached through the machine's functors),
the others' too (recompiled callers). Blocked:
`WaitCourse` by `Scenery::IsLoaded` (itself waiting on `sub_820F1598`),
`EndTick` by `Scenery::IsLoaded`, `CarScene::LoadsDone` and
`CarScene::Update`, `RaceMode::NextMode` by `sub_82150E00` (and
`sub_82292710` under it; since 2026-11-12 both ready together,
`race-end.md`).

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
| `sub_821517D8` | `CourseSetup::Init` | Overridden | r3 = the race mode's state machine at +848. +9604 = 1, then the 11 transitions above through `StateMachine::Add` (`game-modes.md`), then +9644 = 0. From `RaceMode::Init` only. 379 lines. Read in full. Seen in play. Native under `--native_race_loading` (`src/race_loading/race_loading_init.cpp`: the stores and copies replayed in the original's 864-byte frame, so the transitions keep the stack's words as the original leaves them, the race flow's and race sounds' precedent; `StateMachine::Add`'s plain C++; harness `race_loading`); the adapter stays (its caller, `RaceMode::Init`, is recompiled). |
| `sub_82151408` | `CourseSetup::Start` | Overridden | (1, 0), r4 = the dispatcher: posts message 2, then `ScreenFades::FadeIn` with r3 = `[0x82001BEC]` (5 in the image) passes, black (r4 to r6 = 0), layer 0 (r7 = 0). 44 lines. Read in full. Seen in play. Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`: the frame's back chain, the post, `ScreenFades::FadeIn`'s plain C++; r3 the passes, as FadeIn leaves it; harness `race_loading`); the adapter stays (reached through the machine's functors). |
| `sub_82150DE0` | `CourseSetup::Begin` | Overridden | (1, 2) → 3: +9616, +9620, +9608, +9632, +9636 and +9628 = 0. Leaf. Read in full. Seen in play. Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`; harness `race_loading`); the adapter stays (reached through the machine's functors). |
| `sub_821514A0` | `CourseSetup::WaitCourse` | | (3, 0), r4 = the dispatcher: when `Scenery::IsLoaded([0x826C5514])` answers nonzero, the course's collision mesh and route are loaded (`[0x826C5B24]` and `[0x826C5B18]`, the states of `0x826C5B1C` and `0x826C5B10`, both 2), `Text::IsLoaded` answers nonzero (r3 = `[0x826D84F0]`, unused) and `DataTables::IsLoading` 0: posts message 3 and +9648 = 0; else the debug print "CRS SETUP WAITING." (`0x820092C4`; `sub_82106540`, empty, r3 = 500, r4 = 100, r5 = 0). 75 lines. Read in full. Seen in play. |
| `sub_82150DD0` | `CourseSetup::OnCourseReady` | Overridden | (3, 3) → 4: +9640 = 0. Leaf. Read in full. Seen in play. Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`; harness `race_loading`); the adapter stays (reached through the machine's functors). |
| `sub_821515A0` | `CourseSetup::WaitDraw` | Overridden | (4, 0), r4 = the dispatcher. With +9648 = 0: once the archive's queue is empty (`Archive::QueueLength` 0), `ScreenFades::FadeOut` with r3 = `[0x82001BF0]` (15 in the image) passes, black, layer 0, unless layer 0's alpha (`0x826A50D4`) is already 255, and +9648 = 1. With +9648 = 1: once the alpha is 255, posts message 4 (the race world loaded) and +9608, +9648 = 0. Otherwise (and after the first step) the debug print "DRAW SETUP WAITING." (`0x820092D8`, as `WaitCourse`'s). 74 lines. Read in full. Seen in play. Returns r3 = 500 (the print's) or the machine (when message 4 is posted). Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`: the frame's back chain, `Archive::QueueLength`'s and the fade's plain C++, the print left out; harness `race_loading`, the live queue's count set under its lock); the adapter stays (reached through the machine's functors). |
| `sub_82151698` | `CourseSetup::Restart` | | (32, 0), r4 = the dispatcher: +9604 = 3, then posts message 3 (so (3, 3) → 4 follows: the course wait skipped, `WaitDraw` again). Leaf. Read in full. Not seen in play. |
| `sub_821516F0` | `CourseSetup::EndTick` | | (37, 0), r4 = the dispatcher: +9644 plus 1; when `Scenery::IsLoaded([0x826C5514])` answers nonzero, `CarScene::LoadsDone([0x826C2E44])` answers 1, the archive's queue is empty, +9644 is above 2 and layer 0's fade counts (`0x826A50E4` out, `0x826A50DC` in) are both 0: posts message 50 (the unload); otherwise `CarScene::Update` (`scene.md`). 71 lines. Read in full. Seen in play. |
| `sub_82151EF0` | `RaceMode::NextMode` | Overridden | The race mode's slot 6 (vtable `0x82006D34`), the next-mode query `Modes::Tick` asks every tick through the delegate (`Mode::NextModeThunk`, `boot-mode.md`; no direct callers): r3 = the 8-byte answer, r4 = the mode. The answer cleared (byte +0, word +4); when +167260 isn't 0 (1 after `RaceMode::Init`; 0 while the +32 slot 0 below holds the race) posts message 0 (Update), then always message 1 (Draw), into the dispatcher's ring (+240: count +840, start +844); with the +848 machine in state 3 or 4 (+10452), `LoadingScreen::Run(-1)`; then `sub_82293460`, then `GameMessages::Dispatch` (+36) delivers them to the nine machines; with the +848 machine in state 35 (unloaded), `RaceMode::ModeAfterRace` `sub_82150E00`(a temporary, the mode)'s 8 bytes copied into the answer (the mode to go to after the race, [`race-end.md`](race-end.md); ready with this function since 2026-11-12). Returns r3. 91 lines. Read in full. Seen in play. Overridden (B7 loop, 2026-11-12): plain C++ `race_end::NextMode` in `itc_recomp/src/race_end/race_end.cpp` behind an adapter in `src/hooks/race_end.cpp` (`--native_race_end`, with `RaceMode::ModeAfterRace`, not this file's `--native_race_loading`); `LoadingScreen::Run`'s and `sub_82293460`'s plain C++ and `RaceMode::ModeAfterRace`'s called directly, `GameMessages::Dispatch` through its override; reached through `Mode::NextModeThunk` (the race mode's slot 6), so the adapter stays. Checked by the `race_end` harness (its dispatcher without handlers, the loading screen's movie world, the +848 machine's states 0 to 35). Runs every tick of an attract race (native running (game)). |
| `sub_82293460` | | Native | No arguments: when `[0x82816E3C]` isn't 0 and `sub_82293358` answers nonzero, `sub_82292828([0x82816E3C])` and returns 1; else 0. `sub_82293358` (not seen in play) checks `[0x826C7958]` against `[0x826C7944]` and a limit, the player's bit (`[0x826BABA4]`) in `[0x826C7954]` and `[0x826C795C]`, then by `[0x826C6AA8]` (4109 to 4113) calls one of `sub_82293288`, `sub_822929E8`, `sub_82292FB0`, `sub_82291B50`, `sub_82291760`; probably a system request (a sign-in or device change) the game answers; in play its callees never ran, so `[0x82816E3C]` stayed 0 or the check failed. From `RaceMode::NextMode` and four front-end functions (`sub_8225F078`, `sub_82276248`, `sub_82280C68`, `sub_822872A8`, none seen in play). 32 lines. Read in full; its callees not read in detail (a lead). Seen in play. Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`, `CheckSystemRequest` (our name): its two callees through the function table; harness `race_loading`, the request 0 or `sub_82293358`'s first test failing); the adapter stays (recompiled callers). Native (B7 loop, 2026-11-12): its adapter removed once `RaceMode::NextMode` (its only caller in play) was native and calls its plain C++ (`--native_race_end`); `--native_race_loading` no longer switches it; the four front-end callers, never seen in play, reach the original. Harness `race_loading` through the register step. |
| `sub_821511B8` | | | The race mode's +32 vtable's slot 0 (`0x82006D28`; no direct callers): r3 = the mode + 32, r4 = a reason (0 to 7). The current stage (`Stages::Copy` from the race data `[0x826C5B2C]` + 0x59620, r5 = -1); unless the +848 machine is racing (state 7) and `sub_82211E90([0x826D84F0])` answers nonzero, `sub_82211CE0` and returns 0; else the mode's +167260 = 0 (no Update messages), the race data's flags +0x59AE4 get 0x8000 for r4 0, 1 and 4 (4 after `sub_82260800`) or 0x10000 for r4 2, 3, 5 and 6 (nothing for 7), and with kind 6 (+0xA3AF0) and the stage's +0 not 1, `sub_82260800` after the flag for r4 0 to 3; returns 1. Probably the race held for a system event (the system UI or a pad). 100 lines. Read, not in detail. Not seen in play. |
| `sub_821512F0` | | | The race mode's +32 vtable's slot 1 (no direct callers): r3 = the mode + 32. Every machine sent to its end: state 37 for +848, +10528, +87936, +97548, +107168, +116812 and +138284, 24 for +128664 (the race HUD machine), 6 for +147944 (the race overlays'); the mode's +167260 = 1; the race data's +0xA3ACC = 1; then `sub_82211CF8` (a jump to `sub_82292710`). Probably the race ended from outside (a sign-out or the like). Leaf but for that. Read in full. Not seen in play. |
| `sub_82151388` | | | The race mode's +32 vtable's slot 2 (no direct callers): r3 = the mode + 32. The race data's flags +0x59AE4 lose 0x8000 and 0x10000 (slot 0's), `sub_82211F18([0x826D84F0])`, and the mode's +167260 = 1: slot 0's hold undone. Read in full. Not seen in play. |
| `sub_822478D0` | `LoadingScreen::Show` | Overridden | r3 = with a tip (movie 0 only), r4 = which movie (0 to 2, unsigned; 3 or more does nothing), r5 = a frame or -1. The movie `[0x82815E68 + 4 × r4]`, `0x82815E64` = r4; `UiMovie::ResetPose`; with r5 not -1, `UiMovie::SetFrames(movie, r5, r5)`; movie 0: `UiMovie::SetText(movie, -1, 3, 1200)`, movie 2: `SetText(movie, -1, 2, 1200)`; movie 0 with r3 nonzero: `GetLocalTime` (a SYSTEMTIME on its stack), its milliseconds modulo 14 picks the pair, string 1201 + 2 × pair into node 9 and the next into node 10, both shown (`UiMovie::ShowNode`); movie 0 with r3 0: nodes 9 and 10 hidden (`HideNode`). Then `UiMovie::FinishNamed` and `PlayNamed` (r5 to r8 = 0) with the movie's animation (`[0x8263799C + 4 × r4]`), `UiMovie::LoopLast`, and `0x82815E60` = 1. Calls seen: (1, 0, -1) from `RaceMode::Init` and the front end's, (0, 2, 671) from the race's states. 13 callers (above). 162 lines with `SetFrames`. Read in full. Seen in play. Its frame's back chain is written before the test of r4 (r3 left for 3 or more). Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`: the UI movie calls and `GetLocalTime` through their overrides from the original's frame with its return addresses, `SetFrames`'s plain C++; `SetText`'s r7 to r10 passed as 0, the parking area's precedent, so only its frame's +48 to +79 differ; harness `race_loading`, a UI movie world of its own with a made-up time); the adapter stays (recompiled callers). Its frames drawn as with the flag off (a dump during an attract race's load, 2026-11-04). |
| `sub_82247898` | `LoadingScreen::Run` | Overridden | r3 = which movie, or -1 for the current one (`[0x82815E64]`); 3 or more does nothing: tail-calls `UiMovie::Run(movie, 0)` (advance and draw). From `RaceMode::NextMode`, the race's states' loading waits (`race-states.md`) and the front end's modes (16 callers). Leaf but for that. Read in full. Seen in play. r3 compared signed: a negative index other than -1 reads before the movies' words. Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`: `UiMovie::Run` through its override, the frame pacing's, from the caller's frame with its return address; harness `race_loading`); the adapter stays (recompiled callers). |
| `sub_82247A60` | `LoadingScreen::Finish` | Overridden | r3 = which movie, or -1 for the current one; 3 or more does nothing: tail-calls `UiMovie::FinishNamed(movie, [0x8263799C + 4 × which])` (the animation `Show` played). From the race's states' loading waits and the front end's modes (14 callers). Read in full. Seen in play. r3 compared signed, as `Run`. Native under `--native_race_loading` (`src/race_loading/race_loading.cpp`: `UiMovie::FinishNamed` through its override; harness `race_loading`); the adapter stays (recompiled callers). |
| `sub_82290B30` | `UiMovie::SetFrames` (probably) | Native | r3 = a UI movie, r4, r5 = two values: walks the chunks of its live scene (from +16; each chunk +0 a tag, +4 its size) up to the `TRM` one, and sets +72 of each `TXT` chunk to r5 and of each `PRM` and `PIC` chunk to r4 (probably their frame). Leaf. From `LoadingScreen::Show` and `sub_82290FE0` (one of the other movies' users, `data-load.md`, never run). 34 lines. Read in full. Seen in play. Native (`src/race_loading/race_loading.cpp`, `SetFrames`: called by `LoadingScreen::Show`'s plain C++, under `--native_race_loading`; no adapter, `sub_82290FE0` never runs; harness `race_loading`, through the plain C++). |
