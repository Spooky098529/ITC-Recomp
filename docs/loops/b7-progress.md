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
  Validated `itc_3554` (race 47 s, 0 errors, 7 of 7 harness lines,
  6 ran in the race, `Race::ToggleEffects` in the replay `itc_3555`
  (ok); none to drive). Done.
- Subsystem 2 of 3: the command list's render-target records (lead
  picked: `sub_8209B058`, `sub_8209AE48`, `sub_8209B3D0`; runs every
  frame, `sub_8209AE48` ready, `Glow::Record` waits on it; its line
  deleted from `leads.md`). Map checked (a0cac8ac:
  `scene-target.md`, 6 rows, 1 moved from `functions.md`, all in
  play; 0 errors; 3 ready groups, `sub_8209AE48`, `sub_8209AF40`,
  `sub_8217C368`; `[0x8266D08C]` is the scene target). Migration
  982ea51b..5edff580 (`--native_scene_target`, 6 of 6 ours, with
  `Glow::Record` as a cleanup in `post-effects.md`, now 58 of 58; 5
  adapters removed; `itc_3575`: race 38 s, 0 errors, every harness 0
  differing; no subagents). Step: validation, agent running.

## Done

- The post effects' rest: `post-effects.md` (map b86338cf, migrations
  106192bc..b1df9d00 and 95c86284..6bfc02d0, `--native_post_effects`,
  57 of 58; validated `itc_3540` and `itc_3554`: 0 errors, every
  harness line, replay `itc_3555` ok, none to drive).

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
- After its validation (`itc_3554`): ours 4180, work left 2230.
- After the scene target's map: ours 4180, work left 2230 (wrapped
  62, known 475, unknown 1693).
- After its migration: ours 4187, work left 2223 (wrapped 61, known
  469, unknown 1693).
