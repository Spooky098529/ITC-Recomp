# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs.

## Now

- Subsystem: the UI movie library (`8259C000`-`825A6800`; `sub_825A08E0`,
  `sub_825A0EF0`, `sub_825A0CF8`, `sub_825A1218`, `sub_825A1C88`,
  `sub_825A1D68`, `sub_8259C9B0`, from `leads.md`).
- Step: mapping (agent running).
- Session: pass 13 of 20, subsystem 4 of 5. In all: subsystem 4 of 5.

## Done

- C runtime, sixth migration: %f's `_cfltcvt_l`, `_cftof_l`, `_cftof2_l`,
  `_fptostr` (36d633f); harnesses at 0 differing, race 21 s, no errors.
  The C runtime is done: 79 of 89 native (with 2 more counted as ours);
  the rest waits on the heap, TLS (`_getptd`) and setjmp's caller.
- C runtime, fifth migration: memmove, `memcpy_s`, the atof chain,
  `write_multi_char` and the wide character output, `_fltout2`,
  `$I10_OUTPUT` (616f41d..f8933d3), under `--native_c_runtime`; setjmp
  left recompiled (e06e3a6, reason in its row); harnesses at 0 differing,
  race 21 s, no errors. For the user: the agent ran `git checkout` on a
  file holding uncommitted work (against the rules); it rebuilt the work
  and every harness passed before committing.
- C runtime, fourth migration: tan, ldexp, `_clearfp`, `_ld12tod`,
  `___dtold`, `write_char`, `__iob_func`, the start-up setters, and
  memcpy's copy inside its mirror-tracking override (b271bed..147dbae),
  new flag `--native_memcpy`; harnesses at 0 differing, race 20 s (and a
  4-minute race, ~80 million native copies), no errors.
- C runtime, third migration: rint, `_ctrlfp`, `_abstract_sw`, abs, the
  wide and narrow string helpers, the second memcpy, the character helpers,
  strtoul (36b44e6..7c52ae3), under `--native_math` and
  `--native_c_runtime`; harnesses at 0 differing, race 20 s, no errors.
  Not run in attract races: abs, mbtowc, strncpy, strcpy_s, strstr.
- C runtime, second migration: pow and its six helpers, floor, ceil,
  log10, `__ultod`, `_fpclass` (d98d94f..2138474), under `--native_math`;
  harnesses at 0 differing, race 20 s, no errors.
- C runtime, first migration: stack probe (Dropped), asin/acos, memset,
  sin, cos, atan, atan2 (ebf6cf9..14fd610), `--native_c_runtime`,
  `--native_math`; harnesses at 0 differing, races 20-21 s, no errors.
  `src/c_runtime/math.cpp` builds with `-ffp-model=strict
  -ffp-contract=off` (NaN signs bit for bit).
- Sprites, second migration: `sub_82128E70`, `sub_82129280` native under
  `--native_sprites` (0ce9dcc, 23bd994); harnesses at 0 differing, race
  20 s, no errors. Not run in attract races (probably the marker above
  opponents in its screen-placed mode): checked by harness only. Left:
  blocked by Matrix::Multiply (Wrapped), memcpy, the stack probe, acos,
  the texture animations.
- Command list (`functions/command-list.md`, map e0908c6): 9 functions
  native in fa317f9, `--native_command_list`; every harness at 0
  differing, race 21 s, no errors. Left: `CommandList::Init`, waiting on
  the guest heap (the user's decision).
- Sprites (`functions/sprites.md`, map ad863ff): 8 functions native in 5
  groups (04f6450, 460a8d1, a63ab81, f049c28, e5e7ef7), `--native_sprites`,
  `--native_billboards`; every harness at 0 differing, race 21 s, no
  errors. For the user: SpriteList::Reset still writes all 2,000 corners
  (writing only `count`'s would be a behavior change); main thread 6.0 ->
  5.05 M cycles a frame (profile run before profiles were dropped).

## Rows to review

- `docs/functions/sprites.md` (new; 5 rows moved from `functions.md`; 8 now
  Overridden).
- `docs/functions/command-list.md` (new; `sub_8258CAA0` moved from
  `functions.md`, Wrapped -> Overridden).
- `docs/functions/c-runtime.md` (new; `memcpy`, `memset`, `wcslen`, `pow`
  moved from `functions.md`).

## Totals

- Start (2026-10-22): ours 162, work left 6590 (wrapped 151, known 174,
  unknown 6265).
- After mapping sprites: ours 162 (wrapped 151, known 189 with sprites.md's 26),
  unknown 6244.
- After sprites' migration: ours 170, work left 6582 (wrapped 150, known
  188, unknown 6244).
- After mapping the command list: ours 171, work left 6582 (wrapped 150,
  known 198, unknown 6234).
- After the command list's migration: ours 180, work left 6573 (wrapped
  150, known 189, unknown 6234).
- After sprites' second migration: ours 182, work left 6571 (wrapped 150,
  known 187, unknown 6234).
- After mapping the C runtime: ours 182, work left 6571 (wrapped 150,
  known 272, unknown 6149).
- After the C runtime's first migration: ours 191, work left 6562
  (wrapped 150, known 263, unknown 6149).
- After the C runtime's second migration: ours 203, work left 6550
  (wrapped 150, known 251, unknown 6149).
- After the C runtime's third migration: ours 228, work left 6525
  (wrapped 150, known 226, unknown 6149).
- After the C runtime's fourth migration: ours 245, work left 6508
  (wrapped 149, known 210, unknown 6149).
- After the C runtime's fifth migration: ours 257, work left 6496
  (wrapped 149, known 198, unknown 6149).
- After the C runtime: ours 261, work left 6492 (wrapped 149, known 194,
  unknown 6149).

## Stopped

(not stopped)
