# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-08 on `b7-loop-08-11-26` (made by the user for
  this run), from 2bf66687; summary `b7-loop-2026-11-08-1300.md`.
  Limit: 3 subsystems.
- Subsystem 1 of 3: the UI movie helpers without rows (two leads taken
  together: `sub_8211BC18`, whether an animation is running or queued,
  which 19 of the car list's groups wait on, the top recommendation of
  the last run; and `sub_8211CC50`, the node text block's clear beside
  the UI movie text calls; both lines deleted from `leads.md`). Mapped
  into the existing `ui-movies-game.md` (9f12739f: 8 new rows, 3 in
  play, `sub_8211BC18`, `sub_8211DE30`, `sub_8211CC50`, each a ready
  group; 5 never run; no leads added; no subagents); map checked.
  Migration e052912e..e73bf7db (the three native under
  `--native_ui_movies_game`, with `sub_8211DCE8`'s adapter removed;
  cleanup in `car-list.md` they unblocked: the panels' answers, shows,
  selections and moves, eight steps, `SetUpRows`, `SetUpCarPage`, under
  `--native_car_list`; 22 adapters removed; resumed once after an API
  sign-in error cut it off mid-batch; race 36 s, 0 errors, its
  harnesses 0 differing; no subagents). Left: `Step17` `sub_8224C450`
  and `CarPage` `sub_8224B8D8` (need a harness world with the stand's
  car), and `sub_8211E500`, `sub_8211E2C8`, `sub_8211E370`,
  `sub_8211CBD8`, `sub_8211CA18` in `ui-movies-game.md`, ready, not
  taken. Step: validation, agent running.

## Done

## Rows to review

`review.md`, "Run 2026-11-08 13:00".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4920, work left 1506 (wrapped 56, known 448,
  unknown 1002).
- After the UI movie helpers' map: ours 4920, work left 1506 (wrapped
  56, known 451, unknown 999).
- After its migration: ours 4944, work left 1482 (wrapped 56, known
  427, unknown 999).

## Stopped
