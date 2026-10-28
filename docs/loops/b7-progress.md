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
- Subsystem 2 of 3 done: the file layer (below). Recorded in the
  plan's table and boundary log.
- Subsystem 3 of 3 done: the save system's interface (below).
  Recorded in the plan's table and boundary log. The run's limit
  reached: stopped (below).

## Done

- The save system's interface: `save-interface.md` (with rows in
  `files-and-saves.md`, `system-services.md`), 41 of 41 in play native
  in one migration, 17 adapters removed; `--native_save_interface`;
  3aa9f33, 63d02d8..53423d6. Validated (`itc_2311`, `itc_2312`): 46 of
  46 harness lines; the replays' list (4) to drive.
- The file layer: `file-layer.md` (with rows in `c-runtime.md`,
  `files-and-saves.md`), 17 native in one migration (with sprintf and
  the archive's loose read), 7 adapters removed;
  `--native_file_layer`; f053e6c, 47a7c1d..edea212, print a2eadab.
  Validated (`itc_2291`, `itc_2293`): four to drive.
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
- After the file layer's migration 1: ours 2582, work left 3879
  (wrapped 103, known 504, unknown 3272).
- After its validation (`itc_2291`: race 8 s, 0 errors, every harness
  0 differing but the expected groups): ours 2582, work left 3879.
- After the save interface's map: ours 2582, work left 3879 (wrapped
  103, known 544, unknown 3232).
- After the save interface's migration 1: ours 2628, work left 3833
  (wrapped 103, known 498, unknown 3232).
- At the end (after the save interface's validation, `itc_2311`: race
  8 s, the harnesses ~30 s, 0 errors, every harness 0 differing but
  the expected groups): ours 2628, work left 3833 (wrapped 103, known
  498, unknown 3232).

## Stopped

2026-10-28, at the run's limit: 3 subsystems (the archive, the file
layer, the save system's interface). One user's call on the way (the
archive's four semaphores host counting semaphores). Every map,
migration and validation passed its checks.
`check_function_statuses.py`: 0 errors. To drive: `to-drive.md`.
