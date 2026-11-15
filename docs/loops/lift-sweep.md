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

## The user's calls (2026-11-15)

- The end state is the goal: native C++ calling native C++, no `ctx`,
  no adapters, SDK imports kept.
- Lifted-looking code is fine for every size of function.
- The sweep runs on its own branch, `lift-sweep`, from
  `rexglue-removal-project`.
- The Xbox Live and two-player modes' code is skipped (Scope).
