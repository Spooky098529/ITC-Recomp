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
