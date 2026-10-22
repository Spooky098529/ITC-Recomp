# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md` (their rows to review and drive lists are
there), and this file's history.

## Now

- Run started 2026-10-22 22:11 on `b7-loop-22-10-26-2` (the user's
  branch), from 91f40be; summary `b7-loop-2026-10-22-2211.md`.
- Subsystem 1 of 3 (the C runtime heap) finished.
- Subsystem 2 of 3: the collision world's narrow phase and shapes
  (`sub_820BB868`, `sub_820BBE30`, ...; from `leads.md`: runs every
  physics step, many ready groups, blocks `car-physics.md`), mapped in
  `collision.md` (dd758b3; 64 rows; checked: 43 ready groups). Step:
  migration 1 (agent running).

## Done

- The C runtime heap (`heap.md`, its existing rows read in full in
  1f48eaf): 15 functions ours (12 Overridden, 3 Native) in one migration
  (510aae9..34d8c7b), validated (15 of 15 harness lines, all ran in
  play); `--native_c_runtime_heap`. Left: none (the game's own heap
  internals aren't ported).

## Rows to review

- `collision.md` (new, 64 rows), `known-data.md`'s shape, box, pair
  query and table entries, `car-physics.md`'s intro; 1 lead added (the
  track's collision queries). Guesses to confirm: the floats at
  `0x82002B90`..`0x82002B98` and the impulse scale at `0x82600E84`.

- `heap.md`: 15 rows read in full and rewritten (no status changed), the
  intro's paragraph on native versions; migration 1's 15 rows (12
  Overridden, 3 Native; XMemAlloc's attribute bits corrected by the
  harness: protection `(attr >> 28) & 3`, default alignment field 12). For
  review: the adapters run the plain C++ only while `--native_heap` is
  active (`hooks::NativeHeapActive()`), and the error paths (errno, the
  new handler, `bad_alloc`, `_invalid_parameter`) run `__imp__` from the
  start; `known-data.md`'s heap globals;
  1 lead added (`sub_8235AAC0`), operator new taken from the
  VirtualAlloc lead.

## Expected to differ

## To check in a drive

## Totals

- Start: ours 714, work left 6039 (wrapped 137, known 336, unknown 5566).
- After mapping the heap: the same (rows rewritten in place).
- After the heap's migration 1: ours 729, work left 6024 (wrapped 137,
  known 321, unknown 5566).
- After mapping the collision narrow phase: ours 729, work left 6024
  (wrapped 137, known 385, unknown 5502).
