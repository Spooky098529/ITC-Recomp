# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, `b7-loop-2026-10-23-1722.md`,
`b7-loop-2026-10-23-2022.md`, and this file's history. What's left to
see in a drive, from every run: `to-drive.md`.

## Now

- Run started 2026-10-24 03:25 on `b7-loop-24-10-26` (the user's
  branch), from 8073e3b; summary `b7-loop-2026-10-24-0325.md`.
- Subsystem 1 of 3: `leads.md`'s top lead (`sub_822A5B78`, a texture's
  description, with the UI movie's texture set): mapped as the graphics
  resources (`graphics-resources.md`, 44 rows, 0e67c00; 13 ready
  groups). The map's checks passed, but its report's `stop:` named two
  choices for the user, so the loop stopped before migrating (below); resumed with the
  user's calls. Migration 1 done (e4730b8..e6b9811, 25 of 45 ours),
  validated (`itc_1267`, `itc_1269`; replay ok; 1 to drive): second
  migration agent running (Lock, Unlock, Destroy by the user's call,
  and the three ready groups left).

## Done

## Rows to review

This run's rows: `review.md` ("Run 2026-10-24 03:25").

## Expected to differ

This run's groups: `expected-differences.md` (the "03:25 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1485, work left 5273 (wrapped 123, known 559, unknown 4591).
- After mapping the graphics resources: ours 1485, work left 5273
  (wrapped 123, known 595, unknown 4555).

- After the graphics resources' migration 1: ours 1508, work left 5251
  (wrapped 123, known 573, unknown 4555). Race 28 s (`itc_1266`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing outside
  one "(expected to differ)" group added (Edram::Alloc's critical
  section, the user's call).

## Stopped

2026-10-24, after mapping the graphics resources: the mapping report's
`stop:` wasn't `none` (`b7-loop.md`, "When to halt"). For the user:
(1) `Edram::Alloc` (`sub_822B5EF8`, a ready group) calls
RtlInitializeCriticalSection, RtlEnterCriticalSection and
RtlLeaveCriticalSection: porting it replaces kernel imports. (2)
Whether `Resource::Lock`'s fence wait and cache flush packet,
`Resource::Unlock`'s data cache flush (`sub_822AC470`) and
`Resource::Destroy`'s fence waits are Dropped under the graphics rule
(blocked groups: needed only when they're ported). The other 12 ready
groups (format math, descriptions, buffer creation, the device's
getters) need neither. Map checks: `check_function_statuses.py` 0
errors; `b7_status.py` lists the file, 44 of 44 in play. Next: the
user's two calls, then a new loop (or this run continued).

Resumed 2026-10-24 (the user's calls): (1) `Edram::Alloc` (`sub_822B5EF8`) made native with a `std::mutex`
in place of the kernel's critical section (its bases kept: the
renderer keys targets by them); (2) `Resource::Lock`'s and
`Resource::Destroy`'s fence waits, `Lock`'s cache flush packet and
`Unlock`'s data cache flush (`sub_822AC470`) Dropped, the lock count,
the returned address and the freeing kept (a harness group for the
command buffer's words expected to differ). The run goes on
with this subsystem's migration.
