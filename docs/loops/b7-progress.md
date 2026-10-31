# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-31 13:47 on `b7-loop-31-10-26`, from e25e6ac;
  summary `b7-loop-2026-10-31-1347.md`.
- Limit for this run: 3 subsystems; 2 taken.
- The validation agent's race is `attract_race.sh 120` from this run
  (the user's call, 2026-10-31: the harnesses took 56.7 of the 60 s).
- Cleanup lead (no mapping, not counted): the top of `leads.md`, the
  models, textures and shaders, is ours but for `Model::RecordMaterials`
  (`sub_820CECA0`, `models.md`, ready, 1,666 lines: split the
  migration). Migration checked (0c226b78: `ModelRecordMaterials` in
  `src/scenery_lists/model_draws.cpp` beside `RecordShape`, under
  `--native_models`, harness 1,024 cases 0 differing, frame dump alike;
  8de457d4: the vertex declaration's adapter `sub_82589728` removed;
  no subagents). Validated (`itc_2903`, 120 s: race 61 s, 0 errors,
  harnesses 56.8 s): 2 of 2 harness lines, both ran in play, none to
  drive. Done.
- Cleanup lead (no mapping, not counted): `sub_820A06D0`, the shader
  pair's load callback (`models.md`, ready, 92 lines), left by the one
  before. Migration checked (f0e75f6d: `shader_objects::PairLoaded`,
  named `Shaders::OnPairLoaded` (probably), under `--native_models`, its
  adapter kept (a function pointer), 120 cases 0 differing; 56bb7d82,
  b7789d24: 7 adapters removed, `ShaderFile::Parse`, the two
  `CopyHeader`s, the two shaders' `Init`, `TextureProvider::Reserve`,
  `SetSharedShaders`). Validated (`itc_2911`, 120 s: race 61 s, 0
  errors, harnesses 57.0 s): 8 of 8 harness lines, all ran in play,
  none to drive. Done.

- Subsystem 1 of 3: the race HUD holder's object at +1004
  (`sub_82216BA8`...`sub_82217AC8`; picked as seen in play, updated
  from the race flow, 5 of its 6 roots ready). Mapped (12983d8d,
  `alt-hud.md`, 26 new rows, 21 in play, 4 ready groups, no stop: a
  second dashboard on UI movie 22, `AltHud::`, probably the attract or
  replay races'; with the menu list helpers `sub_8220BBB0`,
  `sub_8220BF00`). Migration checked (515bb2e3, f8054cd4: 21 of 21 in
  play ours, `--native_alt_hud`, code in `src/alt_hud/`, 21 harness
  lines 0 differing; not called in an attract race (`SetUp`, `Tick`,
  `RunMovie` counted 0; `MenuList::Release` twice, from the front end);
  two rows corrected; nothing ready left). Validated (`itc_2920`, 120
  s: race 60 s, 0 errors, harnesses 58.6 s; replay `itc_2921` ok): 21
  of 21 harness lines; 2 ran in the race, 6 in the replay, 13 to drive
  (the tick's side). Subsystem 1 finished.

- Subsystem 2 of 3: the race's post effect users (`sub_8215D100`,
  `sub_8215C750`, `sub_8215D0D8`, `sub_8228F5C0`, `sub_8228F0B0`,
  `sub_821597F0`; picked as 3 of its 4 roots ready, run at each race's
  set-up, with the race object `[0x826D82D4]`'s own lead offered to the
  mapper). Mapped (a3291567, `race-scene.md`, 20 new rows, 19 in play,
  5 ready groups, no stop: `[0x826D82D4]` is the race scene, 31,168
  bytes, its machine at +87936 of the race mode; the race object's
  lead taken in and deleted, `sub_8215AA88` taken from the course
  scenery owners' lead). Migration checked (2c5902ef..454bd46e: the
  machine's `Init` (its frame words replayed as the original leaves
  them, the race states' precedent), the lights' set-up, the effect
  parameters and glows, the pause toggle and `Resume`, `Destroy`; 11 of
  20 ours, `--native_race_scene`, 8 harness lines 0 differing, frame
  dumps alike; nothing ready left, the rest blocked by other leads).
  Step: validation (agent running).

## Done

- The alternate HUD (subsystem 1): `alt-hud.md`, 21 of 21 in play, one
  migration; `--native_alt_hud`; 12983d8d..f8054cd4. Validated
  (`itc_2920`).

- The shader pair's load callback (cleanup lead): `models.md`,
  `Shaders::OnPairLoaded` with 7 adapters removed; `--native_models`;
  f0e75f6d..b7789d24. Validated (`itc_2911`).

- `Model::RecordMaterials` (cleanup lead): `models.md`, with the vertex
  declaration's adapter removed; `--native_models`; 0c226b78..8de457d4.
  Validated (`itc_2903`).

## Rows to review

`review.md`, "Run 2026-10-31 13:47".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3514, work left 2883 (wrapped 103, known 480,
  unknown 2300).
- After `Model::RecordMaterials`' migration (`itc_2902`: race 36 s, 0
  errors, models and scenery_lists 0 differing): ours 3515, work left
  2882 (wrapped 103, known 479, unknown 2300).
- After its validation (`itc_2903`, 120 s: race 61 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3515, work left
  2882.
- After the shader pair callback's migration (`attract_race.sh 40`:
  race 36-38 s, 0 errors, models, shader_objects and scenery_lists 0
  differing): ours 3516, work left 2881 (wrapped 103, known 478,
  unknown 2300).
- After its validation (`itc_2911`, 120 s: race 61 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3516, work left
  2881.
- After the alternate HUD's map: ours 3516, work left 2881 (wrapped
  103, known 499, unknown 2279).
- After the alternate HUD's migration (`itc_2919`: race 37 s, 0
  errors, alt_hud 0 differing): ours 3537, work left 2860 (wrapped 103,
  known 478, unknown 2279).
- After its validation (`itc_2920`, 120 s: race 60 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3537, work left
  2860.
- After the race scene's map: ours 3537, work left 2860 (wrapped 103,
  known 497, unknown 2260).
- After the race scene's migration (`itc_2929`: race 38 s, 0 errors,
  race_scene and post_effects 0 differing): ours 3548, work left 2850
  (wrapped 103, known 487, unknown 2260).

## Stopped
