# B7 loop: making the game's functions native, pass by pass

Instructions for a `/loop` session working on B7 ("The game logic
inside" in `docs/rexglue-removal.md`) without the user. Start it with:

```
/loop follow docs/loops/b7-loop.md
```

with no interval: each pass schedules the next one itself. Every pass
starts by reading this file and `docs/loops/b7-progress.md`, so what a
pass needs to know lives in the repo, not in the session's memory.

## Settings (the user sets these before the first pass)

The loop doesn't start until the user has replaced every "to confirm"
below. The suggested values are Claude's, not the user's yet.

- **Subsystem:** to confirm. Suggested: the main thread's performance
  leads (`sub_821298C0`, `sub_8209C6E0` and their callees seen in play;
  "DrawState::Note and AfterDraw" in `docs/native-renderer.md`), as they
  pay off on their own. The other candidate: the boundary's leftovers
  (Voice::Init skipped as high as is safe, the movie player's stream
  set-up).
- **Branch:** to confirm. Suggested: `b7-<subsystem>`, made once from
  `rexglue-removal-project` (the user's approval here counts as asking).
  The loop never makes another branch, never touches `main` or tags, and
  never merges: the user reviews and merges the branch.
- **Code changes without asking first:** to confirm. Suggested: allowed
  on the loop's branch for this subsystem only. The "explain before code
  changes" rule is relaxed by this for the loop's branch, nowhere else.
- **Function rows:** to confirm. Suggested: written to `docs/functions/`
  as the loop goes (the status check after each change), and listed in the
  progress file for one review by the user before the merge.
- **Budget:** to confirm. Suggested: stop after 10 groups, or earlier at a
  stop condition.

## What each pass does

One group a pass: a function and the callees that only it uses, all seen
in play (`itc_recomp/coverage/seen_in_play.txt`), small enough to port and
check in one pass (a handful of functions, not dozens).

1. **Read the state.** This file, `docs/loops/b7-progress.md`, `git status`
   and `git log -5` on the loop's branch. If the tree has uncommitted
   changes the loop didn't make, stop: the user may be editing.
2. **Pick the group.** The next one the progress file names, or else the
   next in the subsystem: start where the `ctx` edge already is (callers
   that are ours or the boundary) and move inward. Check
   `grep -rn sub_XXXXXXXX itc_recomp/src` for each function first: one
   override per function.
3. **Read it.** The functions in `generated/` and their rows in
   `docs/functions/`; note callers and callees, and what's seen in play.
4. **Make it native** ("How a native override is written and checked" in
   `docs/rexglue-removal.md`): plain C++ in the subsystem's directory, a
   thin `REX_FUNC` adapter, a `--native_<subsystem>` flag falling back to
   `__imp__sub_XXXXXXXX`, on by default once checked. Never edit
   `generated/`.
5. **Harness it, in the same change** ("Checking with a harness"): cases
   over its branches, edge cases, and real inputs when known; registered
   in `harness.cpp` and the flag's help text.
6. **Check it.**
   - `cmake --build itc_recomp/out/build/linux-amd64-release -j$(nproc)`
   - `itc_recomp/tools/attract_race.sh 25 --run_harnesses=all`: no new
     error lines, the race reaches 30 ticks/s for ~20 seconds, every
     harness at 0 differing (outside "(expected to differ)" groups).
   - If the group runs in the race, a temporary counter override (the
     harness section's "What an attract race uses") to show it was
     called, removed before committing.
   - For anything drawn, a frame dump (`--native_renderer_dump=<dir>`)
     looked at against one from the fallback flag.
   - Speed, for the performance leads: `profile_render_thread.sh` (with
     `ITC_THREAD=main`), two runs each way, alternating.
7. **Record it.**
   - The rows in `docs/functions/` (one function a row; Status by the
     definitions at the top of the file), then
     `itc_recomp/tools/check_function_statuses.py`.
   - `itc_recomp/tools/b7_status.py`; its totals in the progress file.
   - What was learned in the subsystem's doc, as the repo's docs do.
8. **Commit and push.** Stage only the loop's own files by path; check the
   staged diff for `/home/` first; push the branch to `origin`.
9. **Update the progress file** (below) and schedule the next pass: soon
   (about a minute) after a finished group; when a build or race runs in
   the background, its finishing wakes the loop.

## When to stop and wait for the user

End the loop (and say why in the progress file) when:

- a harness differs and the cause isn't clear from its log after one
  look, or an attract race fails, errors or stalls (restart once: the
  boot sometimes stalls with `--skip_intro`);
- the next step needs the user to play: menus, saves, input feel, or
  anything an attract race doesn't reach and a harness can't cover;
- a fix would mean guessing: add logging, run, read the log ("Find
  problems with logs, not guesses" in CLAUDE.md); if the race can't show
  it, stop;
- the budget is reached, or the subsystem is done;
- a decision belongs to the user (behavior that would change on purpose,
  a new dependency, anything outside the subsystem);
- uncommitted changes the loop didn't make appear.

## Rules that still hold

CLAUDE.md's rules, in short: never edit `generated/`; plain C++ behind
adapters; the SDK's headers used, not reimplemented; no absolute paths in
the repo; American English; no `-march=native`; commit often and push the
branch; commit before any revert; only search inside the repo; SBX-Recomp
only when the user says so; never screenshot the user's desktop.

## The progress file

`docs/loops/b7-progress.md`, made by the first pass. Kept short, so a pass
can read it whole:

- **Now:** the branch, the group in progress or next, and why.
- **Done:** one line a group (functions, flag, commit), newest first.
- **Rows to review:** the rows added or changed, by function, for the user.
- **Totals:** `b7_status.py`'s line after each group.
- **Stopped:** when and why, if the loop ended itself.

Details go in commits and the subsystem's doc, not here.
