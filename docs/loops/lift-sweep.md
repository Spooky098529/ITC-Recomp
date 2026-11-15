# B7 lift sweep: B7's end state, reached by a tool

The user's call (2026-11-15): B7's end state now rather than subsystem
by subsystem. That is native C++ for every function `main` reaches,
calling each other directly, with no `ctx` and no adapters. It still
calls into the SDK where it must (the kernel and XAM imports). This is
an alternative to the B7 loop (`b7-loop.md`). It has the same goal
("B7's goal" there) and is measured by the same static map
(`static-map.md`), but it is reached differently: a tool translates
the functions still recompiled straight from `generated/`, agents
rewire the existing ports to call natively, and validation is done
once at the end.

## The pieces

1. **The lift tool** (`itc_recomp/tools/lift.py`). It reads a
   function's body in `generated/` (never edits it) and writes plain
   C++ in `itc_recomp/src/lifted/`:
   - the function's arguments become typed parameters, worked out from
     the registers it reads before writing them (r3 to r10, f1 to f13,
     the stack pointer, r13 when it reads it);
   - its return value is r3 (or f1), when it writes one;
   - registers become plain locals;
   - guest memory is still read and written through `guest::`;
   - calls to lifted functions are direct C++ calls;
   - other calls go through the dispatch table (below);
   - imports go through the import helper (below);
   - the print (`ITC_NATIVE_RUNNING`) comes first, as in every port.

   It refuses anything it can't translate exactly and lists the
   function with the reason. Expected refusals:
   - vector code;
   - the spin locks that touch the machine state register;
   - a function that reads another's leftover registers after a call;
   - a function that reads a caller's saved registers.

   Refused functions go to agents, who port them by hand.
2. **The dispatch table** (`src/lifted/dispatch.h`). Native code calls
   a guest function pointer or vtable slot by its address. Vtables and
   function pointers stay guest data, since the game keeps them in
   guest memory. The table maps each native function's address to a
   uniform entry (the argument registers in, r3 and f1 out) that unpacks
   them into the function's typed parameters. While some functions are
   still recompiled, an address not in the table runs the recompiled
   function: that fallback goes away with the last of them.
3. **The SDK boundary** (`src/lifted/sdk.h`). Imports take the SDK's
   context, so a helper builds one for each import call from the
   arguments. The other way, the SDK runs game code by its address: the
   entry point, thread start routines and the callbacks it's given.
   These keep a thin entry from the SDK's context into the dispatch
   table. They are the only adapters left at the end.
4. **Rewiring the existing ports.** Ports made so far reach code that
   isn't native through `Calls` callbacks the adapter gives (about
   1,200 call sites). Agents replace those with direct calls, subsystem
   by subsystem, and delete each adapter once nothing recompiled calls
   it. Until then, a lifted function the recompiled code or a
   `Calls` callback still reaches gets a transitional entry, to be
   deleted the same way.

## What's loosened

- **Behavior:** the lifted code follows the recompiled code statement
  by statement, so it should match. "B7: no behavior change" is
  loosened only for what doesn't matter, such as the bytes register
  saves leave in a stack frame.
- **No mapping step and no harness per function.** Rows are written by
  the tool: "Lifted by `lift.py`", what it could tell (callees,
  callers, imports), "Not seen in play" where it applies. Nothing is
  read in detail.
- **Readability:** lifted code may read like the recompiled code
  without `ctx` (one local per register, `goto`s between labels).
  Cleaning it up is for later, if ever.

Everything else under "Rules that still hold" in `b7-loop.md` still
holds.

## Scope

Every function the static map lists as unported, seen in play or not,
outside:
- the replaced libraries;
- System Link;
- the Xbox Live (type 12) and two-player (type 8) modes. Their main
  menu items are turned off (`--multiplayer_menu_items`, off by
  default) precisely so they need no port. The static map's count of
  what's left should leave them out too.

## Steps

1. **Runtime:** the dispatch table and the SDK boundary.
2. **Prototype:** the tool on about 20 functions of different shapes
   (getter, setter, thunk, destructor, a loop, floats, a vtable call,
   an import). Each is harnessed against the recompiled function, and
   the tool is fixed until all match. The user sees the output before
   the tool goes wide.
3. **Sweep:** batches in the static map's order (callees before
   callers). Each batch must build and pass `attract_race.sh 40` as a
   smoke run (boots, races, no new errors). Commit and push each
   batch.
4. **Rewire** the existing ports (piece 4).
5. **Validate at the end:**
   - a harness sample: about 5% of the lifted functions, plus one of
     every construct the tool translates;
   - the attract race and the pad replay;
   - a drive by the user;
   - problems found with logs, as always.

   Once adapters are gone, harnesses can't swap single functions any
   more, so the game running is the main check from then on.
6. **Record:** `static_map.py`, `boundary_status.py` and `b7_status.py`
   run, their totals in "The boundary over time"
   (`docs/rexglue-removal.md`), and a summary like the loop's.

## Progress

- 2026-11-15: the runtime and the tool; the prototype (21 functions,
  harness `lifted`); then the sweep: 2,749 functions lifted in 29
  batches (`src/lifted/batch_000.cpp` to `batch_028.cpp`). Fixed on the
  way: jump-table fragments the recompiler split off are folded into
  their parents (the static map's `folded` column), a return reads what
  the function returns, a conditional return returns the function's
  value, and existing overrides made by macros are found through
  `src_edges.py`. Every harness at 0, the pad replay clean. Static map:
  unported 3,106 -> 329 (205 vector code, 64 with an override of ours,
  the rest reading registers another function left or their caller
  saved, or the global lock).
- The batch flags only switch how the SDK and ported code enter lifted
  functions: lifted functions call each other directly, so bisecting by
  flags doesn't isolate a bad function; harness it directly instead (add
  it to `src/harnesses/lifted.cpp`).
- Rows (the user's call, 2026-11-15): a new status, **Lifted** (defined
  at the top of every function file; `check_function_statuses.py`,
  `b7_status.py`, `static_map.py` and `b7_groups.py` know it). The 1,172
  lifted functions that had a row (all with a blank status) have it set
  to Lifted where they are; the other 1,577 have rows in
  `docs/functions/lifted.md`, written by the tool (what each calls,
  whether it was seen in play, "Not read").
- Vector code (2026-11-15): `lift.py` lifts it too. Vector registers
  are locals of our own 16-byte `Vec` (`src/lifted/lifted.h`) with the
  generated code's views; the simde calls and the SDK's helpers for
  instructions SSE has no single match for (`rex/ppc/intrinsics.h`,
  included, not copied: the user's call) stay as the generated code
  wrote them; element writes count as the register's write (its other
  lanes start at 0); statements split over lines are joined; the vector
  float mode is the host's MXCSR (`VectorFloatMode`). 188 more functions
  (batches 029 and 030), 2,937 in all. The lifted harness covers six
  vector functions, and starts every run with the host float mode and
  the SDK context's cached copy of it in step: the recompiled code skips
  a mode switch its stale copy says is done, which looked like a
  difference. Static map: unported 329 -> 141.
- The dispatch table (`lifted::Call`) stays while the game's objects
  and their vtables live in guest memory as 32-bit addresses; it goes,
  class by class, if a later decompilation moves them into native C++
  classes (the user's question, 2026-11-15). It's our code, not
  rexglue's: its fallback into the SDK's function table goes once
  every function is native.
- Registers kept across calls (2026-11-15): every "reads a register a
  call left" refusal was a caller keeping a value in a volatile register
  across a call its compiler knew doesn't touch it (none was an extra
  output). `lift.py` works out what each function may write, through its
  calls, and a direct call wipes only that; the SDK boundary
  (`lifted::Call`'s fallback, `Import`) puts back the argument registers
  it loads into the context (r4-r10, f2-f13, v1-v13), so a recompiled
  caller further up keeps its values too. `__chkstk` (`sub_825A9934`,
  Dropped) calls are left out. 64 more functions, 3,001 in all.
- The Wrapped functions (2026-11-15): by the user's call, B7-style
  migration agents rather than lifting: 60 of 67 made native or Dropped
  (their wrappers' own work kept). Left: `Device::Resolve`,
  `ScreenObject::Record` and `sub_821743A8` (probably the network
  battle's placement), and four in `825BD8F0`-`825BE300`.
- Left (static map: unported 17): those, the global lock's six, two
  with an SDK macro, `xstart`, `sub_822997B0` (an `r8` read after a
  call) and `sub_82121760` (an unfolded fragment); then rewiring
  (piece 4).
- Wrapped functions (2026-11-15, after the render path's): the game's
  work inside our wraps made native by hand, the wraps kept: the action
  mappers' last read slots, the sound manager's per-frame callees,
  `AudioRenderDriver::Destroy`, `PauseMenu::ShowVolume`,
  `XUserWriteAchievements`' XAPI wrapper, `CreateFile`, `Race::DrawStep`,
  `Modes::Tick`, the world-space sprites' recorders and
  `RaceManager::GiveVehicle`. Their plain C++ calls other functions
  through the dispatch table with no `ctx` (`src/hooks/lifted_call.h`:
  the adapter enters it through `lifted::Enter`, so calls to code that
  isn't native yet go through the boundary). Left recompiled:
  `ScreenObject::Record` (`sub_82130978`: 1,038 instructions of render
  path that returns at once in every call seen, so frame dumps can't
  check it) and `sub_821743A8` (the network battle's cars put at their
  sent places, with system link sends: out of scope).
- The last Wrapped functions (2026-11-15): the draw library's inline
  and non-indexed draws (`BeginVertices`, `EndVertices`,
  `BeginIndexedVertices`, `DrawVertices`, `--native_inline_draws`) and
  `Device::Resolve` (`--native_resolve`) made native inside their wraps,
  their GPU packets Dropped and what the game and our renderer read back
  kept (checked by temporary comparisons with the originals on copies of
  the device, 0 differing, and frame dumps); `ScreenObject::Record` as
  `lift.py`'s translation inside its wrap (`--native_screen_objects`;
  checked with the nitrous forced on). `sub_821743A8` left recompiled:
  only the Xbox Live mode's network battle (race kind 6 with two human
  cars, `RaceWorld::Init`'s +9608) reaches it, out of scope.

## The user's calls (2026-11-15)

- The end state is the goal: native C++ calling native C++, no `ctx`,
  no adapters, needing rexglue only for guest memory, the kernel's
  functions and the SDL window. The generated code goes with the
  dispatch table's fallback into the SDK, once every function is native
  and the refused ones are ported by hand.
- Parameter lists made long by calls through vtables and imports (each
  assumed to pass every argument register) are left for later.
- The lifted code's wordiness isn't cleaned up: most of it is unlikely
  to run.
- Lifted-looking code is fine for every size of function.
- The sweep runs on its own branch, `lift-sweep`, from
  `rexglue-removal-project`.
- The Xbox Live and two-player modes' code is skipped (Scope).
