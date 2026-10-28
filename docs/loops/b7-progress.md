# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-28 18:45 on `b7-loop-28-10-26-2` (the user's
  branch), from 5068523; summary `b7-loop-2026-10-28-1845.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: the archive (`sub_820A0678`, the top lead).
  Mapped (a8a12e3): `archive.md`, 27 new rows (the B4 rows stay in
  `files-and-saves.md`, named shared), 10 ready groups; blocked by the
  game's Semaphore class (the archive's four semaphores). Map check
  passed. The user's call (2026-10-28): the four become host counting
  semaphores with a readable count, shared by every user, native and
  recompiled (the recompiled loose read reaches them through an
  adapter); the game's Semaphore class stays recompiled otherwise. Migration 1 done (47d1ab1..eac2d83): the 14 ready
  functions ours under `--native_archive` (Toc, AssetSize, the request
  list, slot lookup, loose files, static set-ups, request finish,
  `LoaderFunctor::Invoke`, Ucl); 38 s, 0 errors, every harness 0
  differing; nothing left ready but the semaphore users. Validated
  (`itc_2262`): 14 of 14 harness lines, all 14 in the attract race; 8
  s, 0 errors, every harness 0 differing but the expected groups;
  nothing to drive. Step: migration 2 (the semaphore users, by the
  user's call; agent running).

## Done

Nothing yet.

## Rows to review

`review.md`, "Run 2026-10-28 18:45".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 2540, work left 3920 (wrapped 103, known 523,
  unknown 3294).
- After the archive's map: ours 2540, work left 3920 (wrapped 103,
  known 534, unknown 3283).
- After the archive's migration 1: ours 2554, work left 3906 (wrapped
  103, known 520, unknown 3283).

## Stopped

Not stopped.
