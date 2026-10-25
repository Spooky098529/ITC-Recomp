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
  call). Step: migration agent 1 running.

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
