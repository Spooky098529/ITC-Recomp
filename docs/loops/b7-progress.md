# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-31 16:54 on `b7-loop-31-10-26` (the previous
  run's branch, continued by the user's call, 2026-10-31), from
  ec0e7141; summary `b7-loop-2026-10-31-1654.md`.
- Limit for this run: 3 subsystems; 1 taken.
- Subsystem 1 of 3: the second skeleton user (`820EF258`-`820F05D8`),
  the top of `leads.md`:
  - `sub_820EF258`, `sub_820EF310`, `sub_820EF368`, `sub_820EFFD8`, `sub_820F0070`, `sub_820F03F0`: another skeleton user (`820EF258`-`820F05D8`): the skeleton base made (`sub_820F03F0`, 179 lines, calls `SkeletonBase::SkeletonBase`), set up (`sub_820F0070` calls `sub_8259A9E0`) and destroyed (`sub_820EFFD8`, `SkeletonBase::~SkeletonBase`; `functions/model-animation.md`, `functions/models.md`), near `sub_820EF790` (`Model::RecordShape`'s caller). Not read. Found by the B7 loop's mapping agent (model animation), 2026-10-23.
  Map checked (ae8e322b: `accessories.md`, the interior accessories
  `820EF250`-`820F1078`, 50 rows, 31 in play, 8 ready groups, no
  outside blockers; the accessories' release lead folded in; the
  lead's note that its rows were elsewhere was wrong). First migration checked (8d5c9078..24186d92: 27 functions native
  or overridden, 13 adapters removed, `--native_accessories`, harness
  `accessories` 27 groups 0 differing, race 38 s, 0 errors; no
  subagents). Left: `Accessory::Finish`, `AccessoryEntry::OnLoaded`,
  `Accessories::Acquire` with `AcquireEntry`. Validated (`itc_2946`, 120 s: race 59 s, 0 errors, harnesses ~60 s;
  replay `itc_2947` ok): 27 of 27 harness lines, 5 ran in the race,
  22 in the replay, none to drive. Second migration checked (2eb6ca57..3a9de36a: `Accessory::Finish`,
  `AccessoryEntry::OnLoaded`, `Accessories::Acquire` with
  `AcquireEntry`, `Accessories::Release` (`scene-loaders.md`); 6 more
  adapters removed; 31 of 31 in play ours; harness 0 differing, race
  38 s, 0 errors; no subagents). Nothing ready left. Validated (`itc_2963`, 120 s: race 60 s, 0 errors, harnesses 58.4 s;
  replay `itc_2964` ok): 11 of 11 harness lines, 3 ran in the race, 8
  in the replay, none to drive. Done.
- Cleanup lead (no mapping, not counted): `RaceHud::UpdateRacerBar`
  (`sub_82214790`) and `RunCountdown` (`sub_822129A8`) in
  `race-hud-object.md`, ready since the menu sound is ours. Step:
  migration (agent running).

## Done

- The interior accessories (subsystem 1): `accessories.md`, 31 of 31
  in play, two migrations; `--native_accessories`; ae8e322b..3a9de36a.
  Validated (`itc_2946`, `itc_2963`).

## Rows to review

`review.md`, "Run 2026-10-31 16:54".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3552, work left 2846 (wrapped 103, known 487,
  unknown 2256).

- After the accessories' map: ours 3552, work left 2846 (wrapped 103,
  known 518, unknown 2225).
- After the accessories' first migration (race 38 s, 0 errors,
  accessories 0 differing): ours 3579, work left 2819 (wrapped 103,
  known 491, unknown 2225).
- After its validation (`itc_2946`, 120 s: race 59 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3579, work left
  2819.
- After the accessories' second migration (`itc_2962`: race 38 s, 0
  errors, accessories 0 differing): ours 3584, work left 2814 (wrapped
  103, known 486, unknown 2225).
- After its validation (`itc_2963`, 120 s: race 60 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3584, work left
  2814.

## Stopped
