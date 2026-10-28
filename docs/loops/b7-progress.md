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
- Subsystem 3 of 3: the save system's interface over `[0x826BDC90]`
  (`sub_8209FFB8`-`sub_820A0620`; the lead with the most ready groups,
  12). Mapped (3aa9f33): `save-interface.md`, 62 rows (1 moved, 61
  new; 41 in play): the app's interface at `[0x826BDC90]`, its save
  devices' and users' wrappers, and the front end's save layer above
  them (`GameData::Save`, `Load`, the replays); 25 ready groups;
  blocked by two ready rows in `files-and-saves.md` (`sub_82295D48`,
  `sub_82295C18`). Map check passed. Migration 1 done (63d02d8..53423d6):
  all 41 in play ours under `--native_save_interface` (with the two
  blockers and the users' two vtable targets); 17 adapters removed;
  35 s, 0 errors, every harness 0 differing; nothing ready left. Step:
  validation (agent running).

## Done

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

## Stopped

Not stopped.
