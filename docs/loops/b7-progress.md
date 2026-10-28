# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-27 22:31 on `b7-loop-27-10-26` (the user's
  branch), from d7b8a6b; summary `b7-loop-2026-10-27-2231.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: `leads.md`'s top lead, `sub_820AE010`,
  `sub_820AE348`, `sub_820AE498` (`820AE010`-`820AEC10`, objects made
  with `sub_825A6880` from `sub_82094590`; 4 ready groups under them).
  Mapped as the app's update and its state stack (`app-states.md`, 56
  rows: 2 moved from `functions.md`, 54 new; ac92c3b; 1 lead added).
  Map checked (0 errors, 56 of 56 in play, 26 ready groups, no
  blockers outside it, no kernel calls). Migration 1 done after five
  batches (7bb4dfd..b29697d: the containers, the operation list, the
  layer's push, the message vector, `App::IsRunning`, the walks,
  handlers, requests and add slots, `App::Update`; 5 adapters removed,
  4 Native with none, 1 Merged; `--native_app_states`; `itc_1995`: 23
  s, 0 errors, every harness 0 differing but the expected groups;
  frame dumps the same as the fallback). Left ready: `App::Start`,
  `AppStates::Create` and the state-layer tree's ready groups.
  Validated (`itc_1996`: 30 of 30 harness lines, 24 ran in the attract
  race, 6 in the replay `itc_1997`, ok; nothing to drive). Migration 2
  done (4baecb1..30892a7: the state stack's set-up and `App::Start`,
  the layers' walks, updates, operations and toggle messages; 14
  adapters removed; `itc_2011`: 23 s, 0 errors, every harness 0
  differing but the expected groups; frame dumps the same as the
  fallback). `app-states.md` 56 of 56 ours. Validated (`itc_2012`: 28
  of 28 harness lines, 18 ran in the attract race, 10 in the replay
  `itc_2013`, ok; nothing to drive). Finished.
- Subsystem 2 of 3: the car code's skeletons (`sub_820D3DE0`,
  `sub_820D3E40`, `sub_820D3B58`, `sub_820D6010`, `sub_820D6090`,
  `sub_820D6238`, `sub_820D63A0`, `sub_820D7C40`, `sub_820D7CC0`,
  `sub_820D7AB0`, `sub_820D02D0`, `sub_820D6528`, `sub_820D8430`;
  `820D0000`-`820D8908`), picked as closest to ready (31 functions in
  ready groups of 49 left). Mapped as the scene's loading objects
  (`scene-loaders.md`, 78 new rows; 2616cc8; 3 leads added): the first
  scene pool set's seven loading classes, not one skeleton class. Map
  checked (0 errors, 78 of 78 in play, 16 ready groups, no kernel calls:
  `VirtualAlloc` only through its wrapper, routed to the allocation
  layer). Migration 1 done (4b47c12, dfbc5a0: the leaves, `ScenePools::MakeA`,
  the pool node, the classes' `IsLoaded`, `CarParts::Part`, the XPR
  name lookups, `CarModelLoads`' constructor and destructor; 5 Native
  with no adapter, none removable; `--native_scene_loaders`;
  `itc_2018`: 23 s, 0 errors, every harness 0 differing but the
  expected groups; draw lists the same as the fallback). Nothing ready
  left: the rest waits on `LoadSlot::Reset` `sub_820D0730` and
  `ReleaseBuffer` `sub_820D0758`, the part model's `sub_820CE9C8`,
  `sub_820CE990`, `sub_820CE928` (`models.md`), the accessories' release
  and others. Step: validation agent 1 running.

## Done

- The app's update and its state stack (`app-states.md`, mapped in
  ac92c3b; 56 rows): 56 ours in two migrations (7bb4dfd..30892a7), 19
  adapters removed, 4 Native with none, 1 Merged; validated
  (`itc_1996`, `itc_2012`); `--native_app_states`.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-27 22:31").

## Expected to differ

This run's groups: `expected-differences.md` (the "22:31 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 2255, work left 4197 (wrapped 104, known 516, unknown 3577).
  `b7_status.py`'s one check at the start: `movies.md:108`
  `sub_82108018`'s row says never called, but it ran (not this run's).
- After the app states' map: ours 2255, work left 4197 (wrapped 104,
  known 570, unknown 3523).
- After the app states' migration 1: ours 2286, work left 4166
  (wrapped 103, known 540, unknown 3523).
- After the app states' validation: race 23 s (`itc_1996`), 0 errors,
  every harness 0 differing but the expected groups; replay 127 s
  (`itc_1997`), 0 errors.
- After the app states' migration 2: ours 2311, work left 4141
  (wrapped 103, known 515, unknown 3523).
- After the app states' validation 2: race 23 s (`itc_2012`), 0 errors,
  every harness 0 differing but the expected groups; replay 127 s
  (`itc_2013`), 0 errors.
- After the scene loaders' map: ours 2311, work left 4141 (wrapped
  103, known 593, unknown 3445).
- After the scene loaders' migration 1: ours 2331, work left 4121
  (wrapped 103, known 573, unknown 3445).

## Stopped
