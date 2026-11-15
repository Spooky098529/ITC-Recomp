# B7 lift sweep: the never-seen functions, translated by a tool

A variation on the B7 loop (`b7-loop.md`) for the code play never
reaches (the user's call, 2026-11-15). It has the same goal ("B7's
goal" there: no PPC code reachable from `main`) and is measured by the
same static map (`static-map.md`), but it works differently. The B7 loop
maps a subsystem, ports it by hand and validates each step with
harnesses. This sweep has a tool translate the never-seen functions
straight from `generated/` in large batches, and validates once at the
end.

Why: on 2026-11-14 the static map had 3,106 unported functions. 2,482
of them (80%) had never been seen in play, 2,474 were 100 lines or
fewer, and 1,728 had no row in any function file. For code like that,
the loop's per-subsystem mapping, harnesses and 180 s validations cost
far more than the code itself.

## Scope

Taken by the sweep: a function the static map lists as `unported` with
`seen` 0 (`itc_recomp/coverage/static_map.tsv`), outside the replaced
libraries, System Link, the multiplayer modes (below) and anything the
user has ruled out.

Left to the B7 loop: every function seen in play (624 on 2026-11-14),
and anything the tool refuses (below). The two don't take the same
function at once: the sweep runs between loop runs, not beside them.

## What changes from the B7 loop

- **Behavior:** "B7: no behavior change" is loosened for never-seen
  functions. The tool's translation follows the recompiled code
  instruction by instruction, so it matches by construction. The loosening
  only lets a later cleanup simplify what clearly doesn't matter (a
  debug print, an assert's message), noted in the row.
- **No mapping step:** no mapping agent and no map check. The tool
  writes the rows itself (below).
- **No harness per function:** the tool is checked instead (a sample
  and every construct it handles, under "Validating the tool"). Since a
  mechanical translator's mistakes are systematic, a correct sample
  stands for the batch.
- **No validation per batch:** each batch must build and pass a short
  smoke run (below). The full validation is done once, at the end.
- **Readability:** lifted code may read like the recompiled code
  without `ctx` (registers as locals, guest loads and stores). Cleaning
  it into the plain C++ of the hand ports is for later, if ever.

Everything else under "Rules that still hold" in `b7-loop.md` still
holds (CLAUDE.md's rules, nothing in parallel, the replaced libraries,
commits pushed).

## The tool

`itc_recomp/tools/lift.py` (to be written). It reads a function's body
in `generated/` (never edits it) and writes:

- the plain C++, in `itc_recomp/src/lifted/`, one file per address
  range, starting with `ITC_NATIVE_RUNNING`;
- the `REX_FUNC` adapter, under one flag per batch
  (`--native_lifted_<n>`, on by default), so a bad batch can be found
  by turning flags off;
- calls to other functions by their native entry when they're ours,
  and through the guest call otherwise (that callee's turn comes in a
  later batch);
- the row (below).

It **refuses** anything it can't translate exactly, and lists those
functions with the reason. Refused functions go to the B7 loop as a
lead. Cases to expect: indirect calls it can't resolve, setjmp-like
control flow, and constructs the recompiler emits that the tool doesn't
model yet.

Batches go bottom-up in the static map's order (callees before callers,
the frontier first), a few hundred functions each.

## Rows

Each lifted function gets a row. It goes in its subsystem's file when it
has one, otherwise in `docs/functions/lifted.md`, by address. Status as
for a native function; the Notes say "Lifted by `lift.py`, not seen in
play" and what the tool could tell (callees, callers, imports). Nothing
is read in detail. The rows go in `review.md` under the sweep's
heading, as a list of ranges and counts rather than one by one.

## Steps

1. **Prototype:** the tool on about 20 small never-seen functions of
   different shapes (getter, setter, thunk, destructor, a loop, floats,
   a vtable call). Every one gets harness cases, and the tool is fixed
   until all match.
2. **Sweep:** batch by batch. After each batch: the build, then
   `attract_race.sh 40` as a smoke run (it boots, races, logs no new
   errors). A batch that fails either is fixed or its flag turned off
   before the next. Commit and push each batch.
3. **Validate at the end** (below).
4. **Record:** `static_map.py`, `boundary_status.py` and `b7_status.py`
   run, their totals in "The boundary over time"
   (`docs/rexglue-removal.md`), and a summary like the loop's run
   summaries.

## Validating the tool

At the end, once:

- **Harness a sample:** a random sample of lifted functions (about 5%)
  plus at least one of every construct the tool translates, run with
  `attract_race.sh 180 --run_harnesses=lifted`. Every harness at 0
  differing.
- **Run the game:** the attract race and the pad replay
  (`scripted_run.sh`), then a drive by the user through the menus, a
  save and a race. Problems are found with logs, as always: turn the
  batch flags off to find the batch, then the function.

Note that running the game exercises little of the lifted code (it was
never seen in play): the harness sample is what checks it. Bugs left
over would show on rare paths (save errors, options pages).

## The user's calls (2026-11-15)

- Lifted-looking code is fine for every size of function, the big ones
  (over 100 lines, ~630 on 2026-11-14) included.
- The sweep runs on its own branch, `lift-sweep`, from
  `rexglue-removal-project`.
- The Xbox Live (type 12) and two-player (type 8) modes' code is
  skipped, like System Link: the main menu's items for them were
  turned off (`--multiplayer_menu_items`, off by default) so they need
  no port. The tool leaves them out and the static map's count of what's
  left should too.
