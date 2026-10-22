# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs.

## Now

- Subsystem: the graphics library's command list (`sub_8258C9D0`,
  `sub_8258CA50`, `sub_8258CB08`, `sub_8258CB98`, `sub_8258CC30`, from
  `leads.md`).
- Step: mapping (agent running).
- Session: pass 3 of 10, subsystem 2 of 3. In all: subsystem 2 of 5.

## Done

- Sprites (`functions/sprites.md`, map ad863ff): 8 functions native in 5
  groups (04f6450, 460a8d1, a63ab81, f049c28, e5e7ef7), `--native_sprites`,
  `--native_billboards`; every harness at 0 differing, race 21 s, no
  errors. For the user: SpriteList::Reset still writes all 2,000 corners
  (writing only `count`'s would be a behavior change); main thread 6.0 ->
  5.05 M cycles a frame (profile run before profiles were dropped).

## Rows to review

- `docs/functions/sprites.md` (new; 5 rows moved from `functions.md`; 8 now
  Overridden).

## Totals

- Start (2026-10-22): ours 162, work left 6590 (wrapped 151, known 174,
  unknown 6265).
- After mapping sprites: ours 162 (wrapped 151, known 189 with sprites.md's 26),
  unknown 6244.
- After sprites' migration: ours 170, work left 6582 (wrapped 150, known
  188, unknown 6244).

## Stopped

(not stopped)
