# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, `b7-loop-2026-10-23-1722.md`,
`b7-loop-2026-10-23-2022.md`, `b7-loop-2026-10-24-0325.md`,
`b7-loop-2026-10-24-1601.md`, `b7-loop-2026-10-24-1746.md`, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-24 21:33 on `b7-loop-24-10-26-4` (the user's
  branch), from 953a342; summary `b7-loop-2026-10-24-2133.md`.
- Limit for this run: 3 subsystems.
- Between subsystems (no mapping, not counted): `leads.md`'s top lead,
  `sub_8209F430` ActionMappers::Init (`action-mappers.md`, ready), with
  the harness lines of `sub_8258CEB0` and `sub_8258CEE0` to check.
  Migration done (3ac7be7, 9e36924: Init native, 7 adapters removed;
  action-mappers.md and input.md have nothing ready left; TestPressed's
  and TestRepeat's harness lines seen), validated (`itc_1494`,
  `itc_1495`; all ran in play or the replay). Done.
- Between subsystems (no mapping, not counted): `leads.md`'s three
  cleanup leads: `sub_8212B288` Smoke::~Smoke, `sub_8212BF88`
  ParticlesB::~ParticlesB, `sub_8212BFE8` ParticlesB::ParticlesB and
  their deleting destructors (`race-overlays.md`); `sub_8213BDF0`,
  `sub_8213BE80`, `AiPath::JoinCornerPair`'s steps (`ai-drivers.md`);
  the race overlays' flush-to-zero window (`src/race_overlays/`).
  Migration done (2e5ce7c..b6888ac: the particle arrays, the joins'
  steps with PathCalls' join callback gone, the flush-to-zero windows
  fixed (ThinLTO had moved a `dpps` out of one), 3 AI driver adapters
  removed), validated (`itc_1508`, `itc_1509`; the joins' two steps,
  never seen in play, to `to-drive.md`). Done.
- Subsystem 1 of 3: `leads.md`'s race's screen objects (`sub_8212F148`,
  `sub_821329B8`, `sub_82132778`, `sub_8212FCC8`, `sub_8212EDF8`,
  `sub_8212EED0`; `8212EDF8`-`82133000`). Mapped as the screen objects
  (`screen-objects.md`, 42 rows: 1 moved from `functions.md`, 41 new;
  78f986b; 12 ready groups; 0 leads added); map checked (0 errors, 42
  of 42 in play). Migration 1 done (472d5ac..4e3513e: both maps, the
  rings, strength and teardown; 38 of 42 ours, nothing ready left; 8
  adapters removed; `--native_screen_objects`), validated (`itc_1518`,
  `itc_1519`; `sub_8212EEF8`, never seen in play, to `to-drive.md`).
  Finished.
- Subsystem 2 of 3: `leads.md`'s framework's set-up (`sub_82090A08`,
  `sub_82090788`, `sub_820944C8`, `sub_82094350`, `sub_82090830`, the
  getters `sub_82090760`-`sub_82090788`; 12 of 18 named ready).
  Mapping agent running.

## Done

- The screen objects (`screen-objects.md`, mapped in 78f986b; 42
  rows): 38 ours in one migration (472d5ac..4e3513e), 8 adapters
  removed; validated (`itc_1518`, `itc_1519`);
  `--native_screen_objects`. Left: BuildFan, Init, the constructor (on
  `rand`), `Record` (on the shader pairs).
- Cleanup leads (no mapping): the particle arrays, the joins' steps,
  the flush-to-zero windows (`race-overlays.md`, `ai-drivers.md`;
  2e5ce7c..b6888ac), 3 adapters removed; validated (`itc_1508`,
  `itc_1509`); existing flags.
- Cleanup lead (no mapping): `ActionMappers::Init` (`action-mappers.md`;
  3ac7be7, 9e36924), 7 adapters removed; validated (`itc_1494`,
  `itc_1495`); `--native_action_mappers`.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-24 21:33").

## Expected to differ

This run's groups: `expected-differences.md` (the "21:33 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1794, work left 4967 (wrapped 115, known 585, unknown 4267).
- After ActionMappers::Init's migration: ours 1795, work left 4966
  (wrapped 115, known 584, unknown 4267). Race 27 s (`itc_1493`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing.
- After mapping the screen objects: ours 1800, work left 4961 (wrapped
  115, known 620, unknown 4226).
- After the screen objects' migration 1: ours 1838, work left 4923
  (wrapped 115, known 582, unknown 4226). Race 27 s, 0 errors, every
  harness 0 differing.
- After the three cleanup leads' migration: ours 1800, work left 4961
  (wrapped 115, known 579, unknown 4267). Race 27 s (`itc_1507`), 0
  errors, every harness 0 differing.

## Stopped

Paused 2026-10-24 (the user's call) while the framework's mapping agent
ran: the loop stops after recording its report; no migration started.
1 of 3 subsystems done.
