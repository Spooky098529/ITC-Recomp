# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-07 22:22 on `b7-loop-07-11-26-2` (made by the
  user for this run), from 565fde58; summary `b7-loop-2026-11-07-2222.md`.
  Limit: 3 subsystems.
- Cleanup lead (no mapping, not counted): `sub_820C8DE8` (the second
  set's fill under `StandScene::Create`, `car-models.md`), ready, with
  what it unblocks in `type6-type7-modes.md` (`StandScene::Create`
  `sub_82180B00`, the modes' slot 0 `sub_82236880`, `sub_82246C90`);
  its line deleted from `leads.md`. Migration c709d9ed..92169ce4
  (`CarScene::SetUp`, `StandScene::Create`, `Type6Mode::Init`,
  `Type7Mode::Init`; 15 adapters removed; race 33 s, 0 errors, its
  harnesses 0 differing outside one new expected group,
  `Type6Mode::Init`'s transitions' stack leftovers; no subagents);
  left: none. Validated `itc_4432` (race 78 s at 180, the harnesses
  ~88.6 s; 0 errors, every harness 0 differing outside the expected
  group, 19 of 19 harness lines; the two modes' Inits' shared print
  split, 4ec19e07; 1 in the race, 2 in the replay `itc_4433` (ok), 16
  to drive). Recorded.
- Subsystem 1 of 3: the options screen with the settings' load (two
  leads taken together: `sub_821BB9A8` and the 10 others, the
  `Type6Options`/`Type7Options` transition handlers, the most ready
  groups in `leads.md`, 29, beside the type 6 and 7 modes; and
  `sub_821BBF00` and the 4 others, the options screens' volume calls
  over the same settings `[0x826D8478]`; both lines deleted from
  `leads.md`). Map checked (bce411a9: `options-screen.md`, 155 rows,
  1 moved from `functions.md` (`sub_821BE5A8`, Wrapped), 95 in play:
  the machine's handlers, `Settings::Reset`, `Read`, `Write`,
  `Reload`, `Draw`'s 15 pages, `Update`'s 49 steps, the music preview,
  the two-player pages, the type 2, 4 and 8 options' and controller
  pages' Inits; 0 errors; 66 ready groups; 1 lead added (the options'
  save side); three subagents, one at a time). Migration
  a9b2152f..e81aa1ef (`--native_options_screen`, harness
  `options_screen`: the controls' tables, `Settings::Reset`, `Read`,
  `Write`, the handlers, `Close`, `Leave`, `ClosePage`, `Wait`, the
  steps on its movie, the node helpers; 34 functions, 4 tails Merged, 5
  adapters removed; `Options::FadeOut` `sub_821BD680` left recompiled by
  the `BackToBuilds` precedent; races 35-38 s, 0 errors, its harnesses
  0 differing; no subagents); left: 37 ready groups. Validated
  `itc_4443` (race 83 s at 180, the harnesses ~90.1 s; 0 errors, every
  harness 0 differing, 34 of 34 harness lines; 5 in the race, 17 in the
  replay `itc_4444` (ok), 12 to drive). Second migration
  e55413be..bbb0cfe3 (`Draw`, `Update`, the volume, slider and HUD
  helpers, the lists and clashes, the dialogs, the save pages, the
  cursors and value changes, `Settings::Reload`, `OnLeft`, `Show`; 43
  functions, `ShowVolumeBar` Wrapped -> Overridden with its wrap kept,
  7 adapters removed; races 33-37 s, 0 errors, its harnesses 0
  differing; no subagents); left: the type 2 and 4 options' Inits (step
  tables to generate), the controller pages' picks under
  `sub_821C4BE0`, `sub_821BA380`, `sub_821C1498` and the other users of
  the settings. Validated `itc_4463` (race 88 s at 180, the
  harnesses ~90.5 s; 0 errors, every harness 0 differing, 42 of 42
  harness lines; 1 in the race, 21 in the replay `itc_4464` (ok), 20 to
  drive). Third migration 6c23d796..95016660 (the HUD, sliders,
  buttons, analog and sound pages, the clash checks, the page set-ups,
  `FadeIn`, the pages' closes, the type 2 and 4 options' and controller
  pages' Inits with their step tables from the new
  `itc_recomp/tools/init_step_tables.py`; 24 functions, 19 adapters
  removed; races 36 s, 0 errors, its harnesses 0 differing; no
  subagents); left: none in the file (98 of 100 in play ours;
  `Options::OpenCategory` waits on the save screens' lead and
  `sub_8211CBD8`; `FadeOut` stays recompiled). Validation agent
  running.

## Done

- Cleanup: `CarScene::SetUp` (`car-models.md`), `StandScene::Create`
  and the type 6 and 7 modes' slot 0 (`type6-type7-modes.md`, now 113
  of 117 in play); 15 adapters removed; c709d9ed..4ec19e07.

## Rows to review

`review.md`, "Run 2026-11-07 22:22".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4699, work left 1721 (wrapped 57, known 427,
  unknown 1237).
- After the cleanup lead's migration: ours 4703, work left 1717
  (wrapped 57, known 423, unknown 1237).
- After the options screen's map: ours 4703, work left 1717 (wrapped
  57, known 517, unknown 1143).
- After its migration: ours 4741, work left 1682 (wrapped 57, known
  482, unknown 1143).
- After its second migration: ours 4777, work left 1646 (wrapped 56,
  known 447, unknown 1143).
- After its third migration: ours 4801, work left 1624 (wrapped 56,
  known 425, unknown 1143).
