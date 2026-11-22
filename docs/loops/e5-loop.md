# E5 loop: describe an area's types, switch its code to them

Instructions for running E5's steps 2 to 5 (`docs/e5-native.md`) area by
area, as B7's loop did (`docs/loops/b7-loop.md`; its rules carried over by
the user's call, 2026-11-16: precedent first, the user's limits per run).
This file covers step 2 (the schema and named fields) and, drafted,
step 3 (private objects to the host); steps 4 and 5 get their sections
when they start.

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
- Merged into `rexglue-removal-project` up to the pilot (`fc5e3b94`).
