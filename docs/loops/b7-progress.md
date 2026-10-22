# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs.

## Now

- Subsystem: the C runtime helpers (`sub_825A9934`, the stack probe;
  `sub_825A8170` -> `sub_825A7FB0`, probably `acos`; from `leads.md`), with
  the rest of the C runtime seen in play except the heap (a lead that
  waits on the user).
  Mapped as `functions/c-runtime.md` (6cbe96a): 89 rows, 54 ready
  groups; the heap, TLS (`_getptd`) and the kernel-calling pieces left
  out (the core).
- Step: migration (first agent running).
- Session: pass 7 of 20, subsystem 3 of 5. In all: subsystem 3 of 5.

## Done

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

## Stopped

(not stopped)
