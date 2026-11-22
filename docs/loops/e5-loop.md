# E5 loop: describe an area's types, switch its code to them

Instructions for running E5's steps 2 to 5 (`docs/e5-native.md`) area by
area, as B7's loop did (`docs/loops/b7-loop.md`; its rules carried over by
the user's call, 2026-11-16: precedent first, the user's limits per run).
This file covers step 2 (the schema and named fields), step 3 (private
objects to the host) and, drafted, step 4 (shared objects); step 5 gets
its section when it starts.

The session running the loop (the **lead**) does no area's work itself: it
hands each area to one subagent at a time, reads its report, checks it
(the validation below), and goes on or halts. What a pass needs lives in
the repo (this file, `docs/e5-native.md`, the schema, the function files,
commits), not in the session's memory.

## Settings

- **Branch:** the branch the run is on (`e5-native-16-11-26` for the
  first runs). The loop makes no branch, never touches `main` or tags,
  never merges.
- **Code changes without asking first:** allowed on the run's branch for
  the areas the loop takes (as B7's loop).
- **Function rows:** agents may correct a row's notes where they learn a
  field (named in the schema); listed in the run's summary for one review.
- **Limit:** 3 areas a run, then the loop stops for the user's review
  (B7's precedent). Each run gets a summary,
  `docs/loops/e5-loop-<date>-<time>.md`.
- **Precedent first:** follow the user's earlier calls for like cases;
  ask only new decisions, and wait instead of halting.

## Step 2, one area (the agent's work)

The area is a file of `docs/functions/` (`race-menu.md` → area
`race-menu`, schema `itc_recomp/schema/race-menu.toml`, header
`src/schema/race_menu.h`). Nothing in memory changes in step 2: the same
bytes at the same addresses, read and written in the same order.

1. **Read** the area's function file, `docs/known-data.md`'s entries for
   its globals and objects, its hand port's headers (they name most
   offsets in comments and `k...` constants), and its rows in
   `itc_recomp/coverage/type_map.tsv` (`area_list` holds it; private types
   are the area's, shared ones belong to the area touching them most).
   Run `python3 -I itc_recomp/tools/type_map.py` first if
   `itc_recomp/out/type_map.json` is missing.
2. **Seed** the schema: `python3 -I itc_recomp/tools/schema_seed.py <area>`
   (never overwrites; extend an existing file by hand).
3. **Name** every field: from the docs and the hand port's constants;
   `unknown_<offset>` only where nothing says what it is. Fix the seed:
   - one type the measure split (the same object allocated in several
     places, or met through several paths) gets one schema type with all
     its names in `measures = [...]`; a type another area already
     describes (`grep -rn "measure" itc_recomp/schema/`) is extended
     there, not copied (the front-end menu and the page object are
     `menu-helpers.toml`'s, `StateMachine` is `game-modes.toml`'s);
   - field types: `u8 u16 u32 u64 s8 s16 s32 f32 f64`, `ptr` with `to`
     (a schema type), `fn`, `bytes` with `count`; `note` for what the
     docs say;
   - a type of yours already in another area's file (`heap.toml` holds
     classes whose deleting destructors the measure filed under the heap
     before 2026-11-17: `VtableObject`'s 22, `Framework`, `Movie`,
     `CommandList`, `Skeleton`, `SaveSlot` and others) is moved into
     yours and completed there;
   - the area's image words in `[globals]` (name, address, type);
   - types with a C++ name of their own (`TuningScreen`, not
     `New9700_TuningScreen_Create`), unique across the schema.
4. **Generate and check:** `python3 -I itc_recomp/tools/schema_gen.py`,
   then `python3 -I itc_recomp/tools/schema_check.py <area>`: no access may
   fit no field. A miss is a missing field, a wrong width, or a measure
   fault (two objects joined): fix the schema, or report the fault
   (`tools/type_map.py` is the lead's to change).
5. **Switch the hand ports** of the area (its `src/<area>/` folder and
   `src/hooks/<area>.cpp`) from offsets to the views: `guest::LoadU32(base,
   x + 24)` → `schema::T(base, x).field()`, `k...` offset constants
   replaced by the views (deleted when unused), globals through
   `<Area>Globals`. The same loads and stores in the same order: a view
   reads at the call, so `p.set_a(p.b())` keeps load-then-store. Values
   keep their types (cast where the field's type differs, or choose the
   field's type to match its use). Frames and stack words stay as they are
   (they aren't objects: step 6).
6. **Lifted code:** `python3 -I itc_recomp/tools/lift.py reemit` and
   `python3 -I itc_recomp/tools/lift_originals.py --all` name the fields the
   measure types (the lift's own check: no other diff than `schema::`
   names and includes).
7. **Build** (`cmake --build itc_recomp/out/build/linux-amd64-release
   -j$(nproc) -- -k 0`, in the background: a change to the lifted files
   takes 10+ minutes) and **check**: `timeout 120
   itc_recomp/tools/attract_race.sh 40` (no error lines) and `timeout 240
   itc_recomp/tools/scripted_run.sh
   itc_recomp/pad_recordings/race_test.jsonl` (~127 s, no error lines, the
   race at the frame cap); `pkill -9 -x itc` after each.
8. **Commit** by explicit paths (never `git add -A`; the user may be
   editing), message ending with the Co-Authored-By line, and push.
9. **Report:** the types and fields named (counts), what stayed
   `unknown_`, the check's result, the hand-port accesses switched and
   left (with why), the lifted accesses named (`grep -c "schema::"
   itc_recomp/src/lifted/*.cpp`), the runs' log numbers, any row changed.

## The lead's validation after each area

- `schema_check.py` (all areas) passes; `git diff` of the area's hand
  ports shows only offset-to-view changes (spot-read).
- The build, an attract race and the pad replay, run again by the lead.
- Halt (and wait for the user) on: a check that can't pass without a
  measure change, a behavior difference, a crash, or a decision the
  user's earlier calls don't cover.

## Gotchas

- `pkill -f` can match your own shell; use `pkill -x itc`.
- A guest fault or a call to an address with no entry stops the program:
  the log names the address (`lifted: a call to ...`): lift its original
  (`lift_originals.py` with it), as step 0's correction says.
- `base` the parameter shadows the `base` namespace: `::base::` inside.
- Never edit `src/lifted/` or `src/schema/` by hand: change the tool or
  the schema and generate again.

## Step 2 as a swarm (the user's call, 2026-11-18)

The user asked for as many parallel agents as possible, the conversion of
every area in one go, the lead validating once at the end and the issues
fixed after; then for at most 6 agents at a time. Run as a Workflow (the
script: `itc_recomp/tools/workflows/e5-step2-swarm.js`; `args`: `{"areas":
[...]}`), in three phases:

1. **Convert**, one agent per area (6 at a time): its schema file, its own
   types named, its hand ports switched to them. Agents only edit (their
   own area's files: `schema/<area>.toml`, its hand-port files, its
   `docs/functions/<area>.md`); they never build, run the game, use git,
   run `lift.py`, `lift_originals.py`, `type_map.py` or the full
   `schema_gen.py` (only `schema_gen.py --area <area>`, which skips other
   files mid-edit).
2. **Integrate**, one agent: the full `schema_gen.py` and
   `schema_check.py` run clean (renames of clashing types, two types
   claiming one measure name merged, missing pointees), the areas' notes
   about schema files acted on.
3. **Cross-area**, one agent per area again: accesses to types other areas
   own switched to their views.

Then the lead (not an agent): `lift.py reemit`, `lift_originals.py --all`,
the full `schema_gen.py`, `schema_check.py`, the build (fixing what doesn't
compile), an attract race, the pad replay, a frame dump, the commit; then
the fixes the checks call for.

### Where it stands (2026-11-21)

- Done before the swarm, one at a time: `type2-dialog` (the pilot),
  `heap`, `c-runtime` (built, checked, committed), `xaudio-objects` (edits
  only, in the tree).
- Convert done for all 130 areas, Integrate and Cross-area (134 areas)
  done: the swarm finished 2026-11-21 (`wf_444dbbee-5ac`, stopped by the
  usage limit twice and resumed; 187 agents). The integration merged the
  types two areas both described, renamed to the owners' names, and gave
  the measure's faults `fault_*` fields with notes.
- The lead's validation (2026-11-21): the full `schema_gen.py` (134 areas,
  1,608 types) and `schema_check.py` (0 accesses that fit no field);
  `lift.py reemit` (43 batches, 3,726 functions) and `lift_originals.py
  --all` (2,369 lifted; `sub_825B0BE4` refused, as before); the build,
  after three fixes (a field named `register`, now refused by
  `schema_gen.py` with every C++ keyword; `car_settings/handlers.cpp` on
  `FrontEndMenu`'s old names; a stale SDL configure check after a glibc
  update, in the build directory only); an attract race (`attract_race.sh
  40`: 38 s at 30 ticks/s, the frame cap, no error lines) and a frame dump
  (the race drawn right). The pad replay ran after a reboot (2026-11-22:
  `/dev/uinput` couldn't open until then, the kernel upgraded without
  one): 124 s, no error lines.
- The agents' reports (130 convert, the integration, 134 cross-area):
  `docs/loops/e5-swarm-reports.md`, copied from the journal.
- `lifted.md` and `functions.md` (rows not yet in an area) weren't in the
  swarm's list.

### To resume

From another session (the user has to ask for a workflow in their own
words; resume only when the user's current message asks for it): run
`itc_recomp/tools/workflows/e5-step2-swarm.js` as a Workflow (`scriptPath`)
with
- `args.areas`: the areas left to convert (above);
- `args.cross_areas`: every area with a schema file
  (`ls itc_recomp/schema/*.toml`, the file names without `.toml`), so
  the Cross-area phase covers the areas converted earlier too (their first
  pass's notes come from `docs/loops/e5-swarm-reports.md`).
It runs 6 agents at a time (the user's call). If a usage limit stops it,
commit the tree as a WIP snapshot, update "Where it stands" here and the
reports file, and wait for the user before resuming (Workflow with the
same `scriptPath`, `resumeFromRunId` and args replays the finished agents).
When it finishes: the lead's validation (above), then this file and
`docs/e5-native.md` updated.

### For the lead after the swarm

Done (2026-11-22, `d557e44c`):

- The pad replay (after a reboot): 127 s, no error lines, the race at the
  frame cap.
- **The measure's offset fault** (`type_map.py`): a pointer into an object
  (`this + 96`) voted for the callee's parameter as the whole object. Now
  a pointer at a nonzero offset votes for the object embedded there
  (`T@+96`; a negative offset, `T@-12`, the object containing it: array
  headers and secondary bases), one into an array inside it (an index
  added) for nothing, and the same for a pointer stored in a field. The
  joins of two named classes the measure refuses fell from 463 to 142,
  its types from 4,884 to 5,133. The schema followed (a one-off script,
  not kept): measure names renamed where the same object got a new name
  (203 added, 86 gone), 117 split-off embedded objects seeded as their own
  types (`<Owner>At<N>`, `<Owner>Outer<N>`, `unknown_` fields, in the
  owner's file, each marked "Split from"), three of them given to the
  types they are (`HsvColor`, `RoutePosition`), the XACT engine's
  interface no longer duplicated in `menu-sounds.toml`, XAudio's reverb
  stages placed (`ReverbLowPass`, `ReverbDelayLine`, `ReverbAllPass`,
  `ReverbLineFill`, `ReverbEarlyStage`, `ReverbTwoTapLine`), and the 49
  `fault_*` fields no access reaches any more removed. 1,726 types, 0
  accesses that fit no field, 135 never seen (137 before). Built, an
  attract race, the pad replay and a frame dump checked.
- `docs/known-data.md`'s converter state: 88 bytes.
- **The class-name fault** (the same family; 2026-11-22, `81b2de4b`): a
  function whose row names it `X::Y` had its r3 joined with `X` even when
  every caller passes an object embedded in another
  (`CarScene::InitLighting`'s r3 is the scene + 5248). Now the callers
  win: no join by name when every r3 vote is embedded. Refused joins 142
  -> 103. The schema followed (the same script, improved): 212 more
  embedded types seeded, their fields named from the owners' `fault_`
  fields where those covered them (`CarScene`'s `fault_lighting_*` are the
  lighting block's `lighting_*`), one field per width read, a handful of
  names two types claimed given to one by their accesses, and 63 more
  unreached `fault_*` fields removed. 1,938 types, 0 accesses that fit no
  field, 136 never seen. Built, an attract race, the pad replay and a
  frame dump checked.

Left:

- 236 `fault_*` fields still reached (other measure faults; the areas' reports
  say which). `RaceData.fault_car_count` is no longer reached but a hand
  port uses it (`track_collision/track_world.cpp`).
- The 329 seeded embedded types (each marked "Split from") are `unknown_`
  until read, but where an owner's `fault_` field covered them.
- The areas' reports list the accesses left as offsets (loops over tables,
  stack words, register arithmetic) and their notes for the lead.
- `docs/functions/replay-snapshots.md`'s schema paragraph is stale
  (`ReplayPlacedBox`, merged into `BoxShape`; the record copies' r4, split
  by the offset fix).

## Step 3: private objects to the host (draft, 2026-11-22)

`docs/e5-native.md` step 3 is the why. A type moves when only one area's
code touches it: it becomes a plain C++ struct in host memory, and its
schema view (`schema::T`) reads that struct instead of guest memory, so
code already switched to the views doesn't change. The user agreed the
outline (2026-11-22); the details below are a draft for review.

### Groundwork, before the first area (done directly, not by agents)

1. **The state digest**: done (2026-11-22). `--state_digest` logs each
   race tick's racers (their body matrices and speeds); the pad is replayed
   by tick (`--pad_ticks_play`), lined up at each race's start, so two
   runs match tick for tick (`docs/pad-replay.md` "By tick").
   `tools/tick_replay.sh` plays `pad_recordings/race_test.ticks` and
   compares with `race_test.digest`, the baseline from the build before
   step 3. Menus aren't in the digest yet (only races).
Done (2026-11-22): all five, each built and checked (an attract race and
`tick_replay.sh` unchanged). 1 is above; 2 `f70cd5e8`; 3 `2a995859`; 4
`024face6` (`src/memory/handles.h`: 0x60000000 to 0x7EFFFFFF, nothing
touched it); 5 `tools/e5_movable.py` (`coverage/e5_movable.tsv`): 68
types movable now in 31 areas; of the 1,806 the code touches, 1,574 have no
allocation the measure saw (embedded or static: step 4), 338 are shared,
314 read from files, 313 have overlapping fields, 131 feed the GPU, 43
are saved. The first area is a pilot done directly (as step 2's was):
`type2-dialog`'s `Type2DialogData` (68 bytes, one allocation, step 2's
pilot area).

2. **Host storage in the schema**: a type marked `storage = "host"` gets
   a generated C++ struct (host byte order, its fields as members, host
   pointers for `ptr` fields to other host types), and its view holds a
   pointer to it. A host view has **no `_address()` accessors**: every
   use of a moved type's guest address (lifted code's
   `guest::LoadU32(base, view.field_address())`, a hand port's offset
   arithmetic, a memcpy over the object) stops compiling, so the build
   lists what's left to convert.
3. **`lift.py` for host types**: lifted accesses to a host type's field
   become value accessors (`view.field()`, `view.set_field(v)`), with the
   same width and conversion.
4. **Handles**: a 32-bit value standing for a host object where code
   still in guest shape holds it (in a guest object's field, a register
   passed to unconverted code). Handles come from a guest range made
   inaccessible (`PROT_NONE`: today all 4 GB is mapped read and write, so
   a range nothing allocates in is carved out first, probably in the
   0x40000000 view), with a fault handler that logs the address and the
   code. A raw load or store the tools couldn't type, reaching a moved
   object, then stops the game at that access, loudly, instead of reading
   stale guest bytes. A table maps handle to host pointer; views take
   either.
5. **The pick list** (`tools/e5_movable.py`, new): from the measure, the
   types one area touches, not read from a file in place, not read by the
   GPU, not saved; with their allocation and free sites. Ordered by the
   areas with the fewest ties first (`type_map_areas.tsv`). A type whose
   accesses include ones the measure couldn't type is flagged: those
   functions are read first.

Each groundwork piece: built, an attract race and the pad replay (no
object moved yet, so the game is unchanged), committed.

### The pilot (2026-11-22)

`type2-dialog`'s `Type2DialogData` (68 bytes, `[0x826D8400]`): marked
`storage = "host"`; `Type2Dialog::OnEnter` makes it with
`Type2DialogData::New` in place of the `VirtualAlloc(68)` at `0x82186A7C`
(returning the handle as the original returned the address), `OnLeft`
frees it with `Delete()` in place of the `VirtualFree` at `0x82186478`
(returning 1, as that did). Nothing else changed: every user already went
through the view, no lifted code reaches the block (the re-emit left the
batches as they were), and the build had nothing to report. Checked: an
attract race; `tick_replay.sh` (both races match the baseline, the replay
entering and leaving the dialog: `OnEnter` and `OnLeft` ran); no handle
fault. The functions keep their status: the dialog's pages and menu are
still guest objects, so none has all its objects in host memory (**Host**).

### One area (the agent's work)

1. Take the area's movable types from the pick list; read their rows and
   schema entries.
2. For each type: `storage = "host"` in its schema file; its allocation
   sites make a host object (`new`, or a member of another host object)
   and hand out a handle where guest-shaped code keeps it; its frees
   delete it and release the handle.
3. The full `schema_gen.py`, `lift.py reemit`, `lift_originals.py --all`;
   the build. Every compile error is a place still using the guest
   address: converted to the view (or, where a callee in another area
   needs an address, passed the handle).
4. Functions whose objects are all host structs: status **Host** in the
   area's `docs/functions/` file; `check_function_statuses.py`.
5. Report: types moved, handles handed out (and where), anything left in
   guest shape and why.

### Check after each area (the lead)

As now, plus the digest: the build; `timeout 120 attract_race.sh 40`; the
pad replay (no error lines, the race at the frame cap); `tick_replay.sh`
(**a race tick that differs from the baseline is a halt**); a frame
dump. A handle fault (a raw access to a moved object) is a
halt with its address. Then commit and push. The user drives before each
merge into `rexglue-removal-project`.

### Open points for the first run

- Which range the handles take (checked against our allocation layer and
  the physical views).
- Objects kept in arrays inside guest objects, or copied whole by memcpy:
  moved with their container, or later (step 4).
- Vtables: a moved object's vtable word stays the guest address of the
  image's table until step 4 (virtual calls through the dispatch table as
  now).

## Step 3 as a swarm (the user's call, 2026-11-22)

The rest of step 3's movable types, moved by agents in **batches** (the
user's call: a mistake changes how the game plays, so each batch is
checked before the next, and a difference is narrowed to one batch). The
script: `itc_recomp/tools/workflows/e5-step3-swarm.js`, one batch a run,
`args`: `{"areas": [...], "lifted_sites": false}`; one agent per area, 6
at a time, edits only (no builds, no git, own files only, rows proposed in
the report, not written). Each agent lists its area's movable types
(`tools/e5_movable.py <area>`), finds every allocation, free and user,
moves what it can (`storage = "host"`, `New` and `Delete` at the sites, the
users through the view) and skips (with the reason) what it can't: memory
used as memory, an array allocated at once, a lifted function that may
reach it raw, overlapping fields, and (until `lifted_sites`) a type
allocated or freed in lifted code.

### The batches

From `coverage/e5_movable.tsv` (2026-11-22): 68 movable types; the pilot's
done, the movie library's 9 left out (a replaced library: it never runs),
58 left in 29 areas, in order of fewest ties to other areas:

1. **Batch 1** (every type allocated in hand ports; `lifted_sites: false`):
   boot-mode, track-zones, game-modes, type5-mode, type10-mode,
   title-screen, location-screen, garage-screen, car-sounds, network-mode,
   type6-type7-modes.
2. **Before batch 2: `lift.py`'s `host_alloc` and `host_free`** (below), done
   by the lead directly, built and checked with one type.
3. **Batch 2** (`lifted_sites: true`): heap, framework, app-states,
   car-list, race-overlays, material-passes, stand-models, race-screens,
   race-screens-unseen.
4. **Batch 3** (`lifted_sites: true`): todays-result, system-services,
   car-dealer, cameras, collision-world, car-scene-draw, type2-screens,
   car-physics, camera-modes.

Run `tools/e5_movable.py` again before each batch: a batch's moves don't
change the others' lists, but the table is the record.

### lift.py's host_alloc and host_free (to write before batch 2)

40 of the 58 types are allocated (or freed) in lifted code, which agents
can't edit. Lifted code calls an allocator as
`{ const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x820F77DC, r31)); r3 = o.r3; f1 = o.f1; }`,
the first argument being the call's return address (the instruction after
the `bl`; the same address the hand ports' `calls.Call(..., 0x82186A7C, ...)`
name). The spec:

- A host type's schema entry may list its lifted allocations,
  `host_alloc = [0x820F77DC]` (the calls' return addresses), and its lifted
  frees, `host_free = [{ call = 0x8215E2C4, reg = "r3", returns = 1 }]` (the
  register holding the object's pointer: r3 for VirtualFree, operator
  delete and free, r5 for HeapFree; what the call returned: 1 for
  VirtualFree and HeapFree; left out for operator delete and free, which
  leave r3 as it was, so the replacement doesn't set it).
- `lift.py` (where it emits a call statement) replaces a listed allocation
  with `{ r3 = schema::<Type>::New(base).address; f1 = 0; }` and a listed
  free with `{ schema::<Type>(base, uint32_t(<reg>)).Delete(); r3 = <returns>; }`,
  adding the type's header; `lift_originals.py` the same in `originals.cpp`.
  An address listed that no emitted call has is an error (the tool stops
  with it), so a wrong address can't pass silently. Each replacement is marked with
  its address (`// host_alloc 0x820A7130 (FloatPair)`), which is how each
  tool finds the sites the other emitted. A free of 0 does nothing
  (`Delete()` on handle 0), as operator delete's and free's.
- `schema_gen.py` accepts the two keys (on host types only) and checks
  their form.
- Checked on one type first (built, an attract race, `tick_replay.sh`),
  committed, then batch 2 with `lifted_sites: true` (the script's prompt
  then tells agents to list sites instead of skipping).

### The lead's check after each batch

1. Commit the tree as a WIP snapshot (explicit paths, no `/home/` paths,
   push), and add the batch's report to `docs/loops/e5-step3-reports.md`
   (from the Workflow's `journal.jsonl`; what moved, what was skipped and
   why, the lifted concerns, proposed rows, notes).
2. `python3 -I itc_recomp/tools/schema_gen.py`,
   `python3 -I itc_recomp/tools/schema_check.py` (0 accesses that fit no
   field), `python3 -I itc_recomp/tools/lift.py reemit` (its list of host
   types' accesses left raw: each is a place that faults if reached; read
   them), `python3 -I itc_recomp/tools/lift_originals.py --all`.
3. The build, in the background:
   `cmake --build itc_recomp/out/build/linux-amd64-release -j$(nproc) -- -k 0`;
   every error is a use of a moved type's guest address: fix it (through
   the view), or put the type back (`storage` removed and its sites as they
   were) when it can't be.
4. `timeout 120 itc_recomp/tools/attract_race.sh 40` (no error lines; `pkill
   -9 -x itc` after), `timeout 400 itc_recomp/tools/tick_replay.sh` (both
   races "all match"), a frame dump (`attract_race.sh 25
   --native_renderer_dump=<dir in the scratchpad>`, look at `shown.pfm`).
   A **handle fault** in a log names the type and the offset: find the raw
   access (the stack in the log, or run the game under gdb, which may only
   attach to a process it starts: `gdb -batch -ex run -ex "thread apply all
   bt" --args ...`), fix it or put the type back. A **digest that differs**:
   find the batch's area by putting areas back half at a time (commit
   first; then restore an area's files from the commit before the batch in
   a new commit, rebuild, `tick_replay.sh` again).
5. Commit and push; update "Where step 3 stands" below.
6. Show the user the proposed row changes (from the reports) for review;
   write them only after the user agrees, then
   `itc_recomp/tools/check_function_statuses.py`.

### If a run stops (a usage limit, an error)

Commit the tree as a WIP snapshot (explicit paths, push), update "Where
step 3 stands" and the reports file, tell the user where it stopped, and
wait: resume (Workflow with the same `scriptPath`, `resumeFromRunId` and
args, in the same session) only when the user's current message says to.

### Where step 3 stands (2026-11-22)

- Groundwork done; the pilot (`Type2DialogData`) done, `98123e00`.
- Batch 1 done and checked (2026-11-22): 6 types moved, 12 skipped
  (`docs/loops/e5-step3-reports.md`); both replay races match. Only
  `TitleLoadBlock` is on the replay's path; the other five are checked by
  the build only. Its proposed rows await the user's review.
- `lift.py`'s `host_alloc` and `host_free`: done and checked (2026-11-22)
  on `FloatPair` (heap; made by `sub_820A70F8`, freed by `FloatPairItem`'s
  destructor; not seen in play, so the emission and the build are what's
  checked). `host_free`'s `returns` may be left out (operator delete and
  free leave r3). Its first `tick_replay.sh` showed race 1 a tick late from
  tick 12: not the move (it never ran) but the replay's lineup, which
  didn't cover the race's own loads; fixed with a start mark
  (`docs/pad-replay.md`, "By tick"), the recording and baseline made again,
  then both races matched twice.
- Batch 2 done and checked (2026-11-22): 2 types moved (`PauseMenuData`,
  `PauseMenu4Data`, both seen in the replay), 18 skipped; both replay
  races match. Its proposed rows await the user's review.
- Batch 3 done and checked (2026-11-22): 2 types moved (`CameraAnimLoad`,
  `NetScreen7Data`; not on the replay's path), 18 skipped; both replay
  races match. Its proposed rows await the user's review.
- Step 3's swarm, in all: 11 types moved of the 58 (with `FloatPair`, the
  lead's check), 47 skipped, mostly for vtables read raw, objects or
  interfaces embedded at offsets, memcpy targets, and addresses handed to
  other areas (the reports). What's left of step 3 is listed in the
  reports file's batches: types movable after a small change (a field's
  address staged in a guest buffer: `GarageScreenData`,
  `TodaysResultData`; a measure fault split: `PauseMenuScreen`,
  `CameraAnimBlock`; a shared record type: `TodaysEventImage` with
  `Type2HubImage`), and the rest waiting on step 4 (virtual dispatch over
  handles, embedded objects).
- The leftovers (2026-11-22, the lead, the user's call): `GarageScreenData`
  and `TodaysResultData` (their pick passed to the movie by value, no guest
  buffer), `PauseMenuScreen` and `CameraAnimBlock` (their measure faults
  split or folded in) and `Type2HubImage` with `TodaysEventImage` merged in
  moved; `ModeType8` and `SysLinkIdSetNode` left out of the pick list
  (unreachable). Checked: build, attract race, `tick_replay.sh` twice (both
  races match), frame dump. Step 3 in all: 16 types moved (17 schema names
  before the merge); the other 39 skipped wait on step 4 (virtual dispatch
  over handles, embedded objects, memcpy targets, addresses handed to other
  areas). The rows for the leftovers await the user's review.
- Merged into `rexglue-removal-project` up to the pilot (`fc5e3b94`), then
  with the batches, the leftovers and their rows (2026-11-22).

## Step 4: shared objects (2026-11-22)

`docs/e5-native.md` step 4 is the why. The user's call (2026-11-22): the
goal is guest memory out of the picture; the lead takes its own
recommendations below (the open points at the end, now decided) and goes
ahead.

### The ground (measured 2026-11-22, after step 3)

`tools/e5_movable.py` (`coverage/e5_movable.tsv`): 1,805 schema types the
code touches, 17 in host memory (step 3's 16 moves; `Type2HubImage` counts
as shared since `TodaysEventImage` merged into it). "Movable" lists 64: the
16 host types, the movie library's 9 (a replaced library) and step 3's 39
skipped. The rest, by what stops them (a type can have several):

| Blocker | Types |
| --- | --- |
| No allocation seen (embedded in another object, static, or a parameter the measure couldn't join to its allocation) | 1,574 |
| Shared (touched by several areas) | 339 |
| Read from a file in place (with GPU 11, with save 3) | 328 |
| Overlapping fields (a union, or a measure fault) | 313 |
| Read by the GPU | 131 |
| Saved | 43 |

What matters is the combinations:

- **910 private types blocked only by "no allocation seen"** (2,406
  function touches): 741 embedded, 92 in the image, 56 allocated through a
  path the measure doesn't follow, 21 other. By their measure names: 344
  parameters (many are a caller's locals passed by address: step 6), 294
  embedded at an offset (`T@+N`), 151 a field's pointee, 94 classes (the
  menus' 9,620-byte machines carved out of a mode's one block:
  `RaceHudMachine`, `CarDealer`, `GarageScreen`, `PlateEditor`,
  `RaceMenu`; `TrackZone` inside `TrackZones`; `AiPath`), 17 returns.
  257 have their owner noted ("Split from"); 31 have a vtable.
- **22 types blocked only by "shared"** (allocated, no tags, no overlap):
  18 of them shared by two areas (`PtrListNode`, `CollisionHit`,
  `SceneCarModel`, `InputClientList`, `AfterRaceMenuData`, `Framework`,
  `RaceDialogData`, `TabPanel`, `BuildScreenDataPart`, `CarSettingsData`,
  `Movie`, `MovieWrapper`, `ColorPanel`, `RowPanel`, `CollisionWorld`,
  `RaceScreens`, `CarPanel`, `Type2HubImage`, already host), 4 by three
  (`XAudioAllocBlock`, `CString`, `NetRankingData`, `CarDealerData`).
- Shared types by their number of areas: 180 two, 73 three, 29 four, 57
  five or more (the hubs: `UiMovieTableValue` 40 areas, `RaceData` 38,
  `SaveData` 36, `UiMovieNode` 19, `Vehicle` 15; most of them also tagged
  file, save or GPU, or embedded).
- **Step 3's 39 skipped**, by what stops them:
  - virtual dispatch over a raw vtable word (15): `GameModeType6`,
    `GameModeType7`, `ModeHolder`, `FloatPairItem`, `FloatTripleItem`,
    `FloatTriple`, `FrameStart`, `MorphJobMaker`, `AppDelegate`,
    `AppStates`, `OverlayView`, `SysUsers`, `AppCameraList`,
    `AppCameraEntry`, `D3DXBuffer`;
  - objects or interfaces embedded at offsets, their addresses handed out
    (10): `TrackZones`, `CarSoundArray`, `Type4BlockPart`,
    `LocationScreenPart`, `StateLayer`, `WorkerPool`, `Kind6MenuPart`,
    `AppCamera`, `Type2Hub`, `CareerResultsScreen`;
  - an address handed to a callee that reads guest memory (5):
    `LocationScreenPictureTexture`, `CarDealerSwatch`, `Type2HubTexture`,
    `CarOverlays`, `CameraCubeFaceSurface`;
  - raw containers or arrays (5): `HandlerMapNode`, `ByteCountedList`,
    `TrackCollisionObject`, `Vector16Storage`, `CollisionObjectBlock`;
  - memcpy targets (3): `CarListEntry`, `CarDealerEntry`,
    `StandModelsSpecCopy`;
  - file-backed (1): `CarSoundWaveBank` (step 5).
- **Virtual calls** in the lifted code (batches and originals): 2,515 calls
  through `ctr`; 2,365 are the vtable idiom (`lwz a,0(obj)`, `lwz c,N(a)`,
  `mtctr c`, `bctrl`), 484 of them with the vtable word read through a
  view, 1,881 raw; 111 call a slot read from something else (callbacks,
  functors), 39 a computed target. The hand ports: about 100 helpers like
  `VirtualTarget` (`Load(Load(object) + offset)`), 226 uses of a `vtable`
  accessor.

What it means: being shared isn't what stops most types. Since step 2
every area reads a type through the same view, so a shared type whose
accesses all go through views moves like a private one, provided its
allocation sites are known and every area's raw accesses are gone. What
stops them is shape: objects living inside other objects (no allocation of
their own), virtual calls that read the vtable word raw, interior
pointers, memcpy, and addresses handed to callees that read guest memory.
Step 4 is mostly groundwork for those shapes, then moving **units** (an
allocated object with every object embedded in it) wherever their areas
are.

### Groundwork, before the pilot (done directly, not by agents)

1. **Embedded objects in the schema.** A field type `embed`
   (`{ name = "race_menu", offset = 9640, type = "embed", to = "RaceMenu" }`,
   `count` for arrays): a guest view returns `T(base, address + offset)`; in
   a host type, `T`'s struct becomes a member. Seeded from the measure's
   `T@+N` votes and the "Split from" notes (`schema_seed.py` or a one-off
   script), checked by `schema_check.py` (an embedded type's accesses must
   fit its owner's span).
2. **Host layout at the guest offsets.** Host structs keep each field at
   its guest offset (`schema_gen.py` fills the gaps with padding and
   `static_assert`s `offsetof`), in host byte order. Then an interior
   pointer is `object + offset`, an embedded member sits where the guest
   code expects it, and a memcpy between two host objects of one type is a
   memcpy. (Step 3's structs are packed; regenerated this way, nothing that
   reads them changes.) Repacked when the last guest-shaped reader goes
   (step 9).
3. **Interior handles.** `memory::HandleObject` accepts any value inside a
   live object's span (`handle + offset`, already reserved by `MakeHandle`)
   and returns `object + offset`; the views of embedded types inside a host
   owner use it. The fault handler is unchanged for raw accesses.
4. **Dual views** (`storage = "dual"`): for a type with instances in both
   shapes while its owners convert (a generic container such as `PtrList`
   embedded in host and guest objects; a record embedded in several
   owners), the view checks `memory::IsHandle(address)` at each access and
   reads the host struct or guest memory. No `_address()` either, so the
   build still lists what's left. Used only where a type really has both;
   a shared allocated type moves whole (all its allocation sites at once)
   and needs no dual view.
5. **Virtual calls over handles.** `lifted::VtableSlot(base, object,
   offset)`: the vtable word from guest memory, or from a handle's host
   object (its `vtable` member at +0, still the image table's guest address,
   written by the constructors and destructors through the view as now).
   `lift.py` emits it for the 2,365 vtable-idiom sites (`lwz a,0(obj)`
   feeding `lwz c,N(a)` feeding `mtctr`), typed or not, and the hand ports'
   `VirtualTarget`-style helpers call it. The call itself still goes
   through the dispatch table by address. C++ virtual functions (the plan's
   end state) come per class hierarchy once its slots' functions are all
   typed and its objects all host: a follow-up after the moves, not
   groundwork.
6. **memcpy over host objects.** The game's memcpy and memmove (native
   entries, so one place) accept handles on either side: host to host, a
   memcpy of the host bytes (the layout is the guest's); host to guest or
   guest to host, a per-type copy generated by `schema_gen.py`
   (`CopyFromGuest`, `CopyToGuest`: each field swapped) when the size is
   the type's or a field run's, else a logged stop. The same for memset.
7. **Callbacks and contexts.** Callbacks stay guest function addresses
   dispatched through the table (that works whatever shape the object is);
   their context word may be a handle (`CameraAnimLoad` did it). Nothing to
   build: the rule for agents. Function objects come with step 6.
8. **Addresses handed to callees.** No tool: per site, by the precedents.
   A value the callee only reads goes by value (`ui_movies::NameRecord`);
   a callee that writes through it, or keeps it, takes the view (dual if
   it gets both shapes).
9. **The step 4 pick list** (`tools/e5_movable.py --units`, extended):
   units (an allocated type with its `embed` closure), each with its
   allocation and free sites, its areas and hand-port files, its raw
   vtable reads, memcpy sites and interior addresses taken, and what keeps
   it from moving in step 4's terms (a file, GPU or save tag: steps 5 and
   7; an image static: step 8; a caller's local: step 6). Ordered leaves
   first: fewest areas, then fewest functions.

Each piece built, an attract race and `tick_replay.sh` (nothing moved yet:
the game unchanged), committed. Pieces 1 to 4 change only generated code
and step 3's structs' layout; 5 and 6 change every virtual call and copy,
so they're checked like a batch.

### The pilot

Recommended: `app-states`' **`AppStates` with `StateLayer`** (and
`AppDelegate`, made by `Delegate::Clone`). `AppStates` (12 B, vtable
`0x820028F8`) is the app's state stack; `StateLayer` (48 B, vtable
`0x820028E0`) holds a state interface at +4 (the layer + 4 is handed out
as the state), and two `PtrList`s at +12 and +24 walked by the list
helpers. Between them they need pieces 1 to 5 (embedded members, interior
handles, the `PtrList` dual view, virtual calls through raw vtable words,
`VirtualTarget`), and they run every tick, so the attract race and
`tick_replay.sh` check them in full; a mistake stops the boot at once. The
fallback if that's too much for one pilot: `race-overlays`' `CarOverlays`
with `OverlayView` (vtables, an embedded part, the archive's callback
context; run in every race).

### Units and order

From the measure (leaves first), in waves, each a few batches:

1. **The pilot** (above).
2. **Shared allocated types** (the 22 blocked only by "shared", less
   `Type2HubImage`, already host, and `Movie` and `MovieWrapper` if the
   movie wrappers no longer run): no new shape, only their areas' raw
   accesses to clear.
3. **Step 3's 39 skipped**, by groundwork piece: virtual dispatch (15),
   embedded (10), handed addresses (5), containers (5), memcpy (3); less
   `CarSoundWaveBank` (step 5) and `StandModelsSpecCopy` and
   `Kind6MenuPart` (the Xbox Live mode: unreachable, to add to
   `UNREACHABLE`).
4. **Owners with their embedded objects**: the mode blocks
   (`Type2ModeBlock`, `ModeType9Block`, ...) with the menus' machines and
   screens inside them, `TrackZones` with `TrackZone`, `AiDriver` with
   `AiPath`; the 910 private embedded types go with their owners here.
   Areas by fewest ties first (`type_map_areas.tsv`'s shared weight:
   `std-string` 0, `menu-helpers` 1, `morph-jobs` 1, `framework` 17,
   `heap` 17, `screen-constructors` 19, `race-hud` 42, `c-runtime-core` 55,
   `app-states` 56, `c-runtime` 66).
5. **The hubs** last, as far as they go before steps 5 and 7: `Racers`,
   `Vehicle`, the scene and its models, the car scene; the UI movies'
   records and `RaceData` are file-tagged, `SaveData` saved: they move
   with their loaders (step 5).

Out of step 4: types tagged file, GPU or save (steps 5 and 7, even when
otherwise movable), statics in the image (step 8), a caller's locals
passed by address (step 6).

### As a swarm

Like step 3's (batches, one checked before the next; 6 agents at a time;
edits only), with one change: **one agent per unit, not per area**, since a
unit spans areas. The script (a new `e5-step4-swarm.js`) takes a batch of
units whose hand-port files don't overlap (checked by the script from the
pick list, so two agents never edit one file).

An agent may:
- set `storage = "host"` (or `"dual"`) on its unit's types, add `embed`
  fields to them, list `host_alloc` and `host_free`;
- edit every hand-port file touching its unit's types (any area), and the
  unit's schema entries;
- turn a raw access, a vtable read, a memcpy or a handed address into the
  view, `VtableSlot`, the handle-aware copy, a value or a view parameter,
  as the precedents above do.

An agent may not:
- edit `src/lifted/`, `src/schema/` or the tools, build, run the game, use
  git, or run `lift.py`, `lift_originals.py`, `type_map.py` or the full
  `schema_gen.py` (`schema_gen.py --area` only);
- touch types outside its unit, or another unit's files;
- write rows (proposed in its report);
- change what the game does (the behavior rule: the same game tick for
  tick);
- move a type it can't clear of raw accesses: it skips it, with the reason
  and the sites.

### The lead's check after each batch

Step 3's ("The lead's check after each batch"), plus:
- `lift.py reemit`'s list of host types' accesses left raw must be empty
  (each would fault);
- a count of the vtable-idiom sites left raw (`lift.py` reports it);
- `tick_replay.sh` twice, both races "all match"; a digest that differs is
  narrowed to a unit by putting units back half at a time;
- grep the replay's log for "native running" and say which moves only the
  build has checked;
- functions whose objects are now all host get the status **Host** (rows
  proposed to the user first), then `check_function_statuses.py`;
- `tools/e5_status.py` (the plan's "Tracking"; written in the groundwork
  if the user wants it now): the counts after each wave, in
  `docs/e5-native.md`.

### Decided (2026-11-22)

The lead's recommendations, by the user's call ("I don't care about any of
these details, what I want is guest memory out of the picture"): host
structs at the guest offsets (repacked at step 9); virtual calls through
the bridge first (`VtableWord`), C++ virtual functions per hierarchy after
the moves; agents per unit; the pilot `AppStates` with `StateLayer`; types
tagged file, GPU or save left for steps 5 and 7; memcpy between host and
guest objects swapping each field (`memory/host_copy.h`);
`StandModelsSpecCopy` and `Kind6MenuPart` unreachable; `tools/e5_status.py`
written now. Overlapping fields no longer stop a move: a field over
another is read at its offset (a measure's `fault_` field never a member).

### Groundwork: done (2026-11-22, `895f9731`)

- `schema_gen.py`: host structs at the guest offsets (packed, padded,
  `static_assert`ed); `embed` fields (a view at the owner's address +
  offset, from either shape); `storage = "dual"` (each access tells a
  handle from a guest address); overlapping fields read at their offset;
  each host type's layout of scalar fields (`k<Type>Layout`) handed to
  `MakeHandle`.
- `memory::HandleObject` takes any value inside a live object's span;
  `DescribeHandle`.
- `lift.py`: the vtable idiom's first load through `lifted::VtableWord`
  (1,151 sites in the batches, 1,015 in the originals); `guest::VtableWord`
  in `guest.h` for the hand ports (the generic `VirtualTarget` helpers in
  six files call it).
- `memory/host_copy.h`: the game's memcpy (both), memmove, memset, `XMemCpy`
  and `XMemSet` over handles: host to host a byte copy, host and guest with
  each field of the host type's layout swapped.
- Checked: build, attract race (38 s, no error lines), `tick_replay.sh`
  (both races all match).

### The pilot: done (2026-11-22)

`AppStates` and `StateLayer` (`app-states`) in host memory, with their
lists (`PtrList` at the layer's +12 and +24, `OpList` at +36) as `embed`
fields of dual types: the stack made by `AppStates::Create`, the layers by
`AppStates::AppStates`, `AddLayer` and `ApplyOp` (`New`; `host_alloc` for
the four lifted originals' operator new calls). The hand ports' uses of the
layer's interface and lists switched from `_address()` to `address + 4`
and the embed views. One raw access found by the run: the shared getter
`WordAt8` (`sub_822BE2C8`, the layer's vtable slot 1) read the layer's +8;
its parameter type `WordAt8Object` made dual (a generic accessor's type is
dual by rule: its callers pass both shapes). Checked: build, attract race
(38 s, no error lines, no handle fault), `tick_replay.sh` twice (both races
all match), a frame dump (normal). The replay reaches `AppStates::Create`,
`AddLayer`, `StateLayer::ApplyOp` and `Update` (every tick).

### Batch 1: done (2026-11-22)

Six agents, one per unit (edits only; a brief in the session, the pilot as the
model), then the lead's build, checks and fixes:

- **Moved** (host or dual): the framework (`Framework`, `FrameStart`, both
  pool makers `MorphJobMaker`, `HandlerMapNode`, `AppOpListNode`; the
  framework's interface views dual); the nine game mode objects and
  `ModeHolder` (most dual: the measure joins them with stack answers and
  blocks; every 812-byte dispatcher view dual); the car list's entries and
  colors, the dealer's swatches, the camera presets and animations, the
  four panels (dual: the type 8 mode embeds them); `AfterRaceMenuData`
  and its screen (dual), `RaceDialogData`, `PowerGraphScreenData`,
  `UserInfoData`; `CollisionWorld`; `InputClientList` and the race's car
  inputs (`CarInput`, dual). About 40 types.
- **Skipped**, by the agents (their reasons): the vector library and the
  matrix math read field addresses as guest memory (`RigidBody`,
  `BoxShape`, `CollisionHit`, `CollisionContact`); arrays allocated as one
  block (`CollisionObjectBlock`, `PtrListNode`'s allocator); `InputManager`
  and `ActionMapper` (arrays of embedded objects handed out by address);
  `App` (its +16 interface's vtable read raw in other units' files); lifted
  vtable slots reading their objects raw (`AppCamera` and its list and
  entries, `CarDealerEntry`); kernel objects in place (`WorkerPool`);
  objects handed to the graphics code (`AppScene`, `Type2HubTexture`).
- **The lead's fixes on the way:** the game's frees (`memory::FreeHandle`
  from `Free`, `FreePages`, `FreePhysical`) and `InRange`, `IsPages` and
  `IsHeapBlock` take handles (a menu teardown fell into the kernel's
  `NtFreeVirtualMemory` before); every `guest::Load*` and `Store*` reaches
  a host object through its handle (`memory::RawHandleLoad` and `Store`,
  as the guest's bytes would give it, logged once per type and offset):
  raw accesses no longer stop the game (the career's free run,
  `RaceStates::LinkTick`, copies car inputs raw); overlaid fields (a word
  read as two halves) read the guest-order image of the members they
  cover (`memory::LoadOverlay`), with `load_at` and `store_at` for lifted
  accesses inside a field; scalars over bytes fields as members; embed
  accessors as templates, their areas' headers included at the end.
- **The replay's sensitivity to timing**, found because race 1 differed
  from tick 0: the game's own random generator was advanced by the camera
  shake on render-only passes and on each tick of a race's loading, so the
  traffic's random parts moved with the frame rate and the load times
  (found by logging each draw's caller in both builds). Fixed:
  `--game_rand_by_tick` (on by default) and the replay's line-up at each
  race's start (`docs/pad-replay.md`); the baseline made again from the
  pilot's build with both. Things tried and ruled out on the way: the
  moved types one unit at a time, `rand` (identical), zero-filled heap
  blocks (no effect).
- **Checked:** build; attract race (38 s, no error lines); `tick_replay.sh`
  twice against the new baseline (both races all match); a frame dump
  (normal). Raw accesses logged in the replay (handled, to convert): a
  `CarInput`'s +0 to +16 (lifted copies), a `Framework`'s +72.
- Rows proposed by the agents await the user's review (not written).
- The schema check lists 23 accesses that fit no field: the modes' fault
  fields the modes agent removed (block reads joined to the mode, which
  stay guest reads, and the `ForceExit` stores at -28, never reached).

