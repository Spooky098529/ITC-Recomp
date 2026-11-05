# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-05 20:21 on `b7-loop-05-11-26`, from 7b039c95;
  summary `b7-loop-2026-11-05-2021.md`. Limit: 3 subsystems.
- Subsystem 1 of 3: the other makers and users of the graphics
  resources (the top lead: `sub_82135AA8`, `sub_82136130`,
  `sub_821330D0`, `sub_820ED6D0`, `sub_825D99D0` left of it; its line
  deleted from `leads.md`). Map checked (b86338cf: the lead was
  stale, its functions already in `post-effects.md` and
  `car-models.md`; one new row, `sub_825D99D0`
  `PostEffects::InitEdramBase`, in `post-effects.md`; 0 errors; 7
  ready groups: the rest of the post effects, 13 recompiled, all
  ready). Migration 106192bc..b1df9d00 (12 of 13 ours under
  `--native_post_effects`, 5 Native with no adapter; `itc_3535`: race
  38 s, 0 errors, every harness 0 differing but `matrix_math`'s
  expected groups; no subagents); left: `Glow::RecordStar`
  `sub_82136FA8` with `sub_82134060` (ready, the batch limit). Step:
  Validated `itc_3540` (race 47 s, 0 errors, 11 of 11 harness lines,
  all 11 ran in the race; no replay needed; none to drive). Step:
  Second migration 95c86284..6bfc02d0 (`Glow::RecordStar` with
  `RecordStarPass`; 5 adapters removed; `itc_3553`: race 38 s, 0
  errors, 64 of 64 harness lines 0 differing; no subagents); nothing
  ready left (`Glow::Record` waits on `sub_8209AE48`, in `leads.md`).
  Step: its validation, agent running.

## Done

(None yet.)

## Rows to review

`review.md`, "Run 2026-11-05 20:21".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4167, work left 2243 (wrapped 62, known 482,
  unknown 1699).
- After the post effects' map: ours 4167, work left 2243 (wrapped
  62, known 483, unknown 1698).
- After its first migration: ours 4178, work left 2232 (wrapped 62,
  known 472, unknown 1698).
- After its validation (`itc_3540`): ours 4178, work left 2232.
- After its second migration: ours 4180, work left 2230 (wrapped 62,
  known 470, unknown 1698).
