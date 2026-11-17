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
