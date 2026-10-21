# B7 loop: making the game's functions native, group by group

Instructions for a `/loop` session working on B7 ("The game logic
inside" in `docs/rexglue-removal.md`) without the user. Start it, in a
fresh session (`/clear` first), with:

```
/loop follow docs/loops/b7-loop.md
```

with no interval: each pass schedules the next one itself. The session
running the loop (the **lead**) does no porting itself: it hands groups to
subagents, reads their reports, and decides what's next or halts. So its
own context grows by a few reports a pass, and each group starts with a
fresh agent. What a pass needs to know lives in the repo (this file,
`docs/loops/b7-progress.md`, the surveys, commits), not in the session's
memory.

## Settings (the user sets these before the first pass)

The loop doesn't start until the user has replaced every "to confirm"
below. The suggested values are Claude's, not the user's yet.

- **Roots:** to confirm. Suggested: the main thread's performance leads,
  `sub_821298C0` (60 lines, ready) and `sub_8209C6E0` (475 lines, ready, one
  callee never seen in play) ("DrawState::Note and AfterDraw" in
  `docs/native-renderer.md`), then their callers as they become ready.
- **Branch:** to confirm. Suggested: `b7-main-thread`, made once from
  `rexglue-removal-project` (the user's approval here counts as asking).
  The loop never makes another branch, never touches `main` or tags, and
  never merges: the user reviews and merges the branch.
- **Code changes without asking first:** to confirm. Suggested: allowed
  on the loop's branch, for the groups under the roots only. This relaxes
  the "explain before code changes" rule for the loop's branch, nowhere
  else.
- **Function rows:** to confirm. Suggested: written to `docs/functions/`
  by the porting agent (the status check after each change) and listed in
  the progress file for one review by the user before the merge.
- **Passes per session:** to confirm. Suggested: 5. Then the loop ends
  itself; the user runs `/clear` and starts it again (nothing is lost: it
  picks up from the progress file).
- **Groups in all:** to confirm. Suggested: 10, then the loop stops for
  the user's review.

## Groups

A group is a small set of functions made native together and checked
together. Native code is plain C++ without `ctx` ("Plain C++ behind thin
adapters" in `docs/rexglue-removal.md`), so it can only call code that's
already ours: functions become native from the bottom of the call graph
up. For a function F (the group's root), its group is F plus the callees
only the group calls, seen in play and not yet ours, followed down.

- **Ready:** every callee outside the group that runs in play is ours.
  The loop takes ready groups only. (A group still calling recompiled code
  could be made Overridden, but that's the user's call, not the loop's.)
- **Within the cap:** 1,500 generated lines at most (lines, not functions:
  sizes run from 10 lines to thousands).
- **Callees never seen in play:** native code replaces the whole function,
  so their branches need a decision: port them too, or keep the recompiled
  path for that branch behind the fallback flag. The survey proposes; the
  porting agent does it.
- **Indirect calls** (through a register: vtables, callbacks) aren't in
  the static graph; the survey reads which targets run (a temporary
  counter override in an attract race, if reading isn't enough).
- **Kernel imports** the group calls (mostly the core's locks and memory)
  stay as they are through the SDK; a group that would need one replaced
  goes back to the user.
- **The heap:** the functions holding the most groups back are the guest
  heap's (`sub_825A7410`, `sub_825A71D0`, `sub_825A7068`'s group; 2026-10-22).
  Native code that allocates guest memory needs a way to do it without
  `ctx`; until the user decides how, a group needing it isn't ready.

`itc_recomp/tools/b7_groups.py` computes them (reports only):

- `b7_groups.py <root>...`: each root's group, whether it's ready, the
  ready groups under it (largest first) and the blockers under it.
- `b7_groups.py`: the whole game: how many groups are ready, and the
  blockers holding the most back.

Order: the ready groups under the roots, smallest first; once a group is
ours, run the script again, as its callers may have become ready (that's
the way up). When nothing under the roots is ready, stop for the user.

## The pass (the lead)

1. **Read the state.** This file, `docs/loops/b7-progress.md`, `git status`
   and `git log -5`. On the wrong branch, or with uncommitted changes the
   loop didn't make: stop (the user may be editing).
2. **Collect reports** from agents that finished since the last pass
   (their notifications). Apply "When to halt" below to each.
3. **Plan.** Run `b7_groups.py <roots>` and `b7_status.py`. The queue:
   groups surveyed (a file in `docs/loops/surveys/`) and not yet ported,
   then ready groups not yet surveyed.
4. **Hand out work** (the Agent tool; agents run in the background and
   notify when done):
   - **Porting:** if no porting agent is running and a surveyed group is
     queued, start one for the next surveyed group. One at a time: it
     edits, builds and runs the game in this tree.
   - **Surveys:** keep up to three surveyed groups ahead of the porting
     agent: start a survey agent for each ready group not yet surveyed,
     up to three running at once. They only read, so they can run beside
     the porting agent.
5. **Record.** Update the progress file (below); commit and push it.
6. **Next pass.** Count the pass; at the session's limit, end the loop
   (say so in the progress file). Otherwise schedule the next pass: when
   agents are running, their notifications wake the loop, so a long
   fallback (about 30 minutes); when nothing runs and there's work, about
   a minute.

## The survey agent (read-only)

Prompt it with: the group (its root, members, unseen callees, indirect
calls and imports, from `b7_groups.py`), this file's "Groups" and
"Rules that still hold", and the report format below. It reads:

- each member in `generated/` (`grep -n "DEFINE_REX_FUNC(sub_X)"
  itc_recomp/generated/default/*.cpp`), its rows in `docs/functions/`, and
  the docs they point to;
- its callers (`grep -ln "lr = 0xXXXXXXXX;"`) and how often it runs, where
  known.

It writes `docs/loops/surveys/<root>.md`, its only file: what each member
does (arguments, return value, guest memory read and written), its
callers, the native shape proposed (the plain C++ function's signature,
where it goes in `src/`, the adapter, the flag's name), the decision for
each unseen callee and indirect call, harness cases worth having (branches,
edge cases, real inputs), and risks (float rounding, big-endian layout,
order of side effects). It doesn't build, run the game, or change anything
else. Report, in its last message:

```
survey: <root>
file: docs/loops/surveys/<root>.md
ready: yes | no (why)
size: <functions>, <lines>
stop: none | <reason the user must decide>
```

## The porting agent (one at a time)

Prompt it with: the survey file, this file's "Rules that still hold" and
"When to halt", and the report format below. It:

1. Checks `grep -rn sub_XXXXXXXX itc_recomp/src` for each member (one
   override per function).
2. Makes the group native as the survey proposes ("How a native override
   is written and checked" in `docs/rexglue-removal.md`): plain C++ in the
   subsystem's directory, thin `REX_FUNC` adapters, a `--native_<name>`
   flag falling back to `__imp__sub_XXXXXXXX`, on by default once checked.
   Never edits `generated/`.
3. Adds harness cases in the same change ("Checking with a harness"),
   registered in `harness.cpp` and the flag's help text.
4. Checks:
   - `cmake --build itc_recomp/out/build/linux-amd64-release -j$(nproc)`
   - `itc_recomp/tools/attract_race.sh 25 --run_harnesses=all`: no new
     error lines, ~20 seconds at 30 ticks/s, every harness at 0 differing
     (outside "(expected to differ)" groups). A stalled boot is restarted
     once.
   - If the group runs in the race, a temporary counter override to show
     it was called, removed before committing.
   - For anything drawn, a frame dump (`--native_renderer_dump=<dir>`)
     against one with the fallback flag.
   - For the performance leads, `ITC_THREAD=main
     itc_recomp/tools/profile_render_thread.sh`, two runs each way,
     alternating.
5. Records: the rows in `docs/functions/` (one function a row; Status by
   the definitions at the top of the file), then
   `itc_recomp/tools/check_function_statuses.py`; what was learned in the
   subsystem's doc; deletes the survey file (what it knew is now in the
   rows and docs).
6. Commits (only its own files, by path; the staged diff checked for
   `/home/`) and pushes the branch.

On a failure it doesn't fix by guessing: it adds logging, runs, reads the
log; if that doesn't settle it, it leaves the work uncommitted, says so,
and reports. Report, in its last message:

```
port: <root>
commit: <hash> | none
harnesses: <each "harness: <function>: N cases, M differ" line, verbatim>
race: <seconds at 30 ticks/s>, <error lines>
profile: <before -> after cycles a frame> | not run
rows: <functions whose rows were added or changed>
flag: --native_<name>
stop: none | <reason>
```

## When to halt

The lead ends the loop, and writes why under "Stopped" in the progress
file, when a report shows:

- any harness differing (outside "(expected to differ)"), or a harness
  missing for a function made native;
- an attract race that failed, stalled twice, logged new errors, or ran
  under ~20 seconds at 30 ticks/s;
- a porting agent that left work uncommitted, or a commit that doesn't
  build;
- `stop:` anything but `none` (from either agent);
- a profile slower than before, beyond the runs' spread.

And also when: nothing under the roots is ready, the groups in all are
done, a group needs the heap or a kernel import replaced, or the tree has
changes the loop didn't make. Agents still running when the loop halts
are left to finish; their work isn't merged or built on until the user
has looked.

## Rules that still hold

CLAUDE.md's rules, in short: never edit `generated/`; plain C++ behind
adapters; the SDK's headers used, not reimplemented; no absolute paths in
the repo; American English; no `-march=native`; commit often and push the
branch; commit before any revert; stage only your own files; only search
inside the repo; SBX-Recomp only when the user says so; never screenshot
the user's desktop; find problems with logs, not guesses.

## The progress file

`docs/loops/b7-progress.md`, made by the first pass. Kept short, so a pass
can read it whole:

- **Now:** the branch, the pass count this session, the agents running
  (which groups), the queue.
- **Done:** one line a group (root, functions, flag, commit), newest first.
- **Rows to review:** the functions whose rows were added or changed, for
  the user.
- **Totals:** `b7_status.py`'s totals line after each group.
- **Stopped:** when and why, if the loop ended itself.

Details go in commits, the function rows and the subsystem's doc, not
here.
