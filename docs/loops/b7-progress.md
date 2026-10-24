# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, `b7-loop-2026-10-23-1722.md`,
`b7-loop-2026-10-23-2022.md`, `b7-loop-2026-10-24-0325.md`,
`b7-loop-2026-10-24-1601.md`, and this file's history. What's left to
see in a drive, from every run: `to-drive.md`.

## Now

- Run started 2026-10-24 17:46 on `b7-loop-24-10-26-3` (the user's
  branch), from 206854d; summary `b7-loop-2026-10-24-1746.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: `leads.md`'s top lead (`sub_8212EC90`,
  `sub_8212ED78`, `sub_8212EA98`, `sub_8212EDE0`: at the end of the race
  overlays' address range but not overlays; all four ready). Mapped as the post effects
  (`post-effects.md`, 57 rows: 3 moved from `functions.md`, 54 new;
  f3643e1; 32 ready groups; 3 leads added); map checked (0 errors, 57 of
  57 in play). Migration 1 done (ece3c32..66b8f81: 43 functions,
  44 of 57 ours, nothing ready left; 9 adapters removed;
  `--native_post_effects`), validated (`itc_1395`, `itc_1396`; all
  ran in play or the replay). Finished.
- Between subsystems (no mapping, not counted): `leads.md`'s three
  cleanup leads, `sub_8259E6C8` UiMovie::ApplyTransform
  (`ui-movies.md`), `sub_8213B2B8` AiDrivers::Destroy and `sub_8213AD98`
  AiDrivers::DestroyRings (`ai-drivers.md`), `sub_820BAAA0`
  CollisionShape::~CollisionShape's adapter (`collision.md`). Migration
  done (1197f81..cdaa66d: the collision shape's adapter removed, the AI
  drivers' teardown, the UI movie walk from ApplyTransform up to
  `UiMovie::Draw`; 6 adapters removed), validated (`itc_1420`, `itc_1421`; all ran in
  play or the replay). Done.
- Subsystem 2 of 3: `leads.md`'s input processor's constructors
  (`sub_8258CE60`, `sub_825920D0`; both ready, 3 functions left; block
  `ActionMapper::ActionMapper`). Mapped as the input manager and
  processors (`input.md`, 29 rows: 13 moved from `functions.md`, 16 new;
  167d407; 17 ready groups; 1 lead added); map checked (0 errors, 29 of
  29 in play). Migration 1 done (57373be..4dd7a11: input.md 29 of
  29 ours, with `ActionMapper::ActionMapper`, `Bind` and
  `InputClientList::BindAll` it unblocked; 16 adapters removed;
  `--native_input`); `ActionMappers::Init` (`sub_8209F430`) ready, left
  for migration 2. Validation 1 (`itc_1448`, `itc_1449`; prints added
  in 0c64e99; all ran in play or the replay) reported two functions
  `missing:`: the loop stopped (below).

## Done

- The input manager and processors (`input.md`, mapped in 167d407; 29
  rows): 29 ours in one migration (57373be..4dd7a11), with three action
  mapper functions it unblocked; 16 adapters removed; validated with
  two missing harness lines (below); `--native_input`. Left:
  `ActionMappers::Init` (`sub_8209F430`, ready).

- Cleanup leads (no mapping): the collision shape's adapter, the AI
  drivers' teardown, the UI movie walk (`collision.md`, `ai-drivers.md`,
  `ui-movies.md`; 1197f81..cdaa66d); validated (`itc_1420`,
  `itc_1421`); existing flags.

- The post effects (`post-effects.md`, mapped in f3643e1; 57 rows): 44
  ours in one migration (ece3c32..66b8f81), 9 adapters removed;
  validated (`itc_1395`, `itc_1396`); `--native_post_effects`. Left:
  `RenderTextures::Acquire` and the recorders on `sub_8235A9F8`, the
  models' shader pairs, the vertex declaration, `sub_8209AE48`.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-24 17:46").

## Expected to differ

This run's groups: `expected-differences.md` (the "17:46 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1714, work left 5046 (wrapped 122, known 587, unknown 4337).
- After mapping the post effects: ours 1714, work left 5046 (wrapped
  122, known 641, unknown 4283).
- After the post effects' migration 1: ours 1757, work left 5003
  (wrapped 122, known 598, unknown 4283). Race 25 s (`itc_1394`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing.

## Stopped

2026-10-24, after the input's validation 1: two functions `missing:`
(`b7-loop.md`'s "When to halt"). `sub_8258CEB0` InputProcessor::
TestPressed and `sub_8258CEE0` InputProcessor::TestRepeat have no plain
C++ of their own: their adapters (`ITC_INPUT_WRAPPER`,
`src/hooks/input_manager.cpp`) read the mask and call AnyPressed's and
PressedOrRepeat's plain C++, and hold their prints (0c64e99); the
`input` harness's native side for them (`NativeAnyPressed`,
`NativePressedOrRepeat` in `src/harnesses/input.cpp`) calls the inner
plain C++ directly, so their prints aren't reached. Both harnesses at
300 cases, 0 differ; both ran in play or the replay. Everything else
passed: harnesses 0 differing (none added to expected differences),
races 24-25 s under `attract_race.sh 40`, no errors, replays ok.
For the user: whether those two become plain C++ functions of their own
(with the harness calling them), get Merged rows, or the harness calls
their adapters; then the input subsystem's migration 2
(`ActionMappers::Init`, ready). Run limit: 2 of 3 subsystems done.
Also: `review.md` ("Run 2026-10-24 17:46"), among it the harness that
depends on the heap's layout (`strings_and_font`).
Resolved (the user's calls, 2026-10-24): TestPressed and TestRepeat
given plain C++ of their own, which their adapters and the `input`
harness call (7021b78; both harness lines seen, `itc_1490`); the
harness that seemed to depend on the heap's layout was the harnesses'
shared random generator, with `swprintf`'s format generator and the
drawing cases' vertex comparison (1ce3182; `review.md`). The run isn't
resumed: `ActionMappers::Init` is the top of `leads.md` for the next
run. Concluded (the user's call, 2026-10-24): merged into
`rexglue-removal-project`; `review.md`'s section for this run reviewed.
