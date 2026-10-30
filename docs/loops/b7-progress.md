# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-30 01:06 on `b7-loop-29-10-26-2` (the user's
  call, 2026-10-30: this run stays on the last run's branch), from
  92ffb30; summary `b7-loop-2026-10-30-0106.md`.
- Limit for this run: 3 subsystems; 1 taken.
- First, a cleanup lead without mapping (not counted), the last run's
  stop: `CarSound::Part840Update`'s harness (`sub_8210ECF0`,
  `car-sounds.md`), 1 of 80 cases differing once in `itc_2559`
  (intermittent). The precedent: this run's and the 2026-10-24 21:33
  run's first leads were the previous stop's harness. Migration
  checked (d5cfd5c: the mixer swaps XAudio's two source-voice stage
  lists between a case's versions; the real-voice cases compare them
  by role, as the XAudio harnesses' `LockWords`; 0 of 80 over 5 runs,
  `itc_2569`-`itc_2573`; nothing else ready in `car-sounds.md`).
  Validated (`itc_2574`, `itc_2575`): 56 of 56 harness lines, every
  harness 0 outside the expected groups; `CarSounds::StopAll` already
  on the drive list.
- Subsystem 1 of 3: the app's start-up (`sub_82106548`, the top of
  `leads.md`). Mapped (d5647db, `app-start-up.md`, 30 rows, 18 ready
  groups); the map's stop answered: the voice engine (XHV, `Voice::Init` `sub_8257B7A8` under the network manager's `sub_8214F6E8`): not made; its users seen in play (`sub_8214FA78` every frame, the UI's calls through the engine's +52, the release `sub_8214F690`) skipped natively with what they'd read with no headset (no talkers, no data), Status Dropped (the user's call, 2026-10-30, "Map Xbox features, don't port"). Migration 1
  checked (c9aed90..6292712: the voice skip, the start-up's leaves, the
  network manager's set-up, the scene target, `Billboards::Init`, 5
  adapters removed; `--native_app_start_up`; replay ok, `itc_2577`).
  Validated (`itc_2594`): 15 of 15 harness lines, all ran in play.
  Migration 2 checked (427db89..a63717c: `App::StartUp` with the text
  and font set-up, `WordVector::Fill`, `XAudioInitialize`, 6 adapters
  removed). Its stop (`App::App` waits on the movie wrapper's critical
  section, `MovieWrapper::MovieWrapper` `sub_82593DC0` +4 through
  `InitializeCriticalSection`) is covered by a precedent ("Precedent
  first": a kernel critical section a subsystem's code locks becomes a
  host `std::mutex` for that lock, taken by every user, as
  `Edram::Alloc` and the memory blocks' lock; the generic wrapper stays
  recompiled for the others): migration 3 takes it so. Validated
  (`itc_2616`): 15 of 15 harness lines, all ran in play; a print added
  (b95341e). Migration 3 checked (6e8e860..e4104fe: the movie
  wrapper's lock a host recursive mutex as the memory blocks' lock,
  `Movie::Movie`, `AudioManager::Create`, the second camera class,
  `App::App`, `App::Create` and its getters; nothing ready left).
  Validated (`itc_2629`, `itc_2630`): 17 of 17 harness lines; three to
  drive. Subsystem 1 finished.
- Subsystem 2 of 3: the game's data load at boot (`sub_82105868`,
  `sub_82106618`, the top of `leads.md`; 10 ready groups under it).
  Mapping agent running.

## Done

- The app's start-up (subsystem 1): `app-start-up.md`, every ready
  group, three migrations, the voice engine Dropped (the user's call);
  `--native_app_start_up`; c9aed90..e4104fe. Validated (`itc_2594`,
  `itc_2616`, `itc_2629`, `itc_2630`).
- The last stop's `CarSound::Part840Update` harness (cleanup lead, not
  counted): d5cfd5c, a harness fix only (the stage lists compared by
  role); validated (`itc_2574`, `itc_2575`).

## Rows to review

`review.md`, "Run 2026-10-30 01:06".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3031, work left 3287 (wrapped 104, known 501,
  unknown 2682).
- After the cleanup's migration (`itc_2573`: race 38 s, 0 errors,
  car_sounds 0 differing): ours 3031, work left 3287.
- After its validation (`itc_2574`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 3031, work left 3287.
- After the start-up's migration 1 (`itc_2593`: race 38 s, 0 errors,
  touched harnesses 0 differing): ours 3064, work left 3256 (wrapped
  104, known 495, unknown 2657).
- After its validation (`itc_2594`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 3064, work left 3256.
- After the start-up's migration 2 (`itc_2615`: race 38 s, 0 errors,
  touched harnesses 0 differing): ours 3073, work left 3247 (wrapped
  104, known 486, unknown 2657).
- After its validation (`itc_2616`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 3073, work left 3247.
- After the start-up's migration 3 (`itc_2628`: race 38 s, 0 errors,
  touched harnesses 0 differing): ours 3090, work left 3230 (wrapped
  104, known 479, unknown 2647).
- After its validation (`itc_2629`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 3090, work left 3230.
