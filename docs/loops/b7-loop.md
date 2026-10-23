# B7 loop: map a subsystem, make it native, take the next lead

Instructions for a `/loop` session working on B7 ("The game logic
inside" in `docs/rexglue-removal.md`) without the user. Start it, in a
fresh session (`/clear` first), with:

```
/loop follow docs/loops/b7-loop.md
```

with no interval: each pass schedules the next one itself. The session
running the loop (the **lead**) does no mapping or porting itself: it
hands each step to one subagent at a time, reads its report, checks it,
and decides what's next or halts. Its own context grows by a few reports
a pass, and every step starts with a fresh agent. What a pass needs to
know lives in the repo (this file, `docs/loops/b7-progress.md`,
`docs/loops/leads.md`, the function files, commits), not in the session's
memory.

## Settings (set by the user, 2026-10-22)

- **Branch:** the branch the user made for the run and checked out
  before starting it (a new one for each run). The loop stays on it: the
  first pass notes its name in the progress file and doesn't make it.
  The loop never makes another branch, never touches `main` or tags, and never merges:
  the user reviews and merges the branch.
- **Code changes without asking first:** allowed on the run's branch, for the
  subsystems the loop takes. This relaxes the "explain before code
  changes" rule for the loop's branch, nowhere else.
- **Function rows:** written by the agents as they go (the status check
  after each change), and listed in the progress file for one review by
  the user before the merge. This relaxes the "show rows and ask first"
  rule for the loop's branch, nowhere else.
- **Limit:** 3 subsystems a run, then the loop stops for the user's
  review. A run is one loop, from its start until it stops (the user's
  call, 2026-10-22; before, a session ended itself after a few
  subsystems or passes, and a run went on across sessions). Each loop
  started is a new run: its first pass starts the progress file afresh
  and makes its summary.

## The cycle

1. **Pick a subsystem.** The first is the top of `docs/loops/leads.md`;
   after that, the lead picks from `leads.md`: the lead whose functions
   run most often or are closest to being ready
   (`itc_recomp/tools/b7_groups.py <its functions>`), not one that needs
   a decision only the user can make. Its line is deleted from `leads.md`
   when picked. A lead marked "no mapping" (cleanup in files
   already mapped, such as adapters to remove) goes straight to a
   migration agent (step 6) with the files it names, and doesn't count as
   a subsystem toward the limits.
2. **Hand it to a mapping agent** (below).
3. The mapping agent **writes `docs/functions/<name>.md`** for the
   subsystem's functions.
4. It **adds leads**: functions worth investigating that look like other
   subsystems go to `docs/loops/leads.md`.
5. **The lead checks the map** ("Checking a map" below). A failed check
   halts the loop.
6. **Hand it to a migration agent** (below), which makes the subsystem
   native group by group.
7. The migration agent works in the **build, attract race, debug** cycle
   used so far, and reports. A subsystem too big for one agent gets
   another migration agent after the first reports, until nothing ready
   is left in it. The lead adds the report's nonzero `expected to differ:`
   groups to `expected-differences.md` ("Expected differences" below),
   and what's for the user's review to `review.md` ("Rows to review"
   below), not to the progress file.
8. **Hand its work to a validation agent** (below; added by the user,
   2026-10-22) after each migration agent's report has passed the
   lead's checks, the "no mapping" leads' too: it checks from a log that
   every function the step made native ran as our code in its harness,
   and which ran in play.
9. **Record the finished subsystem** in the summary docs, once nothing
   ready is left in it (the lead does this itself, in the pass that
   takes the next lead, and commits it with the progress file):
   - its row in the table under "B7's loop runs" in
     `docs/rexglue-removal.md` (date, subsystem, function file, how many
     of its functions are native, the flags), and anything the run
     learned that the plan should say (what blocks the rest, a build
     setting) in the text under it;
   - `itc_recomp/tools/boundary_status.py` and `b7_status.py` run, their
     totals added as a dated entry at the end of "The boundary over time"
     in the same doc.
10. **Take the next lead**: back to step 1.

## The run's summary

Each run (one loop, from its start until it stops) gets a short summary file, `docs/loops/b7-loop-<date>-<time>.md`
(the run's start, like `b7-loop-2026-10-22-0111.md`), made by the run's
first pass and named under "Now" in the progress file. Not wordy: a title,
the branch and the commit the run started from, a table of the subsystems
finished (function file, how many made native, the flags), a few lines of
totals (`b7_status.py`'s ours and work left, before and after) and what's
left for the user, then "Status changes": the output of
`itc_recomp/tools/status_changes.py <start commit>` (one line per
function whose Status changed, by function file). The lead brings it up
to date in the pass that records each finished subsystem (step 9 of the
cycle), and once more when the loop stops, committed with the progress
file.

A pass does one step: it reads the state, collects the report of the
agent that finished, checks it, records it in the progress file, starts
the next agent, and schedules the next pass. A running agent's finishing
wakes the loop, so the fallback wake-up is long (about 30 minutes); when
nothing runs and there's work, about a minute.

## The tooling (what the agents use)

- `itc_recomp/coverage/seen_in_play.txt`: the functions seen in play
  (`docs/coverage.md`). Functions that never ran get no rows.
- `itc_recomp/tools/b7_status.py`: B7's measure (ours, wrapped, known,
  unknown), by function file; checks rows that say "never called" whose
  function ran.
- `itc_recomp/tools/b7_groups.py [root ...]`: a function's group (it and
  the callees only it calls), whether it's ready (nothing outside it that
  runs is still recompiled), the ready groups under a root and the
  blockers. `--removable` lists the overridden functions whose adapters
  may go (see "Removing an adapter"). Groups are made native from the
  bottom up, as native code can
  only call what's already ours.
- `itc_recomp/tools/check_function_statuses.py`: the Status column
  against `src/` (run after any row change); a function in rows of two
  files is an error.
- `itc_recomp/tools/attract_race.sh <seconds> [flags]`: an attract race
  with the standard flags: the log, the seconds at 30 ticks/s, error
  lines, harness results (`--run_harnesses=all`).
- `itc_recomp/tools/scripted_run.sh <recording> [flags]`: a recorded pad
  (`itc_recomp/pad_recordings/`) replayed into the game, for functions
  attract races don't reach (the menus, a driven race); the same report
  (`docs/pad-replay.md`). The validation agent replays it after its
  attract race ("The validation agent", step 3).
- `src/harnesses/`: native against recompiled on the same inputs
  ("Checking with a harness" in `docs/rexglue-removal.md`).
- `--native_renderer_dump=<dir>`: frames from a race.
- `ITC_THREAD=main itc_recomp/tools/profile_render_thread.sh`: cycles a
  frame, for speed (not part of the loop's checks: see "The migration
  agent").
- `itc_recomp/tools/boundary_status.py`: the SDK boundary.
- `itc_recomp/tools/status_changes.py <commit>`: the functions whose
  Status changed since a commit, for the run's summary.
- Finding code: `grep -n "DEFINE_REX_FUNC(sub_X)"
  itc_recomp/generated/default/*.cpp`; call sites `grep -ln "lr =
  0xXXXXXXXX;"`; overrides `grep -rn sub_X itc_recomp/src`.

## The mapping agent

Prompt it with: the subsystem (its lead line from `leads.md`: the
functions, why), this file's "The mapping agent", "The tooling" and
"Rules that still hold", and the report format. It changes docs only (no
code, no builds, no game runs):

- **Finds the subsystem:** from the lead's functions, their callees seen
  in play (`b7_groups.py <functions>` lists the subtree) and the callers
  up to where another subsystem begins. At most ~150 functions: beyond
  that, it maps a part and puts the rest in `leads.md`.
- **Reads each function** in `generated/`: what it does, its arguments and
  return value, the guest memory it reads and writes (objects, fields,
  globals), its callers and callees, how often it runs where known.
- **Writes `docs/functions/<name>.md`** (a short, lowercase name, words
  joined by hyphens, like `audio.md`): a title "Known functions:
  <subsystem>", a paragraph saying what the subsystem is and which docs
  cover it, the Status definitions copied from the top of `functions.md`
  word for word, then the table (`| Address | Name | Status | Notes |`,
  one function a row, in the order that reads best; no sorting required).
  Only functions seen in play get rows. Notes say how much is known ("Not
  read", "Not read in detail", "probably" for guesses), as the other files
  do. Fields and globals worked out go in `docs/known-data.md`.
- **Moves rows, never copies them:** a function already in `functions.md`
  moves to the new file with its row (kept as it was, with what's new
  added). A function already in another subsystem file stays there: the
  new file names it in its notes as shared.
- **Lists the file** in `functions.md`'s "So far" list.
- **Adds leads** to `docs/loops/leads.md` (format there): functions that
  look like another subsystem, not already in a function file or in
  `leads.md`.
- **Runs** `check_function_statuses.py` (no errors) and `b7_status.py`.
- **Commits** (only its own files, by path; the staged diff checked for
  `/home/`) and pushes the run's branch.

Report, in its last message:

```
map: <name>
file: docs/functions/<name>.md
functions: <rows> (<moved from functions.md>, <new>)
ready groups: <count>, roots: <the ready groups' roots>
blocked by: <blockers outside the subsystem> | none
leads added: <count>
commit: <hash>
stop: none | <reason the user must decide>
```

## Checking a map (the lead)

Before any migration, the lead checks, and halts on any failure:

- `check_function_statuses.py`: 0 errors.
- `b7_status.py`: the new file shows in its table; every function in the
  file is in play.
- The report's `stop:` is `none`, and nothing in the file needs a
  decision only the user can make: a kernel import replaced, behavior
  changed on purpose, a new dependency. Allocating guest memory isn't
  one (since 2026-10-22): native code calls the allocation layer
  (`src/memory/heap.h`: `memory::Alloc`, `Free`, `Realloc`, `AllocPages`,
  `FreePages`, `AllocPhysical`; `docs/native-memory.md`), which every
  allocation, recompiled and native, goes through.
- At least one ready group (`b7_groups.py` on the file's functions). With
  none, the subsystem waits on its blockers: the lead adds them to
  `leads.md` if they aren't there and takes the next lead instead.

## The migration agent

Prompt it with: the function file, this file's "The migration agent",
"The tooling", "Rules that still hold" and "When to halt", and the report
format. One at a time: it edits, builds and runs the game in this tree.
Group by group, ready groups first (`b7_groups.py` on the file's
functions; run again after each group, as callers become ready):

1. Checks `grep -rn sub_X itc_recomp/src` for each member (one override
   per function).
2. Makes the group native ("How a native override is written and
   checked" in `docs/rexglue-removal.md`): plain C++ in the subsystem's
   directory under `src/`, thin `REX_FUNC` adapters, a `--native_<name>`
   flag falling back to `__imp__sub_X`, on by default once checked; each
   plain C++ function starts with `ITC_NATIVE_RUNNING("sub_X Name");`
   ("The validation agent"). A function whose work it does inside a
   native caller (a few lines only that caller needs), with no plain C++
   function of its own, gets the Merged status in its row (the caller,
   the harness covering the work) when it's merged (the validation agent
skips Merged rows). Never
   edits `generated/`. A callee that never ran in play: its branch keeps
   the recompiled path behind the flag, or is ported too, said in the row.
3. Adds harness cases in the same change, registered in `harness.cpp` and
   the flag's help text. Cases keep NaN and infinity inputs, and float
   outputs are compared with `harness::Outputs` ("Comparing floats"
   below), so a NaN's sign or payload is no difference. Cases that
   differ for a real reason that isn't the game's (the fixed-body stack
   leftovers in `expected-differences.md`, say) may go in their own group
   named "... (expected to differ)", with the reason in a comment beside
   it and in the function's row, and the group in the report's `expected
   to differ:` line (the lead records them in
   `docs/loops/expected-differences.md`). No NaN-only "(expected to
   differ)" groups and no explanations of NaN-sign differences (the
   user's call, 2026-10-23). Every other case matches.
4. Builds (`cmake --build itc_recomp/out/build/linux-amd64-release
   -j$(nproc)`) and runs `attract_race.sh 25 --run_harnesses=all`: no new
   error lines, ~20 seconds at 30 ticks/s, every harness at 0 differing
   (outside "(expected to differ)" groups); a stalled boot restarted once.
   If the group runs in the race, a temporary counter override shows it
   was called (removed before committing). For anything drawn, a frame
   dump against one with the fallback flag (attract races put a random
   sepia or grayscale filter over the screen: compare what's drawn, not
   the tint). No profile: native code may
   be slower than the recompiled code for now and is optimized later
   (the user's call, 2026-10-22).
5. On a failure: logging, a run, the log read; no fixes by guessing. If
   that doesn't settle it, it leaves the group uncommitted, says so, and
   reports.
6. Records: the rows (Status by the definitions at the top), then
   `check_function_statuses.py`; what was learned in the subsystem's doc.
7. Commits each group (only its own files, by path; the staged diff
   checked for `/home/`) and pushes the run's branch.
8. Removes the adapters that can go ("Removing an adapter" below): after
   each group, `b7_groups.py --removable` on the subsystem's functions
   (callees become removable as their callers are made native), each
   candidate confirmed, its adapter removed and its row set to Native,
   committed on its own.

### Comparing floats

NaN signs and payloads aren't a goal, thoroughness is (the user's call,
2026-10-23). New harness code (a new harness, or new cases in an
existing one) builds each case's outputs with `harness::Outputs`
(`src/harnesses/harness.h`) and checks them with
`Results::Check(const Outputs&, const Outputs&, what)`:

- `Bytes(...)` and `Word(...)` compare exactly: integers, pointers,
  flags, r3, padding.
- `Float(bits)`, `Floats(base, address, count)` (guest words) and
  `Double(bits)` also match when both sides are NaNs (any sign or
  payload); infinities and every other value must match exactly.
- A value is marked a float only when it's shown to be one: f1 and the
  other float registers, a vector's lanes, a field the function loads
  and stores with float instructions in `generated/`. Everything else,
  and anything unsure, is compared exactly: a float compared exactly
  only fails loudly, while an integer passed as a float could hide a
  difference.
- A case passes only if every entry passes; it counts as NaN-only when
  it passed and some entry matched only as NaNs. The harness line ends
  with "(N NaN-only)". A failing case logs the entry, its kind, its byte
  offset and both values.

The pattern, inside the loop that runs both versions:

```cpp
harness::Outputs out[2];
for (int v = 0; v < 2; v++) {
  // ... set up, run the recompiled (v == 0) or native version on t ...
  out[v].Bytes(Read(h.base, block, 16));          // pointers, flags: exact
  out[v].Floats(h.base, block + 16, 4);           // a vector's four lanes
  out[v].Float(guest::LoadU32(h.base, block + 32));  // a float field
  out[v].Word(t.r3.u32);                          // exact
  out[v].Double(t.f1.u64);                        // f1
}
results.Check(out[0], out[1], what);
```

Existing harnesses keep their byte comparison (they aren't rewritten);
the old `Check(bytes, bytes, what)` stays for them, and their recorded
NaN groups stay in `expected-differences.md` as they are.

### Removing an adapter

A function stays Overridden while its `REX_FUNC` adapter exists, and the
adapter is what recompiled code calls. Once no recompiled code reaches it
in play, the adapter goes and the function is Native (the definitions at
the top of `functions.md`). Removing one can't break anything: without
it, `sub_X` is the recompiled original again, so any recompiled path still
reaching it runs the original, as before; native callers keep calling the
plain C++ directly, and their fallback flags switch only the callers.

`b7_groups.py --removable [function ...]` lists the candidates: every
direct caller seen in play is ours. Functions with no direct caller in
play are reached through a vtable or a function pointer, so their
adapters stay. For each candidate, before removing its adapter:

- Read each caller's adapter and plain C++: it calls the candidate's
  plain C++ directly, and the paths it hands back to the original
  (`__imp__`, edge cases, error paths) aren't the ones that run in play
  (they'd still be correct, as the original runs, but no longer native).
- Grep `itc_recomp/src` for the candidate's adapter being called (other
  adapters or helpers calling `sub_X(ctx, base)`): those calls go to the
  plain C++ first.
- The harness calls the plain C++ and `__imp__sub_X` directly, not the
  adapter. Harnesses written with the adapter call it (once it's gone,
  `sub_X` is the original, and the cases would compare it with itself):
  its register steps move into the harness file as a `Native<Name>`
  function without the flag (the first run's 31 removals did this).
- Its work is plain C++: an adapter whose work makes guest calls through
  `ctx` (the recorders handing chunks to the renderer through its vtable)
  stays, and its row says why.
- A temporary counter in the adapter shows it isn't called in an attract
  race with the flags on (outside the harness); removed by editing.

Then the adapter is deleted, the row set to Native (saying which native
callers reach it, and that the fallback flag now switches only them),
`check_function_statuses.py` run (it refuses Native with an override
left), and the usual build and `attract_race.sh 25 --run_harnesses=all`.

It works in batches (a group, or small related groups together, each
built, raced and committed before the next), one after another in its
own context. It stops when nothing ready is left in the subsystem, after
about five batches (the lead starts another), or at a failure. A massive
migration (a batch too big for one context, such as a group over the
1,500-line cap) may be split: the agent hands its parts to subagents one
at a time, each finishing (committed, or reported failed) before the next
starts, and reports for all of them. Report, in its last
message:

```
migrate: <name>
groups: <root -> commit, one a line>
harnesses: <each "harness: <function>: N cases, M differ" line, verbatim>
expected to differ: none | <function: the group's line, verbatim; the reason>
race: <seconds at 30 ticks/s>, <error lines>
rows: <functions whose rows changed>
native: <functions whose adapters were removed (now Native)> | none
flags: <the --native_ flags added>
left: <ready groups left> | none; blocked by: <blockers> | none
uncommitted: none | <what and why>
stop: none | <reason>
```

## The validation agent

Each native function's plain C++ starts with
`ITC_NATIVE_RUNNING("sub_X Name");` (`src/base/log.h`), which logs
`native running (game): sub_X Name` the first time the game runs it and
`native running (harness): sub_X Name` the first time a harness does
(`--run_harnesses` runs inside `base::InHarnessScope`, a thread-local
flag; once a function and kind, so logs stay readable). Prompt the agent
with: the migration agent's report (its `groups:`, `native:` and `flags:`
lines and commits), this section, "The tooling" and "Rules that still
hold", and the report format. One at a time, after the migration agent
(it builds and runs the game in this tree). For every function the step
made native, or whose adapter it removed:

1. **Checks the print** is the first statement of its plain C++ (grep
   `itc_recomp/src` for the address). Missing ones it adds, in one commit,
   pushed (a print only, no other change).
2. **Runs** the build and `attract_race.sh 25 --run_harnesses=all` with
   the default flags, and checks the log has a `native running
   (harness):` line for each function: its harness ran our code. A
   function with none didn't (a harness calling the adapter, or `__imp__`
   on both sides once its adapter is gone), or its harness doesn't reach
   it. The `(game):` lines list which functions ran in play. No game line
   isn't a failure (a function may only run in menus, saves or on paths
   an attract race doesn't take): those functions go to the replay.
3. **Replays the recorded pad** for the functions with no `native running
   (game):` line (added by the user, 2026-10-22):
   `itc_recomp/tools/scripted_run.sh <the recording in
   itc_recomp/pad_recordings/>` (default flags; the script already passes
   `--log_level=info`, and a flag given twice is refused). Functions with a game line in the replay's log count as
   ran in play. Only what the replay didn't reach goes to the user's
   drive (below). A replay that breaks (stalls, logs new errors, desyncs,
   ends early) is not a halt: the agent says how it broke and where it
   stopped, and every function still without a game line goes to the
   drive list. It doesn't restart or debug the replay.

Functions whose rows are Merged (set by the migration agent) have no
print of their own: the agent skips them.

It fixes nothing but missing prints: a function with no harness line, and
not Merged, goes in the report, and the lead halts. Report, in its last message:

```
validate: <name>
prints added: none | <functions>: <commit>
harness lines: <count> of <count>
missing: none | <functions with no harness line, and why if known>
ran in play: <functions with a game line in the attract race> | none
ran in replay: <functions with a game line only in the replay> | none
replay: ok | broke: <how, where it stopped>
not in game: <functions seen in neither the attract race nor the replay> | none
race: <seconds at 30 ticks/s>, <error lines>, harnesses all 0: yes | no
stop: none | <reason>
```

The lead copies `not in game:` into "To check in a drive" in the progress
file, and notes the `replay:` line beside it when the replay broke (the
native code under test may be the cause). When the loop ends, the user drives a race (menus, a save) with
`--log_level=info` and checks the log's `native running (game):` lines for
those functions: the validation step done by playing.

## When to halt

The lead ends the loop, and writes why under "Stopped" in the progress
file, when:

- a harness differs (outside "(expected to differ)"), or a function made
  native has no harness cases;
- a validation agent reports a function `missing:` (a broken replay is
  not a reason to halt: its functions go to the drive list);
- an attract race failed, stalled twice, logged new errors, or ran under
  ~20 seconds at 30 ticks/s;
- an agent left work uncommitted, or a commit doesn't build;
- a report's `stop:` isn't `none`;
- a map fails its check, or `leads.md` has nothing the loop can take;
- the run's limit is reached;
- the tree has changes the loop didn't make, or isn't on the run's branch.

Whatever the reason, before ending, the lead also brings the run's
summary up to date ("The run's summary") and the B7 status
lines up to date (the subsystems' rows and totals are already there, from
step 9 of the cycle): the "B7 in progress" line near the top of
`docs/rexglue-removal.md` and the B7 sentence in `CLAUDE.md`'s overview
(which subsystems are done, the branch, what the rest waits on), and
commits them with the "Stopped" entry. The user reviews and merges.

## Rules that still hold

Nothing runs in parallel (the user's rule, 2026-10-22): the lead runs
one agent at a time, and an agent that splits its work (a mapping agent's
reading, a massive migration) hands it to subagents one at a time, never
several at once. Agents say in their report if they used subagents.

CLAUDE.md's rules, in short: never edit `generated/`; plain C++ behind
adapters; the SDK's headers used, not reimplemented; the game's use
of the Xbox replaced, never the Xbox or the SDK (plug in data, map Xbox
features without porting them, upstream libraries, no System Link); no absolute paths in
the repo; American English; no `-march=native`; commit often and push the
branch; commit before any revert; stage only your own files; only search
inside the repo; SBX-Recomp only when the user says so; never screenshot
the user's desktop; find problems with logs, not guesses.

## Rows to review

`docs/loops/review.md` (the user's call, 2026-10-23) holds what each run
leaves for the user's review, kept across runs, a section per run, newest
first: the function files added or changed (rows, intros,
`known-data.md`), corrections to earlier maps, and the choices a report
names for review (behavior kept, paths left on the recompiled code, new
helpers). The lead adds to the current run's section after each map and
migration report it checks, and commits it with the progress file. The
user marks a section reviewed. The progress file and run summaries point
to it instead of listing rows.

## Expected differences

`docs/loops/expected-differences.md` (the user's call, 2026-10-23) holds
every "(expected to differ)" harness group with cases that differ, kept
across runs: one table per function file (function, group, differing
cases, reason, review), with the common reasons written once at the top.
When a migration agent's report has a nonzero `expected to differ:`
group, the lead adds its lines there (a new reason gets a line under
"Reasons"), marked "for review (<run> run)", and commits it with the
progress file; the user's agreement makes it "accepted". The other loop
docs (the progress file, the run summaries) point to it instead of
listing groups.

## The progress file

`docs/loops/b7-progress.md`, for the current run. Kept short, so a pass
can read it whole. A run's first pass finds the previous run's file
stopped: it starts it afresh, first copying the old one's drive list
and totals into that run's summary file under "Left for the user" (the
rest is in git; rows to review and expected differences already live in
`review.md` and `expected-differences.md`), and adds a section for the
new run to `review.md`:

- **Now:** the subsystem, the step, the agent running, the run's
  subsystem count.
- **Done:** one line a subsystem (file, groups made native, flags,
  commits), newest first.
- **Rows to review:** a pointer to the run's section of `review.md` only.
- **Expected to differ:** a pointer to `expected-differences.md` only.
- **Totals:** `b7_status.py`'s totals line after each step.
- **Stopped:** when and why, if the loop ended itself.

Details go in commits, the function files and the subsystem's doc, not
here.
