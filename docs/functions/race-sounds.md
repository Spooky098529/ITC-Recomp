# Known functions: the race's sounds

The **race's sounds**: one of the race mode's state machines (race mode
+ 138284, `RaceSounds::`, set up by `RaceSounds::Init` `sub_82177378`
from `RaceMode::Init`, `game-modes.md`; names here are ours, and which
state is which is guessed from what the handlers do, the state numbers
being the ones the race's other machines use). It runs the race's sound
beside the picture: at the set-up it allocates the scene models' sound
buffers (`0x826D8358`, `0x826D8374`, `scene-models.md` loads into them)
and makes the car sounds (`CarSounds::Init`, `car-sounds.md`); once the
race world has loaded (message 4) it loads the effects' wave bank
(asset 22889) into its own buffer, makes each car's two 24-byte wave
bank views, starts the race's first music and puts the 3D listener at
the origin; then every tick it gives each car its wave banks once both
its files and the effects' bank have arrived (`CarSound::Start`,
`CarSounds::SetBanks`), updates the cars' sounds (`CarSound::Update`),
pauses them while the system UI is up, and steps the music: the music
is bank 2 of the sound manager (`[0x826C5C80]`, `menu-sounds.md`), a
bank pair loaded by name (`Music::Play`, `mode-tick.md`) whose cue 0 is
the song; to change songs the handlers fade the "Music" category out
over ten ticks, stop cue 0, release the bank, wait for its slot to
empty, load the next one by name, and play cue 0 once it has loaded,
the volume put back from the one kept before the fade. The songs are
picked from a table of 13 names at `0x826193D4` (`../known-data.md`):
one at the start (by the race kind, random among five for some), one
by the race data's +32 or by the rival's item before a race against a
rival, one of two after the race (probably a win and a loss). Pause,
resume and stop walk the car sounds and the "Music" category; the
unload frees the buffers, the views, the car sounds and the music bank.
Read by the B7 loop's mapping agent, 2026-11-04, from the two leads
"the race's sound handlers" and "the readers of the scene models' sound
slots" (`../loops/leads.md`). The fields are in `../known-data.md`
("race sounds", `0x826D8334` to `0x826D83C8`, `0x826193D4`,
`0x82616BEC`).

The machine's words: +9604 the state (1 at set-up), +9608 the effects'
wave bank view (24 bytes, cleared by `RaceMode::RaceMode` with
`XPR::XPR`), +9632 the effects' bank buffer, +9636 (0 at the start, not
read here), +9640 1 while the car sounds are paused for the system UI,
+9644 the flags (below), +9648 the cars' sounds step (1 to set up, 2
running), +9652 the race kind (the race data's +0xA3AF0, copied by
`Init`: 0 the attract race, 1 to 3 and 6 the race HUD's kinds, 4
probably the split screen, 5 the alternate HUD's, as
`race-hud-machine.md` reads them), +9656 a count of ticks (10 after the
cars' set-up) before `UpdateCars` gives the cars their banks again.
The flags (+9644): 0x2 the music bank released, waiting for its slot to
empty; 0x4 load the next music; 0x10 the music's cue 0 started; 0x20
the music faded out and cue 0 stopped, waiting for it to end; 0x40 to
0x2000 which music comes next (`PickRivalMusic`, `PlayRivalMusic`);
0x4000 the cars' sounds stopped (no bank set-up); 0x8000 fade the music
out; 0x10000 cleared by `OnRollingStartDone` (never set here); 0x20000
the cars' sounds set up. The handlers get r3 = the machine (and r4 =
the race mode's dispatcher, unused); none posts a message.

The transitions `RaceSounds::Init` adds ((state, message) → next
state, handler; 0 = stay; handlers are functors of vtable `0x82011918`,
a member function on the machine, or `0x820069BC`, a state change
only): always (1, 4) → 38, or → 24 with kind 5, `Begin`; (38, 0)
`IdleTick`; (38, 29) → 24 `Pause`; (38, 31) → 24 `PauseCars`; (38,
49) → 37; (38, 44) → 32 `OnAbort`. With kind 1 only: (38, 14) → 10
`OnRollingStart`; (38, 26) → 23 `OnCountdown`; (23, 0) `RaceTick`;
(23, 34), (23, 36) → 30 and (23, 19) → 38 `OnRaceEnd`; (23, 29) → 25
`Pause`; (23, 31) and (23, 58) → 25 `PauseCars`. With kinds 2, 4 and
6: (38, 36) and (38, 34) → 30 `OnRaceEnd`. Always: (30, 0)
`AfterRaceTick`; (30, 40) → 38 `OnLeaveAfterRace`; (30, 49) → 37; (30,
44) → 32 `OnAbort`; (30, 42) → 17 `OnNextStage`; (30, 31) `PauseCars`;
(37, 0) `EndTick`; (37, 50) → 35 `Unload`; (24, 0) nothing
(`sub_82106540`); (24, 29) → 38 `Resume`; (24, 32) → 38
`ResumeCars`; (24, 44) → 32 `OnAbort`; (24, 49) → 37; (24, 14) → 10
`OnRollingStart`; (25, 0) nothing; (25, 29) → 23 `Resume`; (25, 32)
and (25, 58) → 23 `ResumeCars`; (25, 44) → 32 `OnAbort`; (25, 49) →
37; (25, 14) → 10 `OnRollingStart`; (32, 4) → 38 `OnAbortDone`. Then
(17, 0) `StageTick` and (17, 26) → 23 (kind 1) or → 38 (kinds 2, 4, 6)
`OnStageCountdown`. Always: (10, 0) `RollingStartTick`; (10, 10) → 23
`OnRollingStartDone`; (10, 31) `PauseCars`; (10, 32) `ResumeCars`. 32
transitions for the other kinds, 36 for kinds 2, 4 and 6, 43 for kind
1. Which messages these are, from the other machines' files: 4 the race
world loaded, 14 a rolling start, 26 the countdown, 29 probably the
pause (posted by the pad reader, `racer-controllers.md`), 34 the
replay's end, 49 the race's end, 50 the unload; 31 and 32 pause and
resume the car sounds alone (probably a pause screen that keeps the
music), 58 like 31; the rest not known. So: 38 before the countdown
(and the attract race), 23 racing against a rival (kind 1), 24 and 25
paused from 38 and 23, 30 after the race, 17 the next stage loading, 10
a rolling start, 32 aborted, 37 the end, 35 unloaded.

The music steps, the same in `IdleTick`, `RaceTick`, `AfterRaceTick`,
`StageTick` and `RollingStartTick` ("the music steps" below; they
differ only in which music the 0x4 step loads and whether the 0x20 step
also puts the effects' volume back): with 0x8000, `FadeMusic`, and
once it answers 1 (silent), the sound manager's slot 13
(`AudioManager::StopCue`) on bank 2's cue 0, 0x20 set and 0x8000
cleared (a 0 answer ends the tick); with 0x20, slot 14
(`IsCuePlaying`) on that cue: while it plays the tick ends, else slot
5 (`ReleaseBank`) on bank 2, `RestoreMusicVolume`, 0x2 set and 0x20
cleared; with 0x2, slot 8 (`IsBankSlotEmpty`) on bank 2: 0 ends the
tick, else 0x4 set and 0x2 cleared; with 0x4, the next music loaded
(`Music::Play` with r3 = &2, r5 = &(byte 0)) and 0x4 cleared; then slot
6 (`IsBankLoading`) on bank 2: while loading, or with 0x10 or 0x2 set,
the tick ends, else `Sound::Play` (`menu-sounds.md`) on bank 2's cue 0
and 0x10 set. The bank and cue indexes are words on the handler's
stack, passed by address. The slots are reached through the manager's
vtable (`0x82008388`; rows in `sound-cues.md`); `SoundOptions::SetEffectsVolume`
(`menu-sounds.md`) already calls slots 29 and 30 natively through the
function table (the user's call followed: the menu sounds, 2026-10-31).

Shared with other files, which keep their rows: `Sound::Play`
(`sub_82175850`, `menu-sounds.md`); `Archive::Load`'s callback for the
scene models' sound files (`sub_82175EE0`, `scene-models.md`); the car
sounds (`CarSounds::Init` `sub_8210FE00`, `StopAll` `sub_8210F7C8`,
`Release` `sub_8210F750`, `CarSound::PauseAll` `sub_8210E2B8`,
`ResumeAll` `sub_8210E350`, `Stop` `sub_8210E218`, `Start`
`sub_8210E030`, `Update` `sub_82110048`, `CarSounds::SetBanks`
`sub_8210CB48`, `WaveBankView::Init` `sub_8210C9D8`, `car-sounds.md`);
the sound manager's slots 5, 6, 8, 13, 14, 25 to 27 and 29 to 32
(`sound-cues.md`); `Music::Play` (`sub_821063B0`, `mode-tick.md`);
`StateMachine::Add` (`sub_82106158`, `game-modes.md`) and the empty
`sub_82106540` (`audio.md`); `Archive::Toc` (`sub_8209D868`,
`archive.md`) and `Archive::Load` (`sub_8209DE48`,
`files-and-saves.md`); `XPhysicalAlloc`, `XPhysicalFree`, `operator
new` (`sub_822964C8`, `sub_822965C8`, `sub_825A6880`, `heap.md`);
`XPR::XPR` (`sub_82596E68`, `models.md`); `memset` and `rand`
(`sub_825A71D0`, `sub_825A9BB8`, `c-runtime.md`); the 24-byte holders'
delete `sub_82117B30` (`sound-cues.md`); `Vehicle::Get`,
`RacerRecords::At`, `SaveData::Of` (`sub_820C0468`, `sub_820FFE58`,
`sub_82105620`, `vehicle-accessors.md`); `Race::RacerCount`
(`sub_821705F8`, `collision-world.md`); `DataTables::Item`
(`sub_82117178`, `data-tables.md`).

`rand` (`sub_825A9BB8`), which blocked `PlayStartMusic` and through it
`Begin` and `IdleTick`, is native by the user's call (2026-11-04,
`c-runtime.md`): the same seed word in the thread's guest C runtime
data, found as `_getptd` does.
`Init` is 1,680 generated lines (a 3,280-byte frame, the functors built
on the stack and copied as `RaceHudMachine::Init`'s are: stack
leftovers copied as the original leaves them).

Made native by the B7 loop's migration (2026-11-04) under
`--native_race_sounds` (`itc_recomp/src/race_sounds/`, adapters in
`src/hooks/race_sounds.cpp`, harness `race_sounds`): the plain C++ writes
the originals' frames (back chain, locals) and makes its guest calls
through `race_sounds::Calls` with the registers the original sets (the
others left as the previous call leaves them): the car sounds through
their register steps' plain C++, the sound manager's slots (the user's
call for the menu sounds, 2026-10-31) and the other callees through the
function table. The harness points the manager at a fake one whose slots
are leaf recorders and the car sounds' array at seven car sounds whose
holders hold fake voices (as the car sounds' harness builds them).
Where a native step stands in for a call the original makes and that
call leaves registers a later call reads unset (`CarSounds::SetBanks`'
r4 and r5, seen by the harness's recorders through `CarSound::Stop`'s
XAudio calls), the plain C++ passes them on (`Calls::Leave`).
`CarSound::Start` makes XAudio's own voices: the harness's cases for
`SetUpCars` and `UpdateCars` hold the mixer's lock and free every voice
made, as the car sounds' harness's real cases do (empty wave banks, so
the voices are made over no waves).

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
| `sub_82177378` | `RaceSounds::Init` | | r3 = the state machine (race mode + 138284). +9652 = the race kind (`[[0x826C5B2C] + 0xA3AF0]`); for each of the seven slots i: `0x826D83AC + 4i`, `0x826D8390 + 4i` = 0, byte `0x826D8350 + i` = 0, `0x826D8334 + 4i` = i (the load callback's context), then `0x826D8374 + 4i` = `XPhysicalAlloc(0x6B080, -1, 2048, 4)` and `0x826D8358 + 4i` = `XPhysicalAlloc(0x5D2D0, -1, 2048, 4)`, each cleared (`memset`; no check for 0); `CarSounds::Init` (`car-sounds.md`); +9604 = 1; then the transitions in the intro through `StateMachine::Add`, each a 16-byte key (state, message, 0, 0) and a functor's six words copied in the 3,280-byte frame (storage +0 the vtable, +4 the machine, +8 the member function; the other words the frame's) with the next state at +28, the kind deciding which are added (1; 2, 4 and 6; 5 only changes the first's next state). From `RaceMode::Init` (`game-modes.md`), once a race. 1,680 generated lines. Read in full. Seen in play. |
| `sub_82176618` | `RaceSounds::Begin` | Overridden | (1, 4) → 38 (24 with kind 5): +9644 = 0; `0x82616C38` = `0x82616C3C` (the effects' volume); +9640 = 0; +9632 = `XPhysicalAlloc(size, -1, 2048, 4)`, the size asset 22889's TOC entry's +8, or its +4 when that's 0 (`Archive::Toc`, the entry at +0x596A0); `0x826D83C8` = 0, +9636 = 0, +9656 = 0; `Archive::Load(22889, +9632, OnEffectsBankLoaded, 0, 0, 0)` (the effects' wave bank, probably); for each of the seven cars two 24-byte objects (`operator new`, `XPR::XPR`, or 0 when the allocation fails; then `memset` 24 either way) into `0x826D82FC + 8i` and `0x826D8300 + 8i` (the car's two wave bank views); `PlayStartMusic`; `SetListener` with the position (0, 0, 0), front (1, 0, 0), top (0, 1, 0) and velocity (0, 0, 0) (stack vectors of four floats, the fourth 1.0); +9648 = 1. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`: `Archive::Toc` as plain C++, `XPhysicalAlloc`, `Archive::Load`, `operator new` and `XPR::XPR` through the function table (their overrides), `memset` inlined, `PlayStartMusic` and `SetListener` their plain C++; harness `race_sounds`, its allocations real and freed, `Archive::Load` recorded); the adapter stays (reached through the machine's functors). Seen in the attract race. |
| `sub_82175ED0` | `RaceSounds::OnEffectsBankLoaded` | Overridden | `Archive::Load`'s callback for `Begin`'s load (no direct callers; its address formed in `Begin`): `0x826D83C8` = 1. Leaf. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, harness `race_sounds`); the adapter stays (`Archive::Load` calls it through its pointer). |
| `sub_821758A0` | `RaceSounds::SetListener` | Overridden | r3, r4, r5, r6 = four vectors' addresses: the sound manager's slot 26 (`SetListenerOrientation`) with r4 and r5 (front and top), slot 25 (`SetListenerPosition`) with r3, slot 27 (`SetListenerVelocity`) with r6. From `Begin`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, harness `race_sounds`); the adapter stays while `Begin` is recompiled. |
| `sub_82175DB0` | `RaceSounds::PlayStartMusic` | Overridden | r3 = the machine: by the race kind (+9652), kinds 1 and 3 (and any outside 1 to 6, the attract race's 0 among them) `bgm_tmp_06` (the table at `0x826193D4`, entry 0), kinds 2, 4, 5 and 6 one of entries 1 to 5 (`bgm_tmp_07`, `bgm_tmp_01`, `bgm_tmp_15`, `bgm_tmp_05`, `bgm_tmp_10`) by `rand() % 5` (`c-runtime.md`; the indexes 1 to 5 from a stack array); `Music::Play` with r3 = &2 (a stack word), r5 = &(a stack byte 0). From `Begin` and `IdleTick`'s 0x4 step. Was blocked by `rand` until the user's call (2026-11-04). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; `rand` its plain C++, `c_runtime::Rand`, the user's call, 2026-11-04; `Music::Play` through its register step; harness `race_sounds`, the thread's seed set per case). |
| `sub_821767A8` | `RaceSounds::IdleTick` | Overridden | (38, 0): with +9648 1, `SetUpCars` and nothing else; with 2, `UpdateCars`, then the music steps (intro), the 0x4 step's music `PlayStartMusic`; with any other value nothing. Read in full. Seen in play (probably the attract race's, which starts in 38 with kind 0). Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`, as `RaceTick` with `PlayStartMusic`'s plain C++; harness `race_sounds`); the adapter stays (reached through the machine's functors). Seen in the attract race. |
| `sub_82176188` | `RaceSounds::SetUpCars` | Overridden | r3 = the machine: +9656 = 10; for each of the seven car sounds i (`[0x826C5C3C]` + 1,264 i, or 0 when the array is 0) while the effects' bank has arrived (`0x826D83C8`) and the car's two files have (`0x826D83AC + 4i`, `scene-models.md`): `CarSound::Stop`; `WaveBankView::Init(+9608, +9632)` (the effects' view, again for each car); the car's two views (`0x826D82FC + 8i`, `+ 4`) cleared (`memset` 24) and set up, the second over `0x826D8374 + 4i`, the first over `0x826D8358 + 4i`; `CarSound::Start(car, &views, +9608)`; `CarSounds::SetBanks(car, [0x826C5C48 + 4i], [0x826C5C64 + 4i])`; the car's +400 = `Vehicle::Get(i)`, +1252 = 1.0; `0x826D8390 + 4i` = 0; +9648 = 2; +9644 \|= 0x20000. A compare of `[0x826C5C48 + 4i]` - 18 with 26 has no branch after it (dead). From `IdleTick`, `RaceTick`, `OnRollingStart` (with +9648 1). Read in full. Seen in play. Correction (migration): +9648 = 2 and the 0x20000 flag are set inside the loop, for each car set up, so not at all when none is. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_cars.cpp`; `WaveBankView::Init`, `CarSounds::SetBanks` and `Vehicle::Get` as plain C++, `memset` inlined, `CarSound::Stop` and `Start` through their register steps, `SetBanks`' r4 and r5 passed to the next call as the original's leftovers; harness `race_sounds`, with XAudio's real voices made over empty wave banks and freed, as the car sounds' harness does); the adapter stays while `IdleTick`, `RaceTick` and `OnRollingStart` are recompiled. |
| `sub_82175F20` | `RaceSounds::UpdateCars` | Overridden | r3 = the machine. While +9656 > 0, it goes down by one and nothing else; else nothing with +9644's 0x4000 set or 0x20000 clear. For each racer n below `Race::RacerCount` (its entry `[[0x826D82F4] + 2308 + 4n]`, v its +20, the vehicle's index): when `0x826D8390 + 4v` and `0x826D83AC + 4v` are set (asked again and both files arrived), the car sound v as in `SetUpCars` (`CarSound::Stop`, the two views cleared and set up over the slot's buffers, `CarSound::Start` with +9608, `CarSounds::SetBanks` with `[0x826C5C48 + 4v]` and `[0x826C5C64 + 4v]`, +400 = `Vehicle::Get(v)`, +1252 = 1.0, `0x826D8390 + 4v` = 0; the effects' view not set up again; the same dead compare). Then `CarSound::Update` on each racer's car sound whose `0x826D8390 + 4v` is 0 (`Race::RacerCount` called again each step of both loops). Then the system UI's words (`0x826C7960`, `0x826C7964`, `../known-data.md`): either set with +9640 0, `PauseCars` and +9640 = 1; both clear with +9640 set, `ResumeCars` and +9640 = 0. Uses a 240-byte frame and f31. From `IdleTick`, `RaceTick`, `OnRaceEnd`, `AfterRaceTick`, `OnLeaveAfterRace`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_cars.cpp`, as `SetUpCars`; `Race::RacerCount` as plain C++, `PauseCars` and `ResumeCars` their plain C++; harness `race_sounds`, with real voices; its `CarSound::Update` calls do nothing there, with no vehicles, and are seen in the attract race); the adapter stays while its callers are recompiled. |
| `sub_82175908` | `RaceSounds::FadeMusic` | Overridden | The sound manager's slot 29 (`GetCategory`) with "Music" (`0x82008348`) into a stack word; when `0x82616C34` (the music volume kept before the fade) is 0.0 (`[0x82000A34]`), returns 1 and nothing else; else `0x82616BEC` (the music volume) -= `0x82616C34` × 0.1 (`[0x82001FBC]`, one `fnmsubs`): above 0.0, slot 30 (`SetCategoryVolume`, r4 = the category's address, r5 = `0x82616BEC`) and 0 returned; else `0x82616BEC` = 0.0, slot 30, and 1 returned. So ten calls fade it out. From the music steps (`IdleTick`, `RaceTick`, `AfterRaceTick`, `StageTick`, `RollingStartTick`). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, its guest calls through `race_sounds::Calls`: the car sounds' register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_821759C8` | `RaceSounds::RestoreMusicVolume` | Overridden | Slot 29 with "Music", then `0x82616BEC` = `0x82616C34` and slot 30 with it. From the music steps' 0x20 step. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, its guest calls through `race_sounds::Calls`: the car sounds' register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82176968` | `RaceSounds::OnCountdown` | Overridden | (38, 26) → 23 (kind 1): `CarSound::Update` on the first car sound (`[0x826C5C3C]` itself, not checked for 0); the music faded: `0x82616C34` = `0x82616BEC`, byte `0x826C5C84` = 0, +9644 0x10 cleared and 0x8000 set; `PickRivalMusic`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_821764F0` | `RaceSounds::PickRivalMusic` | Native | r3 = the machine: from the race's progress (`[0x826D82E8]`), the results' count c (+2184) and the last entry's item (+2188 + 972 (c - 1) + 4): with c 1 or the item -1, 0x40; else the item's byte +35 (`DataTables::Item`, signed): 1 0x100; 2 0x80; 3 by the item: 383 to 390 0x400, 392 to 397 0x800, else 0x200; 4: 391 0x800, 398 0x1000, else 0x400; 5 0x2000; 0, negative or above 5 0x40; or'd into +9644 (`PlayRivalMusic` plays it). The item is probably the rival's, its byte +35 its class. From `OnCountdown`, `OnNextStage`, `OnRollingStart`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_cars.cpp`, `DataTables::Item` as plain C++; harness `race_sounds`); its adapter removed: its callers (`OnCountdown`, `OnNextStage`, `OnRollingStart` / `RaceTick`, `StageTick`, `RollingStartTick`) call its plain C++, and the flag now switches only them. |
| `sub_821769D0` | `RaceSounds::RaceTick` | Overridden | (23, 0): as `IdleTick` (+9648 1 `SetUpCars`, 2 `UpdateCars` and the music steps), the 0x20 step also putting the effects' volume back (`0x82616C38` = `0x82616C3C`) and the 0x4 step's music `PlayRivalMusic`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_821762F8` | `RaceSounds::PlayRivalMusic` | Native | r3 = the machine: each of +9644's music flags set is played (`Music::Play` with &2 and &(byte 0)) and cleared, in this order: 0x40 by the race data's +32 (`[[0x826C5B2C] + 32]`, probably the course: 0 `bgm_tmp_07`, 1 `bgm_tmp_01`, 2 `bgm_tmp_15`, 3 or more none, the flag cleared all the same), 0x100 `bgm_tmp_10`, 0x80 `bgm_tmp_05`, 0x200 `bgm_tmp_03`, 0x400 `bgm_tmp_09`, 0x800 `bgm_tmp_17`, 0x1000 `bgm_tmp_11`, 0x2000 `bgm_tmp_16` (entries 1 to 10 of `0x826193D4`); only one is set in play. From `RaceTick`, `StageTick`, `RollingStartTick`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_cars.cpp`, `Music::Play` through its register step; harness `race_sounds`); its adapter removed: its callers (`OnCountdown`, `OnNextStage`, `OnRollingStart` / `RaceTick`, `StageTick`, `RollingStartTick`) call its plain C++, and the flag now switches only them. |
| `sub_82176BA0` | `RaceSounds::OnRaceEnd` | Overridden | (23, 34), (23, 36) → 30, (23, 19) → 38 (kind 1); (38, 36), (38, 34) → 30 (kinds 2, 4, 6): `0x82616C38` (the effects' volume) ×= 0.0 (`[0x82000A34]`); `UpdateCars`; the music faded (`0x82616C34` = `0x82616BEC`, byte `0x826C5C84` = 0, 0x10 cleared, 0x8000 set). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82176C10` | `RaceSounds::AfterRaceTick` | Overridden | (30, 0): `UpdateCars` and the music steps (always, with no +9648 test), the 0x4 step's music by the last racer record (`RacerRecords::At(race data + 0x59AE8, -1)`, its +3924, probably the result): kind 6 with the race data's +0xA3AF4 7 `bgm_tmp_14`; kind 6 otherwise by the word `0x826C7824` (`../known-data.md`): set, +3924 1 gives `bgm_tmp_14`; clear, +3924 2 does; any other kind, +3924 1; else `bgm_tmp_08` (entries 11 and 12 of `0x826193D4`; probably a win and a loss). Its last step also skips `Sound::Play` (cue 0) while player 0's save data (`SaveData::Of(0)`) +6100 is 1 (played when it's above 999 or not 1). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). `RacerRecords::At` and `SaveData::Of` as plain C++; the harness's race data in a second block (its records' count at +0x59AE8 + 0x49FE0). |
| `sub_82176E60` | `RaceSounds::OnLeaveAfterRace` | Overridden | (30, 40) → 38: `UpdateCars`; the music faded (as `OnRaceEnd`); `0x82616C38` = `0x82616C3C` (the effects' volume back). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82176EC8` | `RaceSounds::OnNextStage` | Overridden | (30, 42) → 17: `CarSounds::StopAll`; the music faded with 0x4000 too (`0x82616C34` = `0x82616BEC`, byte `0x826C5C84` = 0, 0x10 cleared, 0xC000 set); `PickRivalMusic`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82176F28` | `RaceSounds::StageTick` | Overridden | (17, 0): the music steps only (no car sounds), the 0x4 step's music `PlayRivalMusic`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82175D20` | `RaceSounds::OnStageCountdown` | Overridden | (17, 26) → 23 (kind 1) or 38 (kinds 2, 4, 6): `0x82616C38` = `0x82616C3C`; +9648 = 1 (the cars' sounds set up again); +9644's 0x4000 cleared. Leaf. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, its guest calls through `race_sounds::Calls`: the car sounds' register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_821770C0` | `RaceSounds::OnRollingStart` | Overridden | (38, 14), (24, 14), (25, 14) → 10: with +9648 1, `SetUpCars`; the music faded (as `OnCountdown`); `PickRivalMusic`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82177130` | `RaceSounds::RollingStartTick` | Overridden | (10, 0): the music steps only, the 0x20 step putting the effects' volume back and the 0x4 step's music `PlayRivalMusic`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds_ticks.cpp`; its callees in this file their plain C++, `Music::Play`, `Sound::Play` and the car sounds through their register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82175D48` | `RaceSounds::OnRollingStartDone` | Overridden | (10, 10) → 23: with +9644's 0x20000 set and 0x4000 clear, `CarSounds::StopAll`, 0x4000 set and 0x10000 cleared; then +9648 = 1 and 0x4000 cleared. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, its guest calls through `race_sounds::Calls`: the car sounds' register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82175AE8` | `RaceSounds::Pause` | Overridden | (38, 29) → 24, (23, 29) → 25: `CarSound::PauseAll` on each of the seven car sounds (on 0 each time when the array is 0), then slot 29 with "Music" and slot 31 (`PauseCategory`) with it. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, its guest calls through `race_sounds::Calls`: the car sounds' register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82175B80` | `RaceSounds::Resume` | Overridden | (24, 29) → 38, (25, 29) → 23: `CarSound::ResumeAll` seven times on the first car sound (`[0x826C5C3C]`, the pointer not advanced, as the original), then slot 29 with "Music" and slot 32 (`ResumeCategory`). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, its guest calls through `race_sounds::Calls`: the car sounds' register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_82175C00` | `RaceSounds::PauseCars` | Overridden | (38, 31) → 24, (23, 31) and (23, 58) → 25, (30, 31), (10, 31): `CarSound::PauseAll` on each of the seven (on 0 when the array is 0). Also from `UpdateCars` (the system UI). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, harness `race_sounds`); the adapter stays while `UpdateCars` is recompiled (and for the machine's functors). |
| `sub_82175C58` | `RaceSounds::ResumeCars` | Overridden | (24, 32) → 38, (25, 32) and (25, 58) → 23, (10, 32): `CarSound::ResumeAll` seven times on the first car sound (not advanced, as `Resume`). Also from `UpdateCars`. Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, harness `race_sounds`); the adapter stays while `UpdateCars` is recompiled (and for the machine's functors). |
| `sub_82175CA0` | `RaceSounds::OnAbort` | | (38, 44), (30, 44), (24, 44), (25, 44) → 32: `CarSounds::StopAll`; the music faded with 0x4000 (as `OnNextStage`). Read in full. Not seen in play. |
| `sub_82175CF8` | `RaceSounds::OnAbortDone` | | (32, 4) → 38: +9648 = 1, +9644's 0x4000 cleared, `0x82616C38` = `0x82616C3C`. Leaf. Read in full. Not seen in play. |
| `sub_82175A40` | `RaceSounds::EndTick` | Overridden | (37, 0): once (while +9644's 0x20 is clear): slot 13 (`StopCue`) on bank 2's cue 0, 0x20 set, then `CarSound::PauseAll` on each of the seven (on 0 when the array is 0). Read in full. Seen in play. Native under `--native_race_sounds` (`src/race_sounds/race_sounds.cpp`, its guest calls through `race_sounds::Calls`: the car sounds' register steps, the manager's slots through the function table; harness `race_sounds`); the adapter stays (reached through the machine's functors). |
| `sub_821772D8` | `RaceSounds::Unload` | | (37, 50) → 35: `CarSounds::Release`; for each of the seven slots `XPhysicalFree` on `0x826D8374 + 4i` and `0x826D8358 + 4i` (the pointers left), and `sub_82117B30` on `0x826D82FC + 8i` and `0x826D8300 + 8i` (each view deleted and its pointer cleared); slot 5 (`ReleaseBank`) on bank 2; `XPhysicalFree(+9632)`. Read in full. Seen in play. |
