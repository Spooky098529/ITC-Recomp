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
- Subsystem 1 of 3 done: the archive (below). Recorded in the plan's
  table and boundary log.
- Subsystem 2 of 3: the file layer (two leads taken together: the
  file object `sub_820A6A00`, `sub_820A6A40` and the path objects
  `sub_82593C28`, `sub_82593C48`, `sub_82593CA0`; ready, and blocking
  the archive's loose read). Mapped (f053e6c): `file-layer.md`, 21
  new rows (11 in play; shared rows stay in `files-and-saves.md`,
  `sound-cues.md`, `movies.md`, `c-runtime.md`), 5 ready groups;
  blocked by sprintf (to follow swprintf's precedent,
  `src/c_runtime/wide_format.cpp`). Map check passed. Step: migration
  1 (agent running).

## Done

- The archive: `archive.md` (with shared rows in `files-and-saves.md`,
  `video-output.md`), 25 native in two migrations, 8 adapters removed,
  the four semaphores host ones (the user's call); `--native_archive`;
  a8a12e3, 47d1ab1..eac2d83, f4b6d09..db11bea. Validated (`itc_2262`;
  `itc_2276`, `itc_2277`): two cancels to drive.

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
- After the archive's migration 2: ours 2565, work left 3896 (wrapped
  103, known 510, unknown 3283).
- After its validation (`itc_2276`: race 8 s, the harnesses ~30 s, 0
  errors, every harness 0 differing but the expected groups): ours
  2565, work left 3896.
- After the file layer's map: ours 2565, work left 3896 (wrapped 103,
  known 521, unknown 3272).

## Stopped

Not stopped.
