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
  root; `XPR::XPR` under it blocks the race mode's constructor). Step:
  mapping agent running.

## Done

- The game modes (`game-modes.md`, mapped in 7871fe6; 31 rows): 21
  ours in one migration (21ff7d5..c504e59), 3 adapters removed, 2
  Merged; validated (`itc_1603`); `--native_game_modes`. Left: the race
  mode's constructor and destructor (`XPR::XPR`, its destructor,
  `RaceOverlays::Destroy`), `RaceMode::Init`'s other set-ups, the mode
  constructors, `Modes::Tick`.

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
- After the game modes' validation: race 27 s (`itc_1603`), 0 errors,
  every harness 0 differing.
- After the game modes' migration 1: ours 1923, work left 4528 (wrapped
  111, known 605, unknown 3812).
