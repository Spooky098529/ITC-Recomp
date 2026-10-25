# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-25 03:59 on `b7-loop-25-10-26-2` (the user's
  branch), from e20b6ef; summary `b7-loop-2026-10-25-0359.md`.
- Limit for this run: 5 subsystems (the user's call, 2026-10-25; 3 by default).
- Subsystem 1 of 5: `leads.md`'s top lead, `sub_8215E6C8` (a race
  object's set-up registering message handlers, with the owner's
  constructor and destructor `sub_821073B8`, `sub_82107518`). Mapped
  as the game modes (`game-modes.md`, 31 rows, 7871fe6; the first
  mapping agent was cut off by a crash before writing anything, and
  started again). Map checked (0 errors, 31 of 31 in play, 11 ready
  groups; nothing calls the kernel). Migration 1 done after four batches
  (21ff7d5..c504e59: the state machines, the overlays' machine and
  handlers, the race mode's members and 28-byte buffers; 21 of 31 ours,
  3 adapters removed, 2 Merged; `--native_game_modes`; `itc_1602`: 27
  s, 0 errors, every harness 0 differing). Nothing ready left; the rest
  waits on `XPR::XPR` (`models.md`), the race mode's set-ups, the mode
  constructors and `Modes::Tick`'s callees. Validated (`itc_1603`: 18 of
  18 harness lines, 12 ran in play; the replay broke at boot,
  `itc_1605`, so 6 go to `to-drive.md`). Finished.
- Subsystem 2 of 5: the model class lead (`Model::Model`,
  `Model::~Model`, the texture provider, the XPR's destructor:
  `sub_825948C0`, `sub_82594F58`, `sub_82594850`, `sub_825943E0`,
  `sub_82597148`), picked as closest to ready (ready groups under each
  root; `XPR::XPR` under it blocks the race mode's constructor). Mapped
  into `models.md` (16 rows, and `memmove_s` in `c-runtime.md`;
  f28c6f7). Map checked (0 errors, all in play, 5 ready groups); its
  `stop:` was the memory blocks' lock (`ScopedLock` over the critical
  section `0x8296BD38`, under `Memory::FreeLater` and
  `FreePhysicalLater`, which the destructors need). The user's call,
  2026-10-25: port the lock too (a host `std::mutex`, as `Edram::Alloc`
  got). Migration 1 done (6148c1e..b35547a: the put-off frees with the
  lock, the XPR, `memmove_s`, the texture provider, `Model::Model`,
  `Release`, `~Model`; all 17 rows ours (and `ScopedLock` overridden
  for `0x8296BD38` only), 3 adapters removed; `--native_models`;
  `itc_1613`: 27 s, 0 errors, every harness 0 differing). Nothing ready
  left in it; it makes the game modes' `sub_82106AF0` and
  `sub_821067A0` ready. Validated (`itc_1614`: 19 of 19 harness
  lines, 16 ran in play; the replay broke at boot again, `itc_1616`, so
  3 go to `to-drive.md`). Finished.
- Next: the game modes' cleanup (no mapping, not counted):
  `sub_821067A0`, `sub_82106AF0`, ready since the model class, and what
  they unblock. Migration done (ce8d477..4ff0b54: `XprHolder`, the
  race mode's member, `RaceMode::RaceMode`, `Mode::Create` with the
  mode constructors (11 new rows), 6 adapters removed; `game-modes.md`
  38 of 42 ours; 27 s, 0 errors, every harness 0
  differing). Nothing ready left in the file. The replay's boot marker
  fixed meanwhile (d83677c, the user's go-ahead: it waited for the movie
  library's draw, gone since the FFmpeg movies, so every replay since
  "stalled"). Validated (`itc_1624`: 21 of 21 harness lines; the
  replay `itc_1625` ran, the new marker's first test: front end, a race,
  the screen after it; 4 earlier functions seen and 2 from older runs,
  off `to-drive.md`; 5 mode types not entered go on it). Finished.
- Subsystem 3 of 5: `Modes::Tick`'s callees (`sub_82106868`,
  `sub_82106740`, `sub_822934B8`, `sub_8209EEC8`, `sub_8209BD18`,
  `sub_8209D130`, `sub_8209E7E8`, `sub_8209F318`, `sub_821063B0`:
  probably the loading screen or the mode change's loads), picked as the
  closest to ready (7 of its 9 roots ready, 37 functions left). Mapped
  as the mode tick (`mode-tick.md`, 23 rows, and `Modes::Modes` in
  `game-modes.md`; 2a11eec): the per-mode settings, the sign-in watch,
  the billboard ring flip, the screen fades, the menu music. Map checked
  (0 errors, 23 of 23 in play, 13 ready groups; presence Dropped by B5's
  call). Migration 1 done after five batches (4975324..1897994: the
  ring flip, the fades, the per-mode settings, the sign-in watch, the
  menu music with `lstrcmpA` and `CompareStringA`; 21 of 23 ours,
  presence Dropped, `Modes::Modes` too; `--native_mode_tick`;
  `itc_1638`: 26 s, 0 errors, every harness 0 differing). Nothing ready
  left; the rest waits on the save data's creation and deletion (a lead)
  and `atexit`. Validated (`itc_1639`: 17 of 17 harness lines, all ran
  in the attract race; the replay `itc_1640` ok, showing the overlays'
  four handlers, off `to-drive.md`). Finished.
- Subsystem 4 of 5: the minimap's parts (`sub_820F2D28`, `sub_820F2EF8`,
  `sub_820F4888`, `sub_820F4BA8`, `sub_820F51A8`, `sub_820F5238`,
  `sub_820F4040`, `sub_820F40C0`), picked as the closest to ready (6 of
  its 8 roots ready, 31 functions left). Mapped (`minimap.md`, 15
  rows, 1 moved; f86ea54): `Minimap::Draw` (the map's models, point
  markers, car markers in five colors, the flashing by a blink phase;
  Wrapped by `--per_tick_minimap_blink`, to keep) and its parts. Map
  checked (0 errors, 15 of 15 in play, 9 ready groups). Migration 1
  done (3d8e197, 20d9037: the race manager's lookups, the marker type,
  scaling, triangles, the 80-slot lists, the transform and the
  triangles' records; 12 of 15 ours; `--native_minimap`; interpolation's
  `BlendState_OnMultiply`/`OnCopy` kept, `Minimap::Draw`'s wrap
  untouched; `itc_1645`: 27 s, 0 errors, every harness 0 differing; the
  minimap's frame dump pixel-identical with the fallback). Nothing ready
  left; the rest waits on `atexit` and the course set's lists.
  Validated (`itc_1648`: 12 of 12 harness lines, 10 in the attract
  race, 1 more in the replay `itc_1649`, ok; `SetList::Point` to
  `to-drive.md`). Finished.
- Subsystem 5 of 5 (the user's go-ahead, 2026-10-25; no mapping, but
  counted, the user's call): `atexit` and `_onexit` native with the exit lock (lock 8) a
  host mutex by the recorded lock call, then the callers it unblocks
  (the minimap's marker draws and `Minimap::Draw`, the viewports,
  `UiMovie::TrackProperty`, `SignInWatch::StaticInit`, the static
  set-ups); `leads.md`'s top line, taken. Migration 1 done after five
  batches (19f9deb..1244dd3: `atexit` and `_onexit`, the exit lock a
  host `std::recursive_mutex` taken by every user through `_lock` and
  `_unlock` overridden for lock 8 only; the global strings' static
  set-ups, `SignInWatch::StaticInit`, `UiMovie::TrackProperty`, the
  minimap's marker draws, `Viewports::Init` and `Viewport::Init`; 1
  adapter removed; 27 s, 0 errors, every harness 0
  differing). Left ready: `sub_825A0548` (TrackProperty's caller) and
  `sub_825A6F00` (the exit table's set-up). Validated (`itc_1665`: 13
  of 13 harness lines, 10 in the attract race; replay `itc_1666` ok; 3
  to `to-drive.md`). Step: migration agent 2 running (those two). The run ends after its part 2 (the callers) is validated
  (the user's call, 2026-10-25).

## Done

- The game modes (`game-modes.md`, mapped in 7871fe6; 31 rows): 21
  ours in one migration (21ff7d5..c504e59), 3 adapters removed, 2
  Merged; validated (`itc_1603`); `--native_game_modes`. Left: the race
  mode's constructor and destructor (`XPR::XPR`, its destructor,
  `RaceOverlays::Destroy`), `RaceMode::Init`'s other set-ups, the mode
  constructors, `Modes::Tick`.
- The model class (`models.md` rows of f28c6f7; 17 rows, and
  `memmove_s` in `c-runtime.md`): all 17 ours in one migration
  (6148c1e..b35547a), 3 adapters removed; the memory blocks' lock a host
  lock (the user's call); validated (`itc_1614`); `--native_models`.
- The game modes' cleanup (no mapping; ce8d477..4ff0b54): 38 of 42
  ours, 6 more adapters removed, 11 rows added; validated (`itc_1624`,
  replay `itc_1625`).
- The mode tick (`mode-tick.md`, mapped in 2a11eec; 23 rows): 21 ours
  in one migration (4975324..1897994), presence Dropped, 1 adapter
  removed, `Modes::Modes` too; validated (`itc_1639`, `itc_1640`);
  `--native_mode_tick`. Left: `SignInWatch::OnModeChange` (the save
  data), `SignInWatch::StaticInit` (`atexit`).
- The minimap (`minimap.md`, mapped in f86ea54; 15 rows): 12 ours in one
  migration (3d8e197, 20d9037); validated (`itc_1648`, `itc_1649`);
  `--native_minimap`. Left: the marker draws and `Minimap::Draw`
  (`atexit`, the course set's lists).

## Rows to review

This run's rows: `review.md` ("Run 2026-10-25 03:59").

## Expected to differ

This run's groups: `expected-differences.md` (the "03:59 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1903, work left 4548 (wrapped 111, known 597, unknown 3840).
- After the game modes' map: ours 1903, work left 4548 (wrapped 111,
  known 625, unknown 3812).
- After the atexit step's validation: race 27 s (`itc_1665`), 0
  errors, every harness 0 differing; replay 127 s (`itc_1666`).
- After the atexit step's migration 1: ours 2010, work left 4441
  (wrapped 111, known 591, unknown 3739).
- After the minimap's validation: race 27 s (`itc_1648`), 0 errors,
  every harness 0 differing; replay 127 s (`itc_1649`), 0 errors.
- After the minimap's migration 1: ours 1995, work left 4456 (wrapped
  111, known 599, unknown 3746).
- After the minimap's map: ours 1983, work left 4468 (wrapped 111,
  known 611, unknown 3746).
- After the mode tick's validation: race 27 s (`itc_1639`), 0 errors,
  every harness 0 differing; replay 127 s (`itc_1640`), 0 errors.
- After the mode tick's migration 1: ours 1983, work left 4468 (wrapped
  111, known 597, unknown 3760).
- After the mode tick's map: ours 1961, work left 4490 (wrapped 111,
  known 619, unknown 3760).
- After the game modes' cleanup's validation: race 27 s (`itc_1624`),
  0 errors, every harness 0 differing; replay 127 s (`itc_1625`), 0
  errors.
- After the game modes' cleanup: ours 1961, work left 4490 (wrapped
  111, known 595, unknown 3784).
- After the model class's validation: race 27 s (`itc_1614`), 0 errors,
  every harness 0 differing.
- After the model class's migration 1: ours 1944, work left 4507
  (wrapped 111, known 601, unknown 3795).
- After the model class's map: ours 1923, work left 4528 (wrapped 111,
  known 622, unknown 3795).
- After the game modes' validation: race 27 s (`itc_1603`), 0 errors,
  every harness 0 differing.
- After the game modes' migration 1: ours 1923, work left 4528 (wrapped
  111, known 605, unknown 3812).
