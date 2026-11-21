---
name: e5-swarm
description: Run or resume E5 step 2's swarm (parallel agents converting the game's areas to the schema's views), then validate it. Use only when the user types /e5-swarm; invoking it is the user's go-ahead to run the Workflow.
---

# E5 step 2's swarm

The user invoked this skill: that is their request to run the swarm as a
Workflow. Arguments: none (start or continue from where the repo says it
stands), `resume` (resume this session's stopped run), or `validate` (skip
the swarm; only the lead's validation).

## Before starting

1. Be on branch `e5-native-16-11-26` (`git branch --show-current`); if
   not, stop and ask. The tree should be clean (`git status --short`):
   if it isn't, commit it as a WIP snapshot first (explicit paths, no
   `/home/` paths, push).
2. Read `docs/loops/e5-loop.md`, section "Step 2 as a swarm" (where it
   stands, "To resume", what the lead does after), and the "For the
   lead" notes it points to. `docs/e5-native.md` step 2 is the why.

## Run

- **resume**, in the session that ran it: Workflow with the same
  `scriptPath`, `resumeFromRunId` and args as the stopped run.
- Otherwise: Workflow with `scriptPath`
  `itc_recomp/tools/workflows/e5-step2-swarm.js` and args
  - `areas`: the areas "Where it stands" lists as left to convert (if
    none, skip to the validation);
  - `cross_areas`: every area with a schema file once those are done:
    the names in `itc_recomp/schema/*.toml` without `.toml`, plus the
    `areas` above that have none yet.
- At most 6 agents at a time (the script's pool; the user's call). Don't
  change the agents' rules in the script (no builds, no git, own files
  only).

## If it stops (a usage limit, an error)

1. Commit the tree as a WIP snapshot (explicit paths, push).
2. Update "Where it stands" in `docs/loops/e5-loop.md` (areas done and
   left) and add the new reports to `docs/loops/e5-swarm-reports.md` (from
   the Workflow's `journal.jsonl`: the latest `result` per area; no
   `/home/` paths), commit, push.
3. Tell the user where it stopped and when the limit resets, and stop.
   Never resume on your own: only when the user's current message says to
   (or types `/e5-swarm resume`).

## When it finishes: the lead's validation

From the repo root, in order, fixing as you go:
1. `python3 -I itc_recomp/tools/schema_gen.py` (full) and
   `python3 -I itc_recomp/tools/schema_check.py` until clean.
2. `python3 -I itc_recomp/tools/lift.py reemit` and
   `python3 -I itc_recomp/tools/lift_originals.py --all`.
3. The build, in the background (10+ minutes):
   `cmake --build itc_recomp/out/build/linux-amd64-release -j$(nproc) -- -k 0`;
   fix what doesn't compile (the area agents never built).
4. `timeout 120 itc_recomp/tools/attract_race.sh 40` and
   `timeout 240 itc_recomp/tools/scripted_run.sh itc_recomp/pad_recordings/race_test.jsonl`
   (no error lines; the race at the frame cap), `pkill -9 -x itc` after
   each; a frame dump (`--native_renderer_dump=<dir>`). A crash or a
   call to an address with no entry: find it in the log, fix, run again.
5. Commit and push; update `docs/loops/e5-loop.md`, `docs/e5-native.md`
   (step 2's record) and CLAUDE.md's E5 line; then the lead's list in the
   loop file ("For the lead after the swarm": the measure's offset fault
   first). Report to the user what passed, what was fixed, and what's
   left.
