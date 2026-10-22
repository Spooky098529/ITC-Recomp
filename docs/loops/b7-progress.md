# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs.

## Now

- Subsystem: sprites again (`functions/sprites.md`): the command list made
  `sub_82128E70` (under `sub_82129280`) ready.
- Step: migration (second agent running).
- Session: pass 5 of 20, subsystem 2 of 5 (sprites isn't counted again).
  In all: subsystem 2 of 5.

## Done

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

## Stopped

(not stopped)
