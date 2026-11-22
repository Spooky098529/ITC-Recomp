# E5 loop: describe an area's types, switch its code to them

Instructions for running E5's steps 2 to 5 (`docs/e5-native.md`) area by
area, as B7's loop did (`docs/loops/b7-loop.md`; its rules carried over by
the user's call, 2026-11-16: precedent first, the user's limits per run).
This file covers step 2 (the schema and named fields); steps 3 to 5 get
their sections when they start.

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
