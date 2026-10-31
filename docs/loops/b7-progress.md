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
  lead's note that its rows were elsewhere was wrong). Step: migration
  (agent running).

## Done

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

## Stopped
