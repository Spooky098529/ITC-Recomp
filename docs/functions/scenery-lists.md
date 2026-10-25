# Known functions: the course set's lists of models and the model draws

The course set's lists of models: the course's shared set (pool 2,
`CourseSet::`, the course scenery's manager's +112; `course-scenery.md`)
holds six 528-byte lists of models (+528, +1064, +1596, +2124, +2652 and
+3180: a word, the model count, then a 64-slot `List64` from +4 whose data
slots from +268 point to the models; `track-zones.md` has the list's
layout), each filled from an entry of the set's first file by
`ModelList::Load` (`CourseSet::Parse`), the +3180 list also given 20
pointers to point groups by `ModelList::LoadPoints`, and released by
`ModelList::Release` (`CourseSet::Reset`). `Scenery::DrawSetLists`
(`course-scenery.md`) draws the +1596 list's models whole, those of the
+2124, +2652 and +3180 lists that the race picks (by the race data's
flags, the course's layout and the race mode: `sub_82147850`,
`sub_82147958`, `sub_82147B18`; the layout from `Race::CourseLayout`),
the +3180 list's models at each of its points (`PointList::Draw`); the
minimap (`minimap.md`) draws the +1064 list's first models as its map.
Every model of these lists, and of the scenery's areas
(`Scenery::DrawArea`), is drawn one shape at a time by
`SceneryModel::DrawShape`, which records the shape with
`SceneryModel::RecordShape` (as `Model::RecordShape` in `models.md`, with
the course's texture animations) and hands the chunks to the render
queue.

Beside them, the model draws they share with the rest of the game's
models: `Model::IsShapeVisible` (the shape's bounding box against the
view), `Recorder::RecordObject` (vertex c0-c8: the object's matrices;
Wrapped for frame interpolation, `../frame-interpolation.md`), the scene
lights' pixel constants (`Recorder::RecordSceneLights`, pixel c0-c62:
the view, the directional lights and the headlight projectors that
`Scenery::RecordHeadlightProjectors` sets), the depth-only record of a
shape (`Model::RecordShapeDepth`), and `Model::DrawShape` and
`Model::DrawShapeDepth`, the generic counterparts of
`SceneryModel::DrawShape` (from `Scenery::Draw`, `Model::Draw`'s family
and the car code). Also here: the texture provider's slots 2 and 4 (its
vtable `0x8206CCB4`; the provider's class is in `models.md`). The
command format is in `../gpu-functions.md` ("Command format"); command 7
is the shader constants (`Command::SetShaderConstants`, `functions.md`:
r5 = stage << 24 | type << 16 | start << 8 | count). The fields and
globals are in `../known-data.md` (the course set, the race data,
`0x8265ECA0`-`0x8265ED3F`, `0x8296B8B8`, `0x8296B8FC`-`0x8296B90C`,
`0x8296B950`-`0x8296BCDF`).

Mapped by the B7 loop's mapping agent (`../loops/b7-loop.md`) on
2026-10-25 from the lead in `../loops/leads.md` (the lists' range
`82146AA8`-`82148650`), read in our `generated/` code. These belong
next to `course-scenery.md` (the lists are the course set's, drawn from
`Scenery::DrawSetLists` and `Scenery::DrawArea`) and `models.md` (the
model draws), and have their own file per the loop's instructions.
Shared, with rows elsewhere: in `models.md` `Model::MaterialCount`
(`sub_82594AD8`), `Model::Material` (`sub_82594B20`), the shared shader
pairs (`sub_825896D8`, `sub_82589700`), the vertex declaration
(`sub_82589728`), `Model::RecordShape` (`sub_82595550`, only
`Model::DrawShape` calls it), `Model::Model`, `Model::Release` and
`Model::Load` (`sub_825948C0`, `sub_82594970`, `sub_82596920`); in
`command-list.md` the chunks and slots (`sub_8258C9D0`, `sub_8258CA50`,
`sub_8258CAA0`); in `course-scenery.md` the scenery model's destructor
`sub_820F1208` (slot 0 of the vtable `0x82006764`, called through it by
`ModelList::Release`) and `List64` in `track-zones.md`
(`List64::Insert`, `sub_820FE330`); in `files-and-saves.md`
`DataFile::GetEntry` (`sub_82294730`); in `matrix-math.md`
`Matrix::Multiply`, `Copy`, `RotateY` and `MultiplyTranslation`; in
`vehicle-accessors.md` `Stages::Type` (`sub_820FFBE8`); in `sprites.md`
the light billboards (`sub_82148658`) that `sub_82148650` precedes. Not
here: `sub_82146A38` (`functions.md`, the texture animations' lookup,
not seen calling into this file). Not seen in play, so no rows: the
texture provider's resize (`sub_825944B0`) and the debug print
(`sub_825A1318`) under slot 4, and the material animation
`sub_825A1148` under `SceneryModel::RecordShape`.

For the migration (`b7_groups.py`, 2026-10-25): ready are
`ModelList::Release` (its models' vtable slot 0, `sub_820F1208`, is
reached through the vtable and still recompiled), `sub_82146AA8`,
`AnimationSet::CurrentFrame`, the three entry tests and
`Race::CourseLayout` (with the two parts the recompiler split off),
`sub_82148650`, `Model::IsShapeVisible`, `Recorder::RecordObject` (its
interpolation hooks kept), `Recorder::RecordSceneLights` with its two
fillers, `sub_82589970` and the texture provider's slots 2 and 4 (slot
4's resize and debug prints never ran: ported or left on the recompiled
path). Blocked: `ModelList::Load` and `LoadPoints` by `Model::Load`
(`models.md`, waiting on the shader objects' set-up and the memory
blocks, leads in `../loops/leads.md`); `SceneryModel::DrawShape` and
`RecordShape`, `Model::DrawShape`, `DrawShapeDepth` and
`RecordShapeDepth` by `models.md`'s material and shader helpers (ready
there) and `Model::RecordShape`; `PointList::Draw` by
`SceneryModel::DrawShape`. The draws call the render queue through its
vtable (`[0x8296B91C]`, slot 4) and the models' per-draw hook (slot 1).

Migrated by the B7 loop's migration agent (2026-10-25) behind
`--native_scenery_lists` (`src/scenery_lists/`, adapters in
`src/hooks/scenery_lists.cpp`, harness `scenery_lists`): first the lists'
release, the race's picks with `Race::CourseLayout`, the draw state, the
animation frames, the depth declaration and the texture provider's slots
2 and 4 (slot 4's store only). The jump tables of `IsCEntryOn` and
`CourseLayout` are taken as the translation's switches read them.

Status:

- **Native**: plain C++ in `itc_recomp/src/` that doesn't use the
  PowerPC context (`ctx`): normal arguments and host structs (it may
  still read and write guest memory through `src/guest.h` while
  recompiled code shares the data). The game reaches it without `ctx`,
  from native or overridden callers, and the recompiled original is no
  longer called (except behind a fallback flag or to check against it).
  No `REX_FUNC` override of it exists, not even an adapter kept only for
  a harness (that's Overridden until the adapter goes).
  The end state for every function.
- **Overridden**: an `extern "C" REX_FUNC(sub_XXXXXXXX)` override in
  `itc_recomp/src/` (or a macro defining one) does the work instead of
  the original: a thin adapter in front of native code, or a body that
  still uses `ctx` (calling guest code, reading registers). It may call
  the original when its flag is off, or to check itself against it.
- **Wrapped**: an override that does something of its own (skipping
  render-only passes, logging, timing) and still calls the original
  (`__imp__sub_XXXXXXXX`) for the work.
- **Dropped**: its work isn't needed on PC (hardware set-up, kernel calls
  the SDK already stubbed), so it no longer runs: an override in
  `itc_recomp/src/` replaces it with nothing (or a fixed return value), or
  its callers, made native, leave the call out (no override then; the row
  says which caller). Logs, a harness or the renderer's frame dumps have
  shown that nothing reads what it did. The original is no longer called,
  except behind a fallback flag while the drop is being checked. A
  function the game never calls in the first place isn't Dropped: its
  status stays blank and its row says it's never called.
- **Merged**: its work is done inside a native caller's plain C++ (a
  few lines only that caller needs), so it has no function or override of
  its own in `itc_recomp/src/`. The row names the caller(s) and the
  harness that covers the work. The original is no longer called, except
  behind the caller's fallback flag.
- **Patched**: the generated code was edited in place (search for
  `PATCH:`).

A function usually moves from recompiled or Wrapped to Overridden to
Native as the `ctx` edge moves up through its callers, or straight to
Dropped when its work isn't needed.
`itc_recomp/tools/check_function_statuses.py` checks this column, in
every file of the tables, against `src/` (an override marked Native, a
status with no override, an override with no row, a row in two files);
run it after changing either, on any branch.

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82148428` | `ModelList::Load` | | r3 = a 528-byte list, r4 = a container of the set's first file (an entry of it, by `DataFile::GetEntry`): the count = the container's first word read little-endian; the `List64` at +4 made empty again inline (as `ModelList::Init`: node i's next i + 1 and data slot i, the free head +256 = 0, +252, +258, +260, +262 = -1, the count +264 = 0); then for each i below the count a 448-byte model (`operator new`, `sub_825A6880`; `Model::Model`, then the vtable `0x82006764`, the scenery model's: slot 0 `sub_820F1208`, slot 1 `sub_82106540`, empty), loaded from the container's entry i (`DataFile::GetEntry(r4, i, 0)`, `Model::Load(model, entry, 0)`), and inserted at the list's end (`List64::Insert(+4, &model, count +264)`); +0 = the count. A null from `operator new` (never seen) still goes to `Model::Load` with 0. From `CourseSet::Parse` (the lists at +528, +1064, +1596, +2124 and +2652, entries 2 to 6) and `ModelList::LoadPoints`. Read in full. Runs in race loads. |
| `sub_82148528` | `ModelList::LoadPoints` | | r3 = the course set's +3180 list, r4 = the container (entry 7): `ModelList::Load`, then +528 to +604 = 20 pointers to 256-byte point groups in the image, `0x82617750` + 256 × i (each four 64-byte groups of four 16-byte points, read by `SetList::Point`, `minimap.md`, and `PointList::Draw`), +608, +612, +616 = 0 and +620 = 20 (the entry count). Only caller `CourseSet::Parse`. Read in full. |
| `sub_82147FB8` | `ModelList::Release` | Overridden | r3 = a list: for each of its +0 models in list order (the node walk from the head +264 as in `List64`; data slot +268 + 4 × slot), `Model::Release` (`sub_82594970`), then, when not null, the model's vtable slot 0 with r4 = 1 (`sub_820F1208` for the scenery model: `Model::~Model` and `operator delete`). The list itself (+0, the `List64`) isn't touched. From `CourseSet::Reset` on the six lists. Calls through pointers: the models' vtable slot 0 (one target seen, `sub_820F1208`, `course-scenery.md`). Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`ModelListRelease`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`): Model::Release (through its adapter, `--native_models`) and the vtable slot 0 stay guest calls, made from a 128-byte frame with the original's return addresses; r3 as the last call leaves it. Harness `scenery_lists` (models of zeros, slot 0 the empty `sub_82106540`). Not seen running in an attract race (race loads and resets only). |
| `sub_82146AA8` | `SceneryModel::ResetDrawState` (probably) | Overridden | r3 = a model: +436 = 0 (the animation set `SceneryModel::RecordShape` reads), +432 = 1 (its sampler state 24), +440 = 0. Leaf. From `Scenery::DrawArea` (each area model before its shapes), `Scenery::DrawSetLists` (the +2124 and +2652 lists' models) and `Scenery::Draw`; `PointList::Draw` does the same inline. Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`ResetDrawState`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists`. |
| `sub_82147D88` | `SceneryModel::DrawShape` | | r3 = a model, r4 = the view-projection, r5 = the world matrix, r6 = a shape, r7 = whether to record the depth pass too, r8 = an optional pixel c76 (4 floats), r9 = an optional range for vertex c86 (two floats), r10 = an offset added to the materials' shared shader pair. Nothing when the model's +28 is 0 or the shape isn't visible (`Model::IsShapeVisible`). Opens a chunk with two 3-slot queue entries, then `Recorder::RecordObject` (r4, r5) and `Recorder::RecordSceneLights`; for group 0 and group 1 when they have materials (`Model::MaterialCount`): the entry's +2 = 3, +4 the constants' chunk, +18 = 3, +20 the lights' chunk, +34 = 4, +36 = `SceneryModel::RecordShape(model, shape, group, 1, r8, r9, r10)`, handed to the render queue (`[0x8296B91C]`'s vtable slot 4) with the sort key `[0x8296B90C]` (group 0, f1 = minus the constants chunk's +72) or `[0x8296B900]` (group 1, f1 = +72). With r7, a 2-slot entry for group 0: `Model::RecordShapeDepth(model, shape, 0, 1, 0, 0)` with the key `[0x8296B908]`. The same as `Model::DrawShape` except for its shape recorder and group test. From `Scenery::DrawArea`, `Scenery::DrawSetLists`, `PointList::Draw`, `Minimap::Draw` and `Scenery::Draw`. Calls through pointers: the render queue's slot 4. Read in full. |
| `sub_82146BD0` | `SceneryModel::RecordShape` | | r3 = a model, r4 = a shape (544 bytes from +40), r5 = the group, r6 = whether to set the shaders, r7, r8, r9 as `SceneryModel::DrawShape`'s r8 to r10. Returns 0 (nothing recorded) when the group has no materials, else the chunk's first slot. As `Model::RecordShape` (`models.md`): command 10, the vertex declaration (`sub_82589728` of the shape's +244, +260, +276), the stream (11; the shape's +288, +320 or with +428 = 1 the buffer at +412, +420) and the index buffer (22, with +428 = 1, from +408's +20), the sampler states 28 of samplers 0-3 from the shape's +432; then each material (512 bytes, `Model::Material`): with r6, its shaders (8 and 9): the shared pair `+392 + r9` (`sub_825896D8`, `sub_82589700`) or, with +392 = 0, the model's own pair (+24 + 16 × the material's +96); vertex c9-c13 (+0 and +16 of the material, its +128 colors from the model's +320 table by +208 or 1.0, 1.0, 1.0, 0.0 and +240; the material animation `sub_825A1148` on +364 when +396 isn't -1, never seen); the model's vtable slot 1 (r4 = shape, r5 = material index, r6 = the constants, r7 = &blend index; empty, `sub_82106540`, for the scenery model); pixel c63-c67; render state 3 by the blend index (+108) from the table at `0x82008E6C`, with group 0 render states 96 = 0 and 100 = +112 (others when the material's +388 is set); render state 56 by +344; the textures: for each of the material's +128, the provider (model +48) slot 5 by (+144, +176) or, when the model's +436 is set and +144 is 0, the current frame of the animation +176 in the set +436 (`AnimationSet::CurrentFrame`; 0 when none), and their sampler states 0, 4 and 24 (the model's +432); the environment textures by +104 bits 0x2 (slot 6) and 0x4 (slot 5); r8 gives vertex c86 = (a, b, 1.0 / (b - a), 0.0) and r7 pixel c76, both put back after the draw from `0x8265ED20` and `0x8265ED30`; the draw (18) from +348 and +340. Then the sampler states 16 and 20 of samplers 0-3 (command 2), 36, and the chunk closed. Calls through pointers: the model's slot 1 and the provider's slots 5 and 6. Read for its outline. From `SceneryModel::DrawShape` only. |
| `sub_82146B80` | `AnimationSet::CurrentFrame` | Overridden | Returns the current frame's entry of animation r4 in set r3 (texture id at +4). r3 = an animation set (count +0, then offsets from the end of the offset table, as `AnimationSet::Advance` in `course-scenery.md`), r4 = an index: the animation at r3 + 4 × (count + 1) + offset[r4]; with its frame count +0 and current frame +4, 0 when the current frame isn't below the count, else the 8-byte entry at the animation + 16 + 4 × count + 8 × current (+0 and +4 handed to the provider's slot 5). Leaf. From `SceneryModel::RecordShape` and the light billboards (`sub_82148658`, `sprites.md`). Read in full 2026-10-25. Moved from `functions.md`. Native in `src/scenery_lists/scenery_lists.cpp` (`AnimationCurrentFrame`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists`. |
| `sub_82147850` | `ModelList::IsBEntryOn` (probably) | Overridden | r3 = the course set's +2124 list (not read), r4 = an entry: 1 when the race data's (`[0x826C5B2C]`) +24 flag for it is set (0x1 entry 3, 0x2 4, 0x4 5, 0x8 6, 0x10 7, 0x20 0, 0x40 1, 0x80 2, 0x100 8, 0x200 9, 0x400 10, 0x800 11), else 0. Leaf. Only caller `Scenery::DrawSetLists`. Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`IsBEntryOn`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists`. |
| `sub_82147958` | `ModelList::IsCEntryOn` (probably) | Overridden | r3 = the course set's +2652 list (not read), r4 = an entry: with the race data (`[0x826C5B2C]`) +0xA3AF0 = 5, 0; 3, entry 24 only; 4 with the current stage's type (`Stages::Type(race + 0x59620, -1)`) 0, entry 24 only; else `Race::CourseLayout`: -1 gives 0, the race data's +0x59AE4 bit 0x80 gives 0; entries 0 to 23 give 1 when the layout is the entry's case value (a switch, its table at `0x82008E80`, values 0 to 23); and entries 16 to 23 give 0 unless the stage list's +1220 has 0x20 or 0x40. Only caller `Scenery::DrawSetLists`. Read in full (the case table not read from the image). Native in `src/scenery_lists/scenery_lists.cpp` (`IsCEntryOn`, with `sub_82147A20`; the case table as the translation reads it: entries 0-23 give layouts 4-11, 14, 15, 12, 13, 0, 1, 3, 2, 16-23) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); `Stages::Type` and `Race::CourseLayout` through their plain C++. Harness `scenery_lists`. |
| `sub_82147A20` | | Merged | Not a function: the switch's case 4 inside `sub_82147958` (compares the layout with 4), which the recompiler made one; ported with `sub_82147958`. Merged into `ModelList::IsCEntryOn`'s plain C++ (`IsCEntryOn`, entry 0: layout 4), covered by its harness (`scenery_lists`); no longer reached with `--native_scenery_lists`. |
| `sub_8228F408` | `Race::CourseLayout` (probably) | Overridden | No arguments: from the race data `[0x826C5B2C]` (-1 when it's 0): its +4 (the course, 0 to 22), +20 flags and the race mode +0xA3AF4, and `Stages::Type(race + 0x59620, -1)`. Mode 8: courses 0 to 7 give 16 to 23 (a switch). Modes 3, 4, 7 or a stage of type 1: courses 0 to 22 give 4 to 23 (another switch; the tables at `0x82011E58` and `0x82011E40`). Mode 2: flag 0x20 gives 2, 0x4 and 0x8 both 3, else 1 with 0x10, else 0. Otherwise -1. From `sub_82147958` and the race HUD object's set-up `sub_822122B0` (a lead in `../loops/leads.md`). Read in full (the case tables not read from the image). Native in `src/scenery_lists/scenery_lists.cpp` (`CourseLayout`, with `sub_8228F484`; the tables as the translation reads them: mode 8's courses 0-7 give 16-19, 23, 23, 22, 23; the other switch's courses 0-7 give 16-23, 8-10 none (-1), 11-22 give 4-15) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`, for `sub_822122B0`; `IsCEntryOn` calls the plain C++). Harness `scenery_lists`. |
| `sub_8228F484` | | Merged | Not a function: the part of `Race::CourseLayout` after its mode 8 test (modes 3, 4, 7 and 2 and their switches), which the recompiler made one; ported with `sub_8228F408`. Merged into `Race::CourseLayout`'s plain C++ (`CourseLayout`), covered by its harness (`scenery_lists`); no longer reached with `--native_scenery_lists`. |
| `sub_82147B18` | `PointList::IsEntryOn` (probably) | Overridden | r3 = the course set's +3180 list (not read), r4 = an entry: 0 without race data (`[0x826C5B2C]`), with its +0x59AE4 bit 0x80, or with +0xA3AF0 = 5; race mode (+0xA3AF4) 4 or 7: 1 when the entry (0 to 7) is the course (+4); mode 3: entries 8 to 15 when the course is the entry + 3 (11 to 18), 16 with 21, 17 with 22, 18 with 19, 19 with 20; else 0. Leaf. From `Scenery::DrawSetLists` and `Minimap::Draw`. Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`PointListIsEntryOn`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists`. |
| `sub_82148058` | `PointList::Draw` | | r3 = the course set's +3180 list, r4 = an entry, r5 = the camera pass (`0x826BAC00` + 592 × the pass description's +20): its view-projection (+144) copied to the stack (`memcpy`, 64 bytes). The list's first model (+436 = 0, +432 = 1, +440 = 0, then +48 = the list's +612, the provider, and +436 = +608, the animation set) drawn at the entry's points of groups +616 to 2 (`[+528 + 4 × entry]`, 16 bytes a point, four a group); the second model at the points of group 3. Each point with y at least -1000.0 (`0x82006808`): an identity turned about y by the point's w × the float at `0x82002948`, then by the float at `0x82002A50` (`Matrix::RotateY` twice), moved to the point (`Matrix::MultiplyTranslation`), and the model's shape 0 drawn (`SceneryModel::DrawShape`, r7 = 0, the range (10000.0, 50000.0) from `0x82002814` and `0x82006804`). Only caller `Scenery::DrawSetLists`, which sets +608, +612 and +616 (the start group from `[0x826D82E8]` + 2184: the entry +84 of the 76-byte records by its +836). Read in full. |
| `sub_82148650` | `PassDescription::SetBillboardPass` (probably) | Overridden | r3 = a pass description, r4 = its +8 (the camera pass), r5 = its +24, r6 = its +28: `sub_8209BD40` (+0 = r4, +4 = r6). From `Scenery::DrawArea` and `Scenery::Draw`, before the light billboards (`sub_82148658`). Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`SetBillboardPass`, with `sub_8209BD40`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists`. |
| `sub_8209BD40` | | Merged | r3 = an object: +0 = r4, +4 = r6 (r5 unused). Leaf. Only caller seen `sub_82148650`. Read in full. Merged into `PassDescription::SetBillboardPass`'s plain C++ (`SetBillboardPass`, its tail call), covered by its harness (`scenery_lists`); no longer reached with `--native_scenery_lists`. |
| `sub_825951E0` | `Model::IsShapeVisible` | Overridden | Multiplies a world matrix and camera into a stack temporary; not recorded (likely a visibility test). Read 2026-10-25: r3 = a model, r4 = the view-projection, r5 = the world matrix, r6 = a shape: world × view-projection (`Matrix::Multiply`), then the shape's eight bounding box corners (+96 of the shape, 544 bytes from the model's +40) transformed and divided by w; corners with w below 0.0 count as near (x and y flipped by the float at `0x82001BE0`), others with z / w below 0.0 as the other way, and x and y against the floats at `0x82002B90` and `0x8206CDC4`; returns 0 when all eight are past one plane, else 1. From `Model::Draw` (`functions.md`), `Model::DrawShape`, `Model::DrawShapeDepth` and `SceneryModel::DrawShape`. Moved from `functions.md`. Native in `src/scenery_lists/model_draws.cpp` (`IsShapeVisible`: the product written in the original's frame at r1 - 96 and noted for interpolation with the multiply's return address `0x8259522C`; the transpose and the corners' dot products as the translation's vector steps, the divide and compares as its doubles) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists` (results 0 and 1 both covered). |
| `sub_825953D0` | `Recorder::RecordObject` | Overridden | The bulk per-object recorder, ~150 calls per pass in a race: records command 7 with vertex constants 0-8: world (r5) * camera view-projection (r4) in c0-c3, the world matrix in c4-c7 (for some objects a projective matrix instead), and the camera's world position in c8. The camera is often a stack copy, not a camera pass. Read 2026-10-25: r3 = the model (not read); opens a chunk, the header (7, 9, 0x00010009), c0-c3 by `Matrix::Multiply`, c4-c7 by `Matrix::Copy`, c8 the 16 bytes at `0x8265ECE0`, then command 36, the chunk closed; returns the header slot. Wrapped in `src/interpolation/blend_state.cpp` (`BlendState_BeforeRecordObject` and `AfterRecordObject`, and the caller's return address and r24-r31 kept for pairing products), so a migration keeps those hooks. From `Model::Draw`, `Model::DrawShape`, `Model::DrawShapeDepth` and `SceneryModel::DrawShape`. Moved from `functions.md`. The recording native in `src/scenery_lists/model_draws.cpp` (`RecordObject`: Matrix::Multiply and Copy through their plain C++, noted for interpolation with the original's return addresses `0x82595438` and `0x82595444`) behind `--native_scenery_lists`, called from its interpolation wrapper in `src/interpolation/blend_state.cpp` (`hooks::SceneryListsRecordObject`), which keeps `BlendState_BeforeRecordObject`, `AfterRecordObject` and the caller's context around it as before; the original with the command list's diagnostics on, a chunk open on another list, or no room. Harness `scenery_lists`; frame dumps' draw lists and `--log_blend_state`'s counts the same with the flag off. |
| `sub_82595490` | `Recorder::RecordSceneLights` (probably) | Overridden | r3 = a model, r4 = the world matrix (neither read): opens a chunk, the header (7, 63, 0x0101003F: pixel c0-c62), c0-c3 the view at `0x8265ECA0` (`Matrix::Copy`), c4 the 16 bytes at `0x8265ECE0`, c5-c39 `SceneLights::RecordDirections`, c40-c62 `SceneLights::RecordProjectors`, command 36, the chunk closed; returns the header slot. Frame interpolation blends these 63 registers per object (`blend_state.cpp`, "per-object pixel shader constants"). From `Model::Draw`, `Model::DrawShape` and `SceneryModel::DrawShape`. Read in full. Native in `src/scenery_lists/model_draws.cpp` (`RecordSceneLights`, the view's Matrix::Copy noted with `0x825954F4`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`; the original with the command list's diagnostics on, a chunk open on another list, or no room); harness `scenery_lists`. The zero-length vectors of `RecordDirections` give the dot product's zeros (its select), not the vector. |
| `sub_8258B980` | `SceneLights::RecordDirections` (probably) | Native | r3 = the constants (35 × 16 bytes): 3 vectors from `0x8296B950` normalized (x, y, z by `vrsqrtefp` with one Newton step; a zero length leaves the vector), 8 from `0x8296B9E0` and 8 from `0x8296BA60` copied, 8 from `0x8296BBE0` normalized, 8 from `0x8296BC60` copied. Leaf. Only caller `Recorder::RecordSceneLights`. Read in full. Native in `src/scenery_lists/model_draws.cpp` (`SceneLightsRecordDirections`; the translation's `vrsqrtefp` estimate, its Newton step and the zero select on SSE vectors), no adapter: only `Recorder::RecordSceneLights`'s plain C++ calls it (its fallback flag `--native_scenery_lists` switches the caller). Covered by `RecordSceneLights`'s harness (`scenery_lists`). |
| `sub_82589B58` | `SceneLights::RecordProjectors` (probably) | Native | r3 = the constants (23 × 16 bytes): the x, y, z at `0x8265ED00` with w 1.0, 3 from `0x8296B980`, 3 from `0x8296B9B0`, then the headlight projector slots: 8 from `0x8296BAE0` and 8 from `0x8296BB60` (`Scenery::RecordHeadlightProjectors`, `course-scenery.md`), copied a float at a time. Leaf. Only caller `Recorder::RecordSceneLights`. Read in full. Native in `src/scenery_lists/model_draws.cpp` (`SceneLightsRecordProjectors`, lfs and stfs), no adapter: only `Recorder::RecordSceneLights`'s plain C++ calls it (its fallback flag `--native_scenery_lists` switches the caller). Covered by `RecordSceneLights`'s harness (`scenery_lists`). |
| `sub_825960D0` | `Model::RecordShapeDepth` (probably) | Overridden | r3 = a model, r4 = a shape, r5 = the group, r6 (not read), r7 = a mask (materials whose +56 doesn't share it skipped; 0 for all), r8 = a byte (materials with +40 set skipped unless it's set). Returns 0 when the group has no materials (the shape's +396 block +8 or +12), else the chunk's first slot: command 10, the vertex declaration by the shape's +260 (`sub_82589970`), the stream (11; +320, or +412 with +428 = 1, stride +264), pixel c43 from `0x8265ED10`, the index buffer (22) with +428 = 1, then each material (from the block + 16, or + 16 + 512 × the group 0 count for group 1): render states (command 1) 100 = +112 and 56 = +344, command 12 with &+352 and the draw (18) from +348 and +340; then 36 and the chunk closed. No shaders or textures: probably the depth-only (shadow) pass. From `SceneryModel::DrawShape`, `Model::DrawShape`, `Model::DrawShapeDepth` and `Model::Draw`. Read in full. Native in `src/scenery_lists/model_draws.cpp` (`RecordShapeDepth`; the materials' slots counted first, so a list without room, or a chunk open on another list, runs the original from the start; the material count and declaration through their plain C++) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists`. |
| `sub_82589970` | `VertexDeclarations::ForDepth` (probably) | Overridden | r3 = a stream type: 11 gives `[0x8296B8D0]`, else `[0x8296B8CC]` (the table at `0x8296B8B8`, +24 and +20). Leaf. Only caller `Model::RecordShapeDepth`. Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`DepthVertexDeclaration`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`); harness `scenery_lists`. |
| `sub_82596B18` | `Model::DrawShape` | | As `SceneryModel::DrawShape` (r3 = a model, r4 = the view-projection, r5 = the world matrix, r6 = a shape, r7 = the depth pass too, r8 and r9 to `Model::RecordShape`'s r7 and r8), with `Model::RecordShape` (`models.md`, r6 = 1) for groups 0 and 1 and the groups tested by the shape's +396 block's +8 and +12 instead of `Model::MaterialCount`. From `Scenery::Draw` (the course set's model's shapes) and `sub_820C8480` (a lead). Calls through pointers: the render queue's slot 4. Read in full. |
| `sub_82596D48` | `Model::DrawShapeDepth` (probably) | Overridden | r3 = a model, r4 = the view-projection, r5 = the world matrix, r6 = a shape, r7 = the group (a byte), r8 = a mask, r9 = a byte: nothing when +28 is 0 or the shape isn't visible; `Recorder::RecordObject`, then a 2-slot entry (+2 = 3, +4 the constants, +18 = 4, +20 = `Model::RecordShapeDepth(model, shape, group, 0, r8, r9)`) handed to the render queue with the key `[0x8296B8FC]` (f1 = minus the constants chunk's +72), when the group has materials. From `Scenery::DrawArea` (pass types 10, 13 and 16, probably the shadow passes), `CarPart::Draw` (`sub_820D06F0`), `sub_820E1D88` (a lead) and `sub_820E6A40` (`functions.md`). Calls through pointers: the render queue's slot 4. Read in full. Native in `src/scenery_lists/model_draws.cpp` (`DrawShapeDepth`: IsShapeVisible's product at the original's frame, r1 - 256; Recorder::RecordObject's plain C++ inside the interpolation's hooks, `BlendState_BeforeRecordObject` and After, with the context its wrapper would keep: return address `0x82596D9C` and r24-r31 = r9, r8, r7, r6, the caller's r28, the model, the world matrix, the view-projection, set with `BlendState_SetRecordContext`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`: the render queue's slot 4 a guest call from a 160-byte frame with the return address `0x82596E60`; the original when any slot would be refused). Harness `scenery_lists` (the queue's slot 4 an empty function; r4, r5 and f1 handed to it compared); frame dumps' draw lists and `--log_blend_state`'s counts the same with the flags off. |
| `sub_82594718` | `TextureProvider::Set` | Overridden | Slot 4 of the texture provider's vtable `0x8206CCB4` (no direct callers): r3 = a provider, r4 = a slot, r5 = a value (a texture, or a texture set: `Scenery::DrawSetLists` sets slot 1 to the course set's XPR at +64): when the vector has no storage or the slot isn't below its capacity, `TextureProvider::Reserve` with 1 (0 for slot 0: the doubling loop runs only below 1 and ends at 0) and two debug prints (`sub_825A1318`, never ran); when the slot isn't below the size, `sub_825944B0` (probably the resize; never ran); then `[+8] + 4 × slot` = r5. In play only the store runs (64 slots). From `Scenery::DrawArea`, `Scenery::DrawSetLists` and `Scenery::Draw` (`course-scenery.md`). Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`TextureProviderSet`, the store only) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`, reached through the vtable): the reserve, the debug prints and the resize (never ran) are left on the recompiled path (the adapter runs the original from the start when the slot isn't below the capacity and the size, or there's no storage). Harness `scenery_lists` (the store's cases). |
| `sub_82594328` | `TextureProvider::ClearAll` | Overridden | Slot 2 of the texture provider's vtable (no direct callers): r3 = a provider: every slot from +8 to +12 set to 0. Leaf. From `Scenery::Draw` (the manager's +228). Read in full. Native in `src/scenery_lists/scenery_lists.cpp` (`TextureProviderClearAll`) behind `--native_scenery_lists` (adapter in `src/hooks/scenery_lists.cpp`, reached through the vtable); harness `scenery_lists`. |
