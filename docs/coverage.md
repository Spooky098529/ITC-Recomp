# Which recompiled functions run: coverage

Started 2026-10-21 on branch `coverage` (off `rexglue-removal-project`).
The question: of the 16,552 recompiled functions in `generated/`, how many
does the game actually run? That number, not 16,552, is how much game code
B7 ("The game logic inside" in `docs/rexglue-removal.md`) has to deal
with.

## Why the count is so high

The game's code (`.text`, `0x82090000` + `0x54C064`) is about 5.5 MB,
~1.39 million PowerPC instructions: ~84 instructions a function, an
ordinary size for compiled C++. Much of it isn't the game's own code but
the Xbox libraries linked into it (the C runtime, the graphics library, the
audio library, the movie player, the system wrappers), and much of it never
runs: B5 found 51 of its 80 boundary functions never called in play, and the
game's System Link code is unreachable. About 4,950 of the functions come
from `itc_recomp/itc_functions.toml`, entry points `tools/scan_ptrs.py`
found by scanning the image for code pointers (the game has no RTTI, so
ReXGlue's vtable scanner finds none); some of those may be data that only
looked like a pointer.

## Two ways to find what doesn't run

### The static pass (a quick look)

`itc_recomp/tools/static_reachability.py <image> [--unreachable <file>]`
(the image from `--dump_image`). It starts from the roots (`xstart`, every
function start found as a word in `.rdata`/`.data`, every guest function
`src/` names) and follows direct calls and `lis`/`addi` address constants
in a function's own code. `.pdata` (between `.rdata` and `.text`) is left
out on purpose: it lists every function.

2026-10-21: 1,197 of 16,552 functions unreachable (5.5% of the code). It
can't do much better, since vtables point at every method of every class,
used or not. It also misses addresses built across registers (`lis r9,hi`,
later `addi r28,r28,lo`): 23 functions an attract race ran were
"unreachable" to it. Use it for a look without running the game, not as
the measure.

### The coverage build (the measure)

A second executable, `itc_coverage`, whose copy of the recompiled code
marks each function the first time it runs (clang's
`-fsanitize-coverage=func,inline-bool-flag,pc-table`: one flag a function,
and a table of their addresses in the same order).

- `ITC_COVERAGE` in `itc_recomp/CMakeLists.txt` (off by default) adds the
  target; it's `EXCLUDE_FROM_ALL`, so a normal build skips it. Only
  `itc_coverage_recomp` (the recompiled code) is instrumented; our `src/`
  and the SDK aren't, and `generated/` isn't touched.
- It's a target of the release build directory, not a build directory of
  its own: the SDK writes its libraries to `rexglue-sdk/out/<platform>`
  whatever the build directory, so a second one would overwrite the
  release's `librexruntime.so`.
- The instrumented code is built with `-fno-lto`: under ThinLTO the
  coverage tables' private globals (`__sancov_gen_*`) were undefined at the
  link. The rest of the executable keeps ThinLTO.
- `src/debug/coverage.cpp` takes the flags and the table when the module
  starts, and writes the functions whose flag is set to
  `coverage/seen_<pid>.txt` next to the executable every 5 seconds and at
  exit (addresses relative to the load address, as `nm` prints them).
  Linux only.
- `itc_recomp/tools/coverage_seen.py <itc_coverage> <seen files>...
  [--dry-run]` maps them to guest addresses (`nm`, then
  `itc_register.cpp`) and adds them to `itc_recomp/coverage/seen_in_play.txt`.

Why optimization doesn't hide functions (so no debug build is needed): a
function inlined into its caller wouldn't set its flag, but generated code
calls other functions through their weak `sub_X` symbols, which can't be
inlined, and indirect calls go through the function table. On Linux `sub_X`
is an alias of `__imp__sub_X` (the thunk form in `itc_pch.h` is macOS's),
so each function is one symbol with one flag. No function is dropped as
unused: all are registered in the function table. Checked 2026-10-21:
16,562 flags in the binary (the 16,552 functions and a few helpers in the
generated files); in a test run, 4,975 of the 4,976 flags set mapped to a
recompiled function. Uncapped it ran at ~400 fps, against ~430 for `itc`.

What a run counts: a function whose recompiled body ran. An override in
`src/` that no longer calls its `__imp__` leaves the function out (it's
ours already).

## Running a session

```sh
cmake -S itc_recomp -B itc_recomp/out/build/linux-amd64-release -DITC_COVERAGE=ON   # once
cmake --build itc_recomp/out/build/linux-amd64-release -j$(nproc) --target itc_coverage
# run $B/itc_coverage with the standard flags (CLAUDE.md), play, close the window
python3 -I itc_recomp/tools/coverage_seen.py itc_recomp/out/build/linux-amd64-release/itc_coverage \
  itc_recomp/out/build/linux-amd64-release/coverage/seen_*.txt
```

A session should touch everything once: boot and the intro movies, the
front end's menus, a career save loaded and saved, a race (opponents, the
pause menu and its sub-menus, the rival intro), the garage and tuning,
and closing the window. Sessions add up: a function is in the list once any
session ran it, so later sessions only add what earlier ones missed. Delete
the build's `coverage/` folder between sessions so old files aren't counted
again (harmless, but slower).

Once (2026-10-25), the list was made again instead of added to: the
HLSL compiler's tree (~640 functions, only under `sub_822B8F10`) had run
at boot in the first session, and `--use_hardcoded_shaders` stopped it, so
those would have stayed in the work left. The second session's list
replaced the first's, keeping the first's functions that neither run
under an override now nor sit in the compiler's tree (136: paths the
second session didn't take). Do the same after another replacement
leaves a tree that ran before. Done again on 2026-10-25 after the movie library was
replaced (`--play_movies_with_ffmpeg`, `native-movies.md`): the third
session's list, with the second's functions that neither are ours nor sit
in the library's range (`82481A70`-`82575B07`).

## What's kept, and what isn't

- `itc_recomp/coverage/seen_in_play.txt`: the functions seen in play, one
  address a line, sorted. Generated by `coverage_seen.py`, never edited by
  hand, and not meant to be read through: tools join it with other data.
- No rows or lists for functions that never ran, in `docs/functions/` or
  anywhere else (the user's call, 2026-10-21: a list of dead functions to
  maintain would only grow). A function gets a row in `docs/functions/`
  the way it always has: when it's read, overridden or learned about.
- The totals go in "The boundary over time" in `docs/rexglue-removal.md`,
  with this doc's results below.

## How it relates to B7

B7 makes the game's own logic native, subsystem by subsystem, and the plan
asks for a measure of its progress (per-subsystem counts of the statuses in
`docs/functions/`). Coverage gives it a denominator:

- **The work left:** functions seen in play that aren't ours yet (no
  override, not Native or Dropped). Functions that never run don't need
  porting; when B7 replaces a subsystem from the top, the ones below it that
  never ran go with it, unread.
- **The measure:** `itc_recomp/tools/b7_status.py` (reports only; it reads
  `docs/functions/` with `check_function_statuses.py`'s code). A function
  is in play when it was seen in play or is ours (an override's recompiled
  body no longer runs, so coverage can't see it), and counts once: **ours**
  (overridden, Native or Dropped), **wrapped** (Wrapped: ours in front, the
  recompiled body does the work), **known** (a row, still recompiled) or
  **unknown** (no row yet). Wrapped, known and unknown are the work left.
  It counts them by table (the subsystem files, and "(no row)"), and lists
  as checks the rows that say "never called" whose function ran. (Until
  2026-10-23 it also flagged Native, Dropped and overridden functions that
  ran; dropped, the user's call: the list predates most of them, so every
  one was flagged, and the function tables and the native-running logs
  show what's ours and that it runs.) Run it after each B7 item and record its totals in "The
  boundary over time".
- **Choosing what to do next:** a subsystem's size in functions that
  actually run is a better guide to its cost than its size in `generated/`.
- **Removing the core** (E1-E4) needs no recompiled code left that runs.
  Code that never runs still has to go, but by deleting it, not porting it.

A function seen in play is certain; one not seen may still run on a path no
session took (an error, a rare menu branch). Before B7 relies on a function
never running (to delete it, say), check it with the static pass or by
reading its callers, the way B4's and B5's "Never called" rows were
checked over several sessions.

## Results

| Date | Sessions | Seen in play | Notes |
|---|---|---|---|
| 2026-10-21 | a 20-second attract race (`--skip_intro`) | 4,975 of 16,552 (30%) | The test run; not written to the list |
| 2026-10-21 | the user's first session (~5.5 minutes, closed cleanly) | 6,656 of 16,552 (40.2%; 39.8% of the generated lines) | The list's first entries. 208 of them were "unreachable" to the static pass. The user judged it most of what's needed: the game's loop is simple and reuses its systems (boot and a 20-second attract race already ran 4,975) |
| 2026-10-25 | the user's second session (~9.5 minutes, closed cleanly), with `--use_hardcoded_shaders` | 5,135 ran; the list 5,271 with the first's 136 (5,206 of 16,315 `sub_` functions) | Made again, not added to (above). Of the first list's 6,656, 2,438 didn't run: 1,663 ours since, 639 the compiler's tree, 136 kept. 917 new, almost all without rows: front-end and career paths (422 of their entry points only reached indirectly) |
| 2026-10-25 | the user's third session (~9.7 minutes, closed cleanly; the logo, `OPENING` and `STAGE1_OPENING` played through FFmpeg), with `--play_movies_with_ffmpeg` | 4,094 ran; the list 4,674 with the second's 580 (4,609 of 16,315 `sub_` functions) | Made again (above). Of the second list's 5,271, 1,207 didn't run: 10 ours since, 617 the movie library's, 580 kept. 30 new. Seven in the library's range still run: shared helpers (`sub_8248F030` and the like) |

`b7_status.py` after the first session (2026-10-21). It counts the 16,315
`sub_` functions (the register save and restore helpers and `xstart` left
out), of which 6,591 were seen in play:

```
in play (seen, or ours): 6752
  ours 162, work left 6590 (wrapped 151, known 174, unknown 6265)
rows: 673; for functions not seen in play: 186

table                         ours   wrapped     known   unknown   in play
audio.md                        29         0         9         0        38
files-and-saves.md              44         0        29         0        73
functions.md                    61       151       118         0       330
system-services.md              28         0        18         0        46
(no row)                         0         0         0      6265      6265
```

So B7 is ~6,600 functions, ~6,300 of them not yet looked at. ("rows" counts
functions named in rows: some rows name several, 673 functions in 548
rows.) One check: `DrawIndexedVertices` (`sub_825BE708`, Overridden) ran,
as its override hands predicated draws and `--native_renderer_library_packets`
to the original from `src/native/draw_packets.cpp` (a file the status
check didn't read for it; since 2026-10-22 it counts a fallback to
`__imp__sub_X` from any file in `src/` outside `src/harnesses/`, including
calls built by macros, so this check is gone).

After B7's first loop run (2026-10-22, branch `b7-loop-test`; "B7's first
loop run" in `docs/rexglue-removal.md`), still counted against the first
session's list:

```
in play (seen, or ours): 6753
  ours 335, work left 6418 (wrapped 142, known 243, unknown 6033)
```

Its one check left is the stack probe (`sub_825A9934`), Dropped since but
seen running in that session.

Against the second session's list (2026-10-25, after B7's thirteenth loop
run and the hardcoded shaders): more work left than before, as the 917
new functions outweigh the compiler's 639:

```
in play (seen, or ours): 7039
  ours 1839, work left 5200 (wrapped 117, known 644, unknown 4439)
rows: 2793; for functions not seen in play: 193
```

Against the third session's list (2026-10-25, the movie library
replaced): 596 fewer functions left.

```
in play (seen, or ours): 6451
  ours 1847, work left 4604 (wrapped 113, known 651, unknown 3840)
rows: 2824; for functions not seen in play: 213
```
