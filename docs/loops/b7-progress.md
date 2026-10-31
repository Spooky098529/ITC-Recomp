# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-31 13:47 on `b7-loop-31-10-26`, from e25e6ac;
  summary `b7-loop-2026-10-31-1347.md`.
- Limit for this run: 3 subsystems; 0 taken.
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
  `SetSharedShaders`). Step: validation (agent running).

## Done

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

## Stopped
